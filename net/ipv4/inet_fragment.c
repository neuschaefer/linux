/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 * inet fragments management
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * 		Authors:	Pavel Emelyanov <xemul@openvz.org>
 *				Started as consolidation of ipv4/ip_fragment.c,
 *				ipv6/reassembly. and ipv6 nf conntrack reassembly
 */

#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/random.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>

#include <net/inet_frag.h>

static void inet_frag_secret_rebuild(unsigned long dummy)
{
	struct inet_frags *f = (struct inet_frags *)dummy;
	unsigned long now = jiffies;
	int i;

	write_lock(&f->lock);
	get_random_bytes(&f->rnd, sizeof(u32));
	for (i = 0; i < INETFRAGS_HASHSZ; i++) {
		struct inet_frag_queue *q;
		struct hlist_node *p, *n;

		hlist_for_each_entry_safe(q, p, n, &f->hash[i], list) {
			unsigned int hval = f->hashfn(q);

			if (hval != i) {
				hlist_del(&q->list);

				/* Relink to new hash chain. */
				hlist_add_head(&q->list, &f->hash[hval]);
			}
		}
	}
	write_unlock(&f->lock);

	mod_timer(&f->secret_timer, now + f->secret_interval);
}

void inet_frags_init(struct inet_frags *f)
{
	int i;

	for (i = 0; i < INETFRAGS_HASHSZ; i++)
		INIT_HLIST_HEAD(&f->hash[i]);

	rwlock_init(&f->lock);

	f->rnd = (u32) ((num_physpages ^ (num_physpages>>7)) ^
				   (jiffies ^ (jiffies >> 6)));

	setup_timer(&f->secret_timer, inet_frag_secret_rebuild,
			(unsigned long)f);
	f->secret_timer.expires = jiffies + f->secret_interval;
	add_timer(&f->secret_timer);
}
EXPORT_SYMBOL(inet_frags_init);

void inet_frags_init_net(struct netns_frags *nf)
{
	nf->nqueues = 0;
#ifdef CONFIG_ATP_COMMON
    atomic_set(&nf->ipfrag_count, 0); /* add ipfrag count */
#endif
	atomic_set(&nf->mem, 0);
	INIT_LIST_HEAD(&nf->lru_list);
    //CVE-2014-0100
    spin_lock_init(&nf->lru_lock);
}
EXPORT_SYMBOL(inet_frags_init_net);

void inet_frags_fini(struct inet_frags *f)
{
	del_timer(&f->secret_timer);
}
EXPORT_SYMBOL(inet_frags_fini);

void inet_frags_exit_net(struct netns_frags *nf, struct inet_frags *f)
{
	nf->low_thresh = 0;

	local_bh_disable();
	inet_frag_evictor(nf, f);
	local_bh_enable();
}
EXPORT_SYMBOL(inet_frags_exit_net);

static inline void fq_unlink(struct inet_frag_queue *fq, struct inet_frags *f)
{
	write_lock(&f->lock);
	hlist_del(&fq->list);
	fq->net->nqueues--;
	write_unlock(&f->lock);
    //CVE-2014-0100
    inet_frag_lru_del(fq);
}

void inet_frag_kill(struct inet_frag_queue *fq, struct inet_frags *f)
{
	if (del_timer(&fq->timer))
		atomic_dec(&fq->refcnt);

	if (!(fq->last_in & INET_FRAG_COMPLETE)) {
		fq_unlink(fq, f);
		atomic_dec(&fq->refcnt);
		fq->last_in |= INET_FRAG_COMPLETE;
	}
}
EXPORT_SYMBOL(inet_frag_kill);

static inline void frag_kfree_skb(struct netns_frags *nf, struct inet_frags *f,
		struct sk_buff *skb, int *work)
{
	if (work)
		*work -= skb->truesize;

	atomic_sub(skb->truesize, &nf->mem);
#ifdef CONFIG_ATP_COMMON
	atomic_sub(1, &nf->ipfrag_count); /* add ipfrag count */
#endif
    
	if (f->skb_free)
		f->skb_free(skb);
	kfree_skb(skb);
}

void inet_frag_destroy(struct inet_frag_queue *q, struct inet_frags *f,
					int *work)
{
	struct sk_buff *fp;
	struct netns_frags *nf;

	WARN_ON(!(q->last_in & INET_FRAG_COMPLETE));
	WARN_ON(del_timer(&q->timer) != 0);

	/* Release all fragment data. */
	fp = q->fragments;
	nf = q->net;
	while (fp) {
		struct sk_buff *xp = fp->next;

		frag_kfree_skb(nf, f, fp, work);
		fp = xp;
	}

	if (work)
		*work -= f->qsize;
	atomic_sub(f->qsize, &nf->mem);

	if (f->destructor)
		f->destructor(q);
	kfree(q);

}
EXPORT_SYMBOL(inet_frag_destroy);

int inet_frag_evictor(struct netns_frags *nf, struct inet_frags *f)
{
	struct inet_frag_queue *q;
	int work, evicted = 0;

#ifdef CONFIG_ATP_COMMON
    /* start add ipfrag count */
    if ((atomic_read(&nf->ipfrag_count) > nf->ipfrag_count_thresh)
        && (atomic_read(&nf->mem) > (IPFRAG_PACKET_TRRUSIZE_MIN * nf->ipfrag_count_thresh))
        && (nf->low_thresh > (IPFRAG_PACKET_TRRUSIZE_MIN * nf->ipfrag_count_thresh)))
    {
    	work = atomic_read(&nf->mem) - (IPFRAG_PACKET_TRRUSIZE_MIN * nf->ipfrag_count_thresh);
    }
    else
#endif
    {
	    work = atomic_read(&nf->mem) - nf->low_thresh;
    }
    /* end add ipfrag count */

	while (work > 0) {
        //CVE-2014-0100
		spin_lock(&nf->lru_lock);
        
		if (list_empty(&nf->lru_list)) {
			spin_unlock(&nf->lru_lock);
			break;
		}

		q = list_first_entry(&nf->lru_list,
				struct inet_frag_queue, lru_list);
		atomic_inc(&q->refcnt);
		spin_unlock(&nf->lru_lock);

		spin_lock(&q->lock);
		if (!(q->last_in & INET_FRAG_COMPLETE))
			inet_frag_kill(q, f);
		spin_unlock(&q->lock);

		if (atomic_dec_and_test(&q->refcnt))
			inet_frag_destroy(q, f, &work);
		evicted++;
	}

	return evicted;
}
EXPORT_SYMBOL(inet_frag_evictor);

static struct inet_frag_queue *inet_frag_intern(struct netns_frags *nf,
		struct inet_frag_queue *qp_in, struct inet_frags *f,
		void *arg)
{
	struct inet_frag_queue *qp;
#ifdef CONFIG_SMP
	struct hlist_node *n;
#endif
	unsigned int hash;

	write_lock(&f->lock);
	/*
	 * While we stayed w/o the lock other CPU could update
	 * the rnd seed, so we need to re-calculate the hash
	 * chain. Fortunatelly the qp_in can be used to get one.
	 */
	hash = f->hashfn(qp_in);
#ifdef CONFIG_SMP
	/* With SMP race we have to recheck hash table, because
	 * such entry could be created on other cpu, while we
	 * promoted read lock to write lock.
	 */
	hlist_for_each_entry(qp, n, &f->hash[hash], list) {
		if (qp->net == nf && f->match(qp, arg)) {
			atomic_inc(&qp->refcnt);
			write_unlock(&f->lock);
			qp_in->last_in |= INET_FRAG_COMPLETE;
			inet_frag_put(qp_in, f);
			return qp;
		}
	}
#endif
	qp = qp_in;
	if (!mod_timer(&qp->timer, jiffies + nf->timeout))
		atomic_inc(&qp->refcnt);

	atomic_inc(&qp->refcnt);
	hlist_add_head(&qp->list, &f->hash[hash]);
	nf->nqueues++;
	write_unlock(&f->lock);
    //CVE-2014-0100
    inet_frag_lru_add(nf, qp);
	return qp;
}

static struct inet_frag_queue *inet_frag_alloc(struct netns_frags *nf,
		struct inet_frags *f, void *arg)
{
	struct inet_frag_queue *q;

	q = kzalloc(f->qsize, GFP_ATOMIC);
	if (q == NULL)
		return NULL;

	f->constructor(q, arg);
	atomic_add(f->qsize, &nf->mem);
	setup_timer(&q->timer, f->frag_expire, (unsigned long)q);
	spin_lock_init(&q->lock);
	atomic_set(&q->refcnt, 1);
	q->net = nf;

	return q;
}

static struct inet_frag_queue *inet_frag_create(struct netns_frags *nf,
		struct inet_frags *f, void *arg)
{
	struct inet_frag_queue *q;

	q = inet_frag_alloc(nf, f, arg);
	if (q == NULL)
		return NULL;

	return inet_frag_intern(nf, q, f, arg);
}

struct inet_frag_queue *inet_frag_find(struct netns_frags *nf,
		struct inet_frags *f, void *key, unsigned int hash)
	__releases(&f->lock)
{
	struct inet_frag_queue *q;
	struct hlist_node *n;

	hlist_for_each_entry(q, n, &f->hash[hash], list) {
		if (q->net == nf && f->match(q, key)) {
			atomic_inc(&q->refcnt);
			read_unlock(&f->lock);
			return q;
		}
	}
	read_unlock(&f->lock);

	return inet_frag_create(nf, f, key);
}
EXPORT_SYMBOL(inet_frag_find);
