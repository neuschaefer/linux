/*
 * Copyright (c) 2010-2011 Broadcom Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "vchiq_core.h"

#define VCHIQ_SLOT_HANDLER_STACK 8192

#define SLOT_INFO_FROM_INDEX(state, index) (state->slot_info + (index))
#define SLOT_DATA_FROM_INDEX(state, index) (state->slot_data + (index))
#define SLOT_INDEX_FROM_DATA(state, data) \
	(((unsigned int)((char *)data - (char *)state->slot_data)) / \
	VCHIQ_SLOT_SIZE)
#define SLOT_INDEX_FROM_INFO(state, info) \
	((unsigned int)(info - state->slot_info))
#define SLOT_QUEUE_INDEX_FROM_POS(pos) \
	((int)((unsigned int)(pos) / VCHIQ_SLOT_SIZE))


#define BULK_INDEX(x) (x & (VCHIQ_NUM_SERVICE_BULKS - 1))


struct vchiq_open_payload {
	int fourcc;
	int client_id;
	short version;
	short version_min;
};

/* we require this for consistency between endpoints */
vchiq_static_assert(sizeof(VCHIQ_HEADER_T) == 8);
vchiq_static_assert(IS_POW2(sizeof(VCHIQ_HEADER_T)));
vchiq_static_assert(IS_POW2(VCHIQ_NUM_CURRENT_BULKS));
vchiq_static_assert(IS_POW2(VCHIQ_NUM_SERVICE_BULKS));
vchiq_static_assert(VCHIQ_VERSION >= VCHIQ_VERSION_MIN);

/* Run time control of log level, based on KERN_XXX level. */
int vchiq_core_log_level = VCHIQ_LOG_DEFAULT;
int vchiq_core_msg_log_level = VCHIQ_LOG_DEFAULT;
int vchiq_sync_log_level = VCHIQ_LOG_DEFAULT;

static atomic_t pause_bulks_count = ATOMIC_INIT(0);

static const char *const srvstate_names[] = {
	"FREE",
	"HIDDEN",
	"LISTENING",
	"OPENING",
	"OPEN",
	"OPENSYNC",
	"CLOSESENT",
	"CLOSING",
	"CLOSEWAIT"
};

static const char *const reason_names[] = {
	"SERVICE_OPENED",
	"SERVICE_CLOSED",
	"MESSAGE_AVAILABLE",
	"BULK_TRANSMIT_DONE",
	"BULK_RECEIVE_DONE",
	"BULK_TRANSMIT_ABORTED",
	"BULK_RECEIVE_ABORTED"
};

static const char *const conn_state_names[] = {
	"DISCONNECTED",
	"CONNECTING",
	"CONNECTED",
	"PAUSING",
	"PAUSE_SENT",
	"PAUSED",
	"RESUMING"
};


static void
release_message_sync(VCHIQ_STATE_T *state, VCHIQ_HEADER_T *header);

static const char *msg_type_str(unsigned int msg_type)
{
	switch (msg_type) {
	case VCHIQ_MSG_PADDING:       return "PADDING";
	case VCHIQ_MSG_CONNECT:       return "CONNECT";
	case VCHIQ_MSG_OPEN:          return "OPEN";
	case VCHIQ_MSG_OPENACK:       return "OPENACK";
	case VCHIQ_MSG_CLOSE:         return "CLOSE";
	case VCHIQ_MSG_DATA:          return "DATA";
	case VCHIQ_MSG_BULK_RX:       return "BULK_RX";
	case VCHIQ_MSG_BULK_TX:       return "BULK_TX";
	case VCHIQ_MSG_BULK_RX_DONE:  return "BULK_RX_DONE";
	case VCHIQ_MSG_BULK_TX_DONE:  return "BULK_TX_DONE";
	case VCHIQ_MSG_PAUSE:         return "PAUSE";
	case VCHIQ_MSG_RESUME:        return "RESUME";
	case VCHIQ_MSG_REMOTE_USE:    return "REMOTE_USE";
	case VCHIQ_MSG_REMOTE_RELEASE:      return "REMOTE_RELEASE";
	case VCHIQ_MSG_REMOTE_USE_ACTIVE:   return "REMOTE_USE_ACTIVE";
	}
	return "???";
}

static inline void
vchiq_set_service_state(VCHIQ_SERVICE_T *service, int newstate)
{
	vchiq_log_info(vchiq_core_log_level, "%d: srv:%d %s->%s",
		service->state->id, service->localport,
		srvstate_names[service->srvstate],
		srvstate_names[newstate]);
	service->srvstate = newstate;
}

static inline int
is_valid_service(VCHIQ_SERVICE_T *service)
{
	if ((service != NULL) && (service->srvstate != VCHIQ_SRVSTATE_FREE)) {
		if (service->magic == VCHIQ_MAGIC)
			return 1;
		else
			vchiq_log_error(vchiq_core_log_level,
				"Invalid service magic number");
	}
	return 0;
}

static inline VCHIQ_STATUS_T
make_service_callback(VCHIQ_SERVICE_T *service, VCHIQ_REASON_T reason,
	VCHIQ_HEADER_T *header, void *bulk_userdata)
{
	vchiq_log_trace(vchiq_core_log_level, "%d: callback:%d (%s, %x, %x)",
		service->state->id, service->localport, reason_names[reason],
		(unsigned int)header, (unsigned int)bulk_userdata);
	return service->base.callback(reason, header, &service->base,
		bulk_userdata);
}

static inline void
vchiq_set_conn_state(VCHIQ_STATE_T *state, VCHIQ_CONNSTATE_T newstate)
{
	VCHIQ_CONNSTATE_T oldstate = state->conn_state;
	vchiq_log_info(vchiq_core_log_level, "%d: %s->%s", state->id,
		conn_state_names[oldstate],
		conn_state_names[newstate]);
	state->conn_state = newstate;
	vchiq_platform_conn_state_changed(state, oldstate, newstate);
}

static inline void
remote_event_create(REMOTE_EVENT_T *event)
{
	event->armed = 0;
	/* Don't clear the 'fired' flag because it may already have been set
	** by the other side. */
	sema_init(event->event, 0);
}

static inline void
remote_event_destroy(REMOTE_EVENT_T *event)
{
	(void)event;
}

static inline int
remote_event_wait(REMOTE_EVENT_T *event)
{
	if (!event->fired) {
		event->armed = 1;
		dsb();
		if (!event->fired) {
			if (down_interruptible(event->event) != 0) {
				event->armed = 0;
				return 0;
			}
		}
		event->armed = 0;
		wmb();
	}

	event->fired = 0;
	return 1;
}

static inline void
remote_event_signal_local(REMOTE_EVENT_T *event)
{
	event->armed = 0;
	up(event->event);
}

static inline void
remote_event_poll(REMOTE_EVENT_T *event)
{
	if (event->fired && event->armed)
		remote_event_signal_local(event);
}

void
remote_event_pollall(VCHIQ_STATE_T *state)
{
	remote_event_poll(&state->local->sync_trigger);
	remote_event_poll(&state->local->sync_release);
	remote_event_poll(&state->local->trigger);
	remote_event_poll(&state->local->recycle);
}

/* Round up message sizes so that any space at the end of a slot is always big
** enough for a header. This relies on header size being a power of two, which
** has been verified earlier by a static assertion. */

static inline unsigned int
calc_stride(unsigned int size)
{
	/* Allow room for the header */
	size += sizeof(VCHIQ_HEADER_T);

	/* Round up */
	return (size + sizeof(VCHIQ_HEADER_T) - 1) & ~(sizeof(VCHIQ_HEADER_T)
		- 1);
}

static VCHIQ_SERVICE_T *
get_listening_service(VCHIQ_STATE_T *state, int fourcc)
{
	int i;

	WARN_ON(fourcc == VCHIQ_FOURCC_INVALID);

	for (i = 0; i < state->unused_service; i++) {
		VCHIQ_SERVICE_T *service = state->services[i];
		if (service &&
			(service->public_fourcc == fourcc) &&
			((service->srvstate == VCHIQ_SRVSTATE_LISTENING) ||
			((service->srvstate == VCHIQ_SRVSTATE_OPEN) &&
			(service->remoteport == VCHIQ_PORT_FREE))))
			return service;
	}

	return NULL;
}

static VCHIQ_SERVICE_T *
get_connected_service(VCHIQ_STATE_T *state, unsigned int port)
{
	int i;
	for (i = 0; i < state->unused_service; i++) {
		VCHIQ_SERVICE_T *service = state->services[i];
		if (service && (service->srvstate == VCHIQ_SRVSTATE_OPEN)
			&& (service->remoteport == port))
			return service;
	}
	return NULL;
}

inline void
request_poll(VCHIQ_STATE_T *state, VCHIQ_SERVICE_T *service, int poll_type)
{
	uint32_t value;

	if (service) {
		do {
			value = atomic_read(&service->poll_flags);
		} while (atomic_cmpxchg(&service->poll_flags, value,
			value | (1 << poll_type)) != value);

		do {
			value = atomic_read(&state->poll_services[
				service->localport>>5]);
		} while (atomic_cmpxchg(
			&state->poll_services[service->localport>>5],
			value, value | (1 << (service->localport & 0x1f)))
			!= value);
	}

	state->poll_needed = 1;
	wmb();

	/* ... and ensure the slot handler runs. */
	remote_event_signal_local(&state->local->trigger);
}

/* Called from queue_message, by the slot handler and application threads,
	with slot_mutex held */
static VCHIQ_HEADER_T *
reserve_space(VCHIQ_STATE_T *state, int space, int is_blocking)
{
	VCHIQ_SHARED_STATE_T *local = state->local;
	int tx_pos = state->local_tx_pos;
	int slot_space = VCHIQ_SLOT_SIZE - (tx_pos & VCHIQ_SLOT_MASK);

	if (space > slot_space) {
		VCHIQ_HEADER_T *header;
		/* Fill the remaining space with padding */
		WARN_ON(state->tx_data == NULL);
		header = (VCHIQ_HEADER_T *)
			(state->tx_data + (tx_pos & VCHIQ_SLOT_MASK));
		header->msgid = VCHIQ_MSGID_PADDING;
		header->size = slot_space - sizeof(VCHIQ_HEADER_T);

		tx_pos += slot_space;
	}

	/* If necessary, get the next slot. */
	if ((tx_pos & VCHIQ_SLOT_MASK) == 0) {
		int slot_index;

		/* If there is no free slot... */
		if (tx_pos == (state->slot_queue_available * VCHIQ_SLOT_SIZE)) {
			/* ...wait for one. */
			VCHIQ_STATS_INC(state, slot_stalls);

			/* But first, flush through the last slot. */
			local->tx_pos = tx_pos;
			remote_event_signal(&state->remote->trigger);

			do {
				if (!is_blocking ||
					(down_interruptible(
					&state->slot_available_event) != 0)) {
					return NULL; /* No space available */
				}
			} while (tx_pos == (state->slot_queue_available *
				VCHIQ_SLOT_SIZE));
		}

		slot_index = local->slot_queue[
			SLOT_QUEUE_INDEX_FROM_POS(tx_pos) &
			VCHIQ_SLOT_QUEUE_MASK];
		state->tx_data =
			(char *)SLOT_DATA_FROM_INDEX(state, slot_index);
	}

	state->local_tx_pos = tx_pos + space;

	return (VCHIQ_HEADER_T *)(state->tx_data + (tx_pos & VCHIQ_SLOT_MASK));
}

/* Called with slot_mutex held */
static void
process_free_queue(VCHIQ_STATE_T *state)
{
	VCHIQ_SHARED_STATE_T *local = state->local;
	BITSET_T service_found[BITSET_SIZE(VCHIQ_MAX_SERVICES)];
	int slot_queue_available;

	/* Use a read memory barrier to ensure that any state that may have
	** been modified by another thread is not masked by stale prefetched
	** values. */
	rmb();

	/* Find slots which have been freed by the other side, and return them
	** to the available queue. */
	slot_queue_available = state->slot_queue_available;

	while (slot_queue_available != local->slot_queue_recycle) {
		unsigned int pos;
		int slot_index = local->slot_queue[slot_queue_available++ &
			VCHIQ_SLOT_QUEUE_MASK];
		char *data = (char *)SLOT_DATA_FROM_INDEX(state, slot_index);

		vchiq_log_trace(vchiq_core_log_level, "%d: pfq %d=%x %x %x",
			state->id, slot_index, (unsigned int)data,
			local->slot_queue_recycle, slot_queue_available);

		/* Initialise the bitmask for services which have used this
		** slot */
		BITSET_ZERO(service_found);

		pos = 0;

		while (pos < VCHIQ_SLOT_SIZE) {
			VCHIQ_HEADER_T *header =
				(VCHIQ_HEADER_T *)(data + pos);
			int msgid = header->msgid;
			if (VCHIQ_MSG_TYPE(msgid) == VCHIQ_MSG_DATA) {
				int port = VCHIQ_MSG_SRCPORT(msgid);
				VCHIQ_SERVICE_QUOTA_T *service_quota =
					&state->service_quotas[port];
				int count;
				count = service_quota->message_use_count;
				if (count > 0) {
					service_quota->message_use_count =
						count - 1;
					if (count ==
						service_quota->message_quota) {
						/* Signal the service that it
						** has dropped below its quota
						*/
						up(&service_quota->quota_event);
					}
				} else {
					vchiq_log_error(vchiq_core_log_level,
						"service %d "
						"message_use_count=%d "
						"(header %x, msgid %x, "
						"header->msgid %x, "
						"header->size %x)",
						port,
						service_quota->
							message_use_count,
						(unsigned int)header, msgid,
						header->msgid,
						header->size);
					WARN(1, "invalid use count\n");
				}
				if (!BITSET_IS_SET(service_found, port)) {
					/* Set the found bit for this service */
					BITSET_SET(service_found, port);

					count = service_quota->slot_use_count;
					if (count > 0) {
						service_quota->slot_use_count =
							count - 1;
						/* Signal the service in case
						** it has dropped below its
						** quota */
						up(&service_quota->quota_event);
						vchiq_log_trace(
							vchiq_core_log_level,
							"%d: pfq:%d %x@%x - "
							"slot_use->%d",
							state->id, port,
							header->size,
							(unsigned int)header,
							service_quota->
								slot_use_count);
					} else {
						vchiq_log_error(
							vchiq_core_log_level,
								"service %d "
								"slot_use_count"
								"=%d (header %x"
								", msgid %x, "
								"header->msgid"
								" %x, header->"
								"size %x)",
							port,
							service_quota->
								slot_use_count,
							(unsigned int)header,
							msgid,
							header->msgid,
							header->size);
						WARN(1, "invalid use count\n");
					}
				}
			}

			pos += calc_stride(header->size);
			if (pos > VCHIQ_SLOT_SIZE) {
				vchiq_log_error(vchiq_core_log_level,
					"pfq - pos %x: header %x, msgid %x, "
					"header->msgid %x, header->size %x",
					pos, (unsigned int)header, msgid,
					header->msgid, header->size);
				WARN(1, "invalid slot position\n");
			}
		}
	}

	if (slot_queue_available != state->slot_queue_available) {
		state->slot_queue_available = slot_queue_available;
		wmb();
		up(&state->slot_available_event);
	}
}

/* Called by the slot handler and application threads */
static VCHIQ_STATUS_T
queue_message(VCHIQ_STATE_T *state, VCHIQ_SERVICE_T *service,
	int msgid, const VCHIQ_ELEMENT_T *elements,
	int count, int size, int is_blocking)
{
	VCHIQ_SHARED_STATE_T *local;
	VCHIQ_SERVICE_QUOTA_T *service_quota = NULL;
	VCHIQ_HEADER_T *header;

	unsigned int stride;

	local = state->local;

	stride = calc_stride(size);

	WARN_ON(!(stride <= VCHIQ_SLOT_SIZE));

	if ((VCHIQ_MSG_TYPE(msgid) != VCHIQ_MSG_RESUME) &&
		(mutex_lock_interruptible(&state->slot_mutex) != 0))
		return VCHIQ_RETRY;

	if (service) {
		int tx_end_index = SLOT_QUEUE_INDEX_FROM_POS(
			state->local_tx_pos + stride - 1);

		if (service->srvstate != VCHIQ_SRVSTATE_OPEN) {
			/* The service has been closed, probably while waiting
			** for the mutex */
			mutex_unlock(&state->slot_mutex);
			return VCHIQ_ERROR;
		}

		service_quota = &state->service_quotas[service->localport];

		/* ...ensure it doesn't use more than its quota of messages or
		** slots */
		while ((service_quota->message_use_count ==
				service_quota->message_quota) ||
			((tx_end_index != service_quota->previous_tx_index) &&
			(service_quota->slot_use_count ==
				service_quota->slot_quota))) {
			vchiq_log_trace(vchiq_core_log_level,
				"%d: qm:%d %s,%x - quota stall "
				"(msg %d, slot %d)",
				state->id, service->localport,
				msg_type_str(VCHIQ_MSG_TYPE(msgid)), size,
				service_quota->message_use_count,
				service_quota->slot_use_count);
			VCHIQ_SERVICE_STATS_INC(service, quota_stalls);
			mutex_unlock(&state->slot_mutex);
			if (down_interruptible(&service_quota->quota_event)
				!= 0)
				return VCHIQ_RETRY;
			if (mutex_lock_interruptible(&state->slot_mutex) != 0)
				return VCHIQ_RETRY;
			tx_end_index = SLOT_QUEUE_INDEX_FROM_POS(
				state->local_tx_pos + stride - 1);
		}
	}

	header = reserve_space(state, stride, is_blocking);

	if (!header) {
		if (service)
			VCHIQ_SERVICE_STATS_INC(service, slot_stalls);
		mutex_unlock(&state->slot_mutex);
		return VCHIQ_RETRY;
	}

	if (service) {
		int i, pos;
		int tx_end_index;

		vchiq_log_info(vchiq_core_log_level,
			"%d: qm %s@%x,%x (%d->%d)",
			state->id,
			msg_type_str(VCHIQ_MSG_TYPE(msgid)),
			(unsigned int)header, size,
			VCHIQ_MSG_SRCPORT(msgid),
			VCHIQ_MSG_DSTPORT(msgid));

		for (i = 0, pos = 0; i < (unsigned int)count;
			pos += elements[i++].size)
			if (elements[i].size) {
				if (vchiq_copy_from_user
					(header->data + pos, elements[i].data,
					(size_t) elements[i].size) !=
					VCHIQ_SUCCESS) {
					mutex_unlock(&state->slot_mutex);
					VCHIQ_SERVICE_STATS_INC(service,
						error_count);
					return VCHIQ_ERROR;
				}
				if (i == 0) {
					if (vchiq_core_msg_log_level >=
						VCHIQ_LOG_INFO)
						vchiq_log_dump_mem("Sent", 0,
							header->data + pos,
							min(64,
							elements[0].size));
				}
			}

		/* If this transmission can't fit in the last slot used by this
		** service... */
		tx_end_index =
			SLOT_QUEUE_INDEX_FROM_POS(state->local_tx_pos - 1);
		if (tx_end_index != service_quota->previous_tx_index) {
			service_quota->slot_use_count++;
			vchiq_log_trace(vchiq_core_log_level,
				"%d: qm:%d %s,%x - slot_use->%d",
				state->id, service->localport,
				msg_type_str(VCHIQ_MSG_TYPE(msgid)), size,
				service_quota->slot_use_count);
		}

		service_quota->previous_tx_index = tx_end_index;
		service_quota->message_use_count++;
		VCHIQ_SERVICE_STATS_INC(service, ctrl_tx_count);
		VCHIQ_SERVICE_STATS_ADD(service, ctrl_tx_bytes, size);
	} else {
		vchiq_log_info(vchiq_core_log_level,
			"%d: qm %s@%x,%x (%d->%d)", state->id,
			msg_type_str(VCHIQ_MSG_TYPE(msgid)),
			(unsigned int)header, size,
			VCHIQ_MSG_SRCPORT(msgid),
			VCHIQ_MSG_DSTPORT(msgid));
		if (size != 0) {
			WARN_ON(!((count == 1) && (size == elements[0].size)));
			memcpy(header->data, elements[0].data,
				elements[0].size);
		}
		VCHIQ_STATS_INC(state, ctrl_tx_count);
	}

	header->msgid = msgid;
	header->size = size;

	{
		int svc_fourcc;

		svc_fourcc = service
			? service->base.fourcc
			: VCHIQ_MAKE_FOURCC('?', '?', '?', '?');

		vchiq_log_info(vchiq_core_msg_log_level,
			"Sent Msg %s(%u) to %c%c%c%c s:%u d:%d len:%d",
			msg_type_str(VCHIQ_MSG_TYPE(msgid)),
			VCHIQ_MSG_TYPE(msgid),
			VCHIQ_FOURCC_AS_4CHARS(svc_fourcc),
			VCHIQ_MSG_SRCPORT(msgid),
			VCHIQ_MSG_DSTPORT(msgid),
			size);
	}

	/* Make sure the new header is visible to the peer. */
	wmb();

	/* Make the new tx_pos visible to the peer. */
	local->tx_pos = state->local_tx_pos;
	wmb();

	if (VCHIQ_MSG_TYPE(msgid) != VCHIQ_MSG_PAUSE)
		mutex_unlock(&state->slot_mutex);

	remote_event_signal(&state->remote->trigger);

	return VCHIQ_SUCCESS;
}

/* Called by the slot handler and application threads */
static VCHIQ_STATUS_T
queue_message_sync(VCHIQ_STATE_T *state, VCHIQ_SERVICE_T *service,
	int msgid, const VCHIQ_ELEMENT_T *elements,
	int count, int size, int is_blocking)
{
	VCHIQ_SHARED_STATE_T *local;
	VCHIQ_HEADER_T *header;

	local = state->local;

	if ((VCHIQ_MSG_TYPE(msgid) != VCHIQ_MSG_RESUME) &&
		(mutex_lock_interruptible(&state->sync_mutex) != 0))
		return VCHIQ_RETRY;

	remote_event_wait(&local->sync_release);

	rmb();

	header = (VCHIQ_HEADER_T *)SLOT_DATA_FROM_INDEX(state,
		local->slot_sync);

	{
		int oldmsgid = header->msgid;
		if (oldmsgid != VCHIQ_MSGID_PADDING)
			vchiq_log_error(vchiq_core_log_level,
				"%d: qms - msgid %x, not PADDING",
				state->id, oldmsgid);
	}

	if (service) {
		int i, pos;

		vchiq_log_info(vchiq_sync_log_level,
			"%d: qms %s@%x,%x (%d->%d)", state->id,
			msg_type_str(VCHIQ_MSG_TYPE(msgid)),
			(unsigned int)header, size,
			VCHIQ_MSG_SRCPORT(msgid),
			VCHIQ_MSG_DSTPORT(msgid));

		for (i = 0, pos = 0; i < (unsigned int)count;
			pos += elements[i++].size)
			if (elements[i].size) {
				if (vchiq_copy_from_user
					(header->data + pos, elements[i].data,
					(size_t) elements[i].size) !=
					VCHIQ_SUCCESS) {
					mutex_unlock(&state->sync_mutex);
					VCHIQ_SERVICE_STATS_INC(service,
						error_count);
					return VCHIQ_ERROR;
				}
				if (i == 0) {
					if (vchiq_sync_log_level >=
						VCHIQ_LOG_TRACE)
						vchiq_log_dump_mem("Sent Sync",
							0, header->data + pos,
							min(64,
							elements[0].size));
				}
			}

		/* If this transmission can't fit in the last slot used by this
		** service... */
		VCHIQ_SERVICE_STATS_INC(service, ctrl_tx_count);
		VCHIQ_SERVICE_STATS_ADD(service, ctrl_tx_bytes, size);
	} else {
		vchiq_log_info(vchiq_sync_log_level,
			"%d: qms %s@%x,%x (%d->%d)", state->id,
			msg_type_str(VCHIQ_MSG_TYPE(msgid)),
			(unsigned int)header, size,
			VCHIQ_MSG_SRCPORT(msgid),
			VCHIQ_MSG_DSTPORT(msgid));
		if (size != 0) {
			WARN_ON(!((count == 1) && (size == elements[0].size)));
			memcpy(header->data, elements[0].data,
				elements[0].size);
		}
		VCHIQ_STATS_INC(state, ctrl_tx_count);
	}

	header->size = size;
	header->msgid = msgid;

	if (vchiq_sync_log_level >= VCHIQ_LOG_TRACE) {
		int svc_fourcc;

		svc_fourcc = service
			? service->base.fourcc
			: VCHIQ_MAKE_FOURCC('?', '?', '?', '?');

		vchiq_log_trace(vchiq_sync_log_level,
			"Sent Sync Msg %s(%u) to %c%c%c%c s:%u d:%d len:%d",
			msg_type_str(VCHIQ_MSG_TYPE(msgid)),
			VCHIQ_MSG_TYPE(msgid),
			VCHIQ_FOURCC_AS_4CHARS(svc_fourcc),
			VCHIQ_MSG_SRCPORT(msgid),
			VCHIQ_MSG_DSTPORT(msgid),
			size);
	}

	/* Make sure the new header is visible to the peer. */
	wmb();

	remote_event_signal(&state->remote->sync_trigger);

	if (VCHIQ_MSG_TYPE(msgid) != VCHIQ_MSG_PAUSE)
		mutex_unlock(&state->sync_mutex);

	return VCHIQ_SUCCESS;
}

static inline void
claim_slot(VCHIQ_SLOT_INFO_T *slot)
{
	slot->use_count++;
}

static void
release_slot(VCHIQ_STATE_T *state, VCHIQ_SLOT_INFO_T *slot_info)
{
	int release_count;
	mutex_lock(&state->recycle_mutex);

	release_count = slot_info->release_count;
	slot_info->release_count = ++release_count;

	if (release_count == slot_info->use_count) {
		int slot_queue_recycle;
		/* Add to the freed queue */

		/* A read barrier is necessary here to prevent speculative
		** fetches of remote->slot_queue_recycle from overtaking the
		** mutex. */
		rmb();

		slot_queue_recycle = state->remote->slot_queue_recycle;
		state->remote->slot_queue[slot_queue_recycle &
			VCHIQ_SLOT_QUEUE_MASK] =
			SLOT_INDEX_FROM_INFO(state, slot_info);
		state->remote->slot_queue_recycle = slot_queue_recycle + 1;
		vchiq_log_info(vchiq_core_log_level,
			"%d: release_slot %d - recycle->%x",
			state->id, SLOT_INDEX_FROM_INFO(state, slot_info),
			state->remote->slot_queue_recycle);

		/* A write barrier is necessary, but remote_event_signal
		** contains one. */
		remote_event_signal(&state->remote->recycle);
	}

	mutex_unlock(&state->recycle_mutex);
}

/* Called by the slot handler - don't hold the bulk mutex */
static VCHIQ_STATUS_T
notify_bulks(VCHIQ_SERVICE_T *service, VCHIQ_BULK_QUEUE_T *queue)
{
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;

	vchiq_log_trace(vchiq_core_log_level,
		"%d: nb:%d %cx - p=%x rn=%x r=%x",
		service->state->id, service->localport,
		(queue == &service->bulk_tx) ? 't' : 'r',
		queue->process, queue->remote_notify, queue->remove);

	if (service->state->is_master) {
		while (queue->remote_notify != queue->process) {
			VCHIQ_BULK_T *bulk =
				&queue->bulks[BULK_INDEX(queue->remote_notify)];
			int msgtype = (bulk->dir == VCHIQ_BULK_TRANSMIT) ?
				VCHIQ_MSG_BULK_RX_DONE : VCHIQ_MSG_BULK_TX_DONE;
			int msgid = VCHIQ_MAKE_MSG(msgtype, service->localport,
				service->remoteport);
			VCHIQ_ELEMENT_T element = { &bulk->actual, 4 };
			/* Only reply to non-dummy bulk requests */
			if (bulk->remote_data) {
				status = queue_message(service->state, NULL,
					msgid, &element, 1, 4, 0);
				if (status != VCHIQ_SUCCESS)
					break;
			}
			queue->remote_notify++;
		}
	} else {
		queue->remote_notify = queue->process;
	}

	if (status == VCHIQ_SUCCESS) {
		while (queue->remove != queue->remote_notify) {
			VCHIQ_BULK_T *bulk =
				&queue->bulks[BULK_INDEX(queue->remove)];

			/* Only generate callbacks for non-dummy bulk
			** requests, and non-terminated services */
			if (bulk->data && service->instance) {
				if (bulk->actual != VCHIQ_BULK_ACTUAL_ABORTED) {
					if (bulk->dir == VCHIQ_BULK_TRANSMIT) {
						VCHIQ_SERVICE_STATS_INC(service,
							bulk_tx_count);
						VCHIQ_SERVICE_STATS_ADD(service,
							bulk_tx_bytes,
							bulk->actual);
					} else {
						VCHIQ_SERVICE_STATS_INC(service,
							bulk_rx_count);
						VCHIQ_SERVICE_STATS_ADD(service,
							bulk_rx_bytes,
							bulk->actual);
					}
				} else {
					VCHIQ_SERVICE_STATS_INC(service,
						bulk_aborted_count);
				}
				if (bulk->mode == VCHIQ_BULK_MODE_BLOCKING) {
					struct bulk_waiter *waiter =
						(struct bulk_waiter *)bulk->
							userdata;
					if (waiter) {
						waiter->actual = bulk->actual;
						up(&waiter->event);
					}
				} else if (bulk->mode ==
					VCHIQ_BULK_MODE_CALLBACK) {
					VCHIQ_REASON_T reason = (bulk->dir ==
						VCHIQ_BULK_TRANSMIT) ?
						((bulk->actual ==
						VCHIQ_BULK_ACTUAL_ABORTED) ?
						VCHIQ_BULK_TRANSMIT_ABORTED :
						VCHIQ_BULK_TRANSMIT_DONE) :
						((bulk->actual ==
						VCHIQ_BULK_ACTUAL_ABORTED) ?
						VCHIQ_BULK_RECEIVE_ABORTED :
						VCHIQ_BULK_RECEIVE_DONE);
					status = make_service_callback(service,
						reason,	NULL, bulk->userdata);
					if (status == VCHIQ_RETRY)
						break;
				}
			}

			queue->remove++;
			up(&service->bulk_remove_event);
		}
	}

	if (status != VCHIQ_SUCCESS)
		request_poll(service->state, service,
			(queue == &service->bulk_tx) ?
			VCHIQ_POLL_TXNOTIFY : VCHIQ_POLL_RXNOTIFY);

	return status;
}

/* Called by the slot handler thread */
static void
poll_services(VCHIQ_STATE_T *state)
{
	int group, i;

	for (group = 0; group < BITSET_SIZE(state->unused_service); group++) {
		uint32_t flags;
		flags = atomic_xchg(&state->poll_services[group], 0);
		for (i = 0; flags; i++) {
			if (flags & (1 << i)) {
				VCHIQ_SERVICE_T *service =
					state->services[(group<<5) + i];
				uint32_t service_flags =
					atomic_xchg(&service->poll_flags, 0);
				if (service_flags &
					(1 << VCHIQ_POLL_TERMINATE)) {
					vchiq_log_info(vchiq_core_log_level,
						"%d: ps - terminate %d<->%d",
						state->id, service->localport,
						service->remoteport);
					if (vchiq_close_service_internal(
						service, 0/*!close_recvd*/) !=
						VCHIQ_SUCCESS)
						request_poll(state, service,
							VCHIQ_POLL_TERMINATE);
				}
				if (service_flags & (1 << VCHIQ_POLL_TXNOTIFY))
					notify_bulks(service,
						&service->bulk_tx);
				if (service_flags & (1 << VCHIQ_POLL_RXNOTIFY))
					notify_bulks(service,
						&service->bulk_rx);
				flags &= ~(1 << i);
			}
		}
	}
}

/* Called by the slot handler or application threads, holding the bulk mutex. */
static int
resolve_bulks(VCHIQ_SERVICE_T *service, VCHIQ_BULK_QUEUE_T *queue)
{
	VCHIQ_STATE_T *state = service->state;
	int resolved = 0;

	while ((queue->process != queue->local_insert) &&
		(queue->process != queue->remote_insert)) {
		VCHIQ_BULK_T *bulk = &queue->bulks[BULK_INDEX(queue->process)];

		vchiq_log_trace(vchiq_core_log_level,
			"%d: rb:%d %cx - li=%x ri=%x p=%x",
			state->id, service->localport,
			(queue == &service->bulk_tx) ? 't' : 'r',
			queue->local_insert, queue->remote_insert,
			queue->process);

		WARN_ON(!((int)(queue->local_insert - queue->process) > 0));
		WARN_ON(!((int)(queue->remote_insert - queue->process) > 0));

		mutex_lock(&state->bulk_transfer_mutex);
		vchiq_transfer_bulk(bulk);
		mutex_unlock(&state->bulk_transfer_mutex);

		if (vchiq_core_msg_log_level >= VCHIQ_LOG_INFO) {
			const char *header = (queue == &service->bulk_tx) ?
				"Send Bulk to" : "Recv Bulk from";
			if (bulk->actual != VCHIQ_BULK_ACTUAL_ABORTED)
				vchiq_log_info(vchiq_core_msg_log_level,
					"%s %c%c%c%c d:%d len:%d %x<->%x",
					header,
					VCHIQ_FOURCC_AS_4CHARS(
						service->base.fourcc),
					service->remoteport,
					bulk->size,
					(unsigned int)bulk->data,
					(unsigned int)bulk->remote_data);
			else
				vchiq_log_info(vchiq_core_msg_log_level,
					"%s %c%c%c%c d:%d ABORTED - tx len:%d,"
					" rx len:%d %x<->%x",
					header,
					VCHIQ_FOURCC_AS_4CHARS(
						service->base.fourcc),
					service->remoteport,
					bulk->size,
					bulk->remote_size,
					(unsigned int)bulk->data,
					(unsigned int)bulk->remote_data);
		}

		vchiq_complete_bulk(bulk);
		queue->process++;
		resolved++;
	}
	return resolved;
}

/* Called with the bulk_mutex held */
static void
abort_outstanding_bulks(VCHIQ_SERVICE_T *service, VCHIQ_BULK_QUEUE_T *queue)
{
	int is_tx = (queue == &service->bulk_tx);
	vchiq_log_trace(vchiq_core_log_level,
		"%d: aob:%d %cx - li=%x ri=%x p=%x",
		service->state->id, service->localport, is_tx ? 't' : 'r',
		queue->local_insert, queue->remote_insert, queue->process);

	WARN_ON(!((int)(queue->local_insert - queue->process) >= 0));
	WARN_ON(!((int)(queue->remote_insert - queue->process) >= 0));

	while ((queue->process != queue->local_insert) ||
		(queue->process != queue->remote_insert)) {
		VCHIQ_BULK_T *bulk = &queue->bulks[BULK_INDEX(queue->process)];

		if (queue->process == queue->remote_insert) {
			/* fabricate a matching dummy bulk */
			bulk->remote_data = NULL;
			bulk->remote_size = 0;
			queue->remote_insert++;
		}

		if (queue->process != queue->local_insert) {
			vchiq_complete_bulk(bulk);

			vchiq_log_info(vchiq_core_msg_log_level,
				"%s %c%c%c%c d:%d ABORTED - tx len:%d, "
				"rx len:%d",
				is_tx ? "Send Bulk to" : "Recv Bulk from",
				VCHIQ_FOURCC_AS_4CHARS(service->base.fourcc),
				service->remoteport,
				bulk->size,
				bulk->remote_size);
		} else {
			/* fabricate a matching dummy bulk */
			bulk->data = NULL;
			bulk->size = 0;
			bulk->actual = VCHIQ_BULK_ACTUAL_ABORTED;
			bulk->dir = is_tx ? VCHIQ_BULK_TRANSMIT :
				VCHIQ_BULK_RECEIVE;
			queue->local_insert++;
		}

		queue->process++;
	}
}

/* Called from the slot handler thread */
static void
pause_bulks(VCHIQ_STATE_T *state)
{
	if (unlikely(atomic_inc_return(&pause_bulks_count) != 1)) {
		WARN_ON_ONCE(1);
		atomic_set(&pause_bulks_count, 1);
		return;
	}

	/* Block bulk transfers from all services */
	mutex_lock(&state->bulk_transfer_mutex);
}

/* Called from the slot handler thread */
static void
resume_bulks(VCHIQ_STATE_T *state)
{
	if (unlikely(atomic_dec_return(&pause_bulks_count) != 0)) {
		WARN_ON_ONCE(1);
		atomic_set(&pause_bulks_count, 0);
		return;
	}

	/* Allow bulk transfers from all services */
	mutex_unlock(&state->bulk_transfer_mutex);
}

static int
parse_open(VCHIQ_STATE_T *state, VCHIQ_HEADER_T *header)
{
	VCHIQ_SERVICE_T *service = NULL;
	int msgid, size;
	int type;
	unsigned int localport, remoteport;

	msgid = header->msgid;
	size = header->size;
	type = VCHIQ_MSG_TYPE(msgid);
	localport = VCHIQ_MSG_DSTPORT(msgid);
	remoteport = VCHIQ_MSG_SRCPORT(msgid);
	if (size == sizeof(struct vchiq_open_payload)) {
		const struct vchiq_open_payload *payload =
			(struct vchiq_open_payload *)header->data;
		unsigned int fourcc;

		fourcc = payload->fourcc;
		vchiq_log_info(vchiq_core_log_level,
			"%d: prs OPEN@%x (%d->'%c%c%c%c')",
			state->id, (unsigned int)header,
			localport,
			VCHIQ_FOURCC_AS_4CHARS(fourcc));

		service = get_listening_service(state, fourcc);

		if (service) {
			/* A matching service exists */
			short version = payload->version;
			short version_min = payload->version_min;
			if ((service->version < version_min) ||
				(version < service->version_min)) {
				/* Version mismatch */
				vchiq_loud_error_header();
				vchiq_loud_error("%d: service %d (%c%c%c%c) "
					"version mismatch - local (%d, min %d)"
					" vs. remote (%d, min %d)",
					state->id, service->localport,
					VCHIQ_FOURCC_AS_4CHARS(fourcc),
					service->version, service->version_min,
					version, version_min);
				vchiq_loud_error_footer();
				goto fail_open;
			}
			if (service->srvstate == VCHIQ_SRVSTATE_LISTENING) {
				/* Acknowledge the OPEN */
				if (service->sync) {
					if (queue_message_sync(state, NULL,
						VCHIQ_MAKE_MSG(
							VCHIQ_MSG_OPENACK,
							service->localport,
							remoteport),
						NULL, 0, 0, 0) == VCHIQ_RETRY)
						/* Bail out if not ready */
						return 0;
				} else {
					if (queue_message(state, NULL,
						VCHIQ_MAKE_MSG(
							VCHIQ_MSG_OPENACK,
							service->localport,
							remoteport),
						NULL, 0, 0, 0) == VCHIQ_RETRY)
						/* Bail out if not ready */
						return 0;
				}

				/* The service is now open */
				vchiq_set_service_state(service,
					service->sync ? VCHIQ_SRVSTATE_OPENSYNC
					: VCHIQ_SRVSTATE_OPEN);
			}

			service->remoteport = remoteport;
			service->client_id = ((int *)header->data)[1];
			if (make_service_callback(service, VCHIQ_SERVICE_OPENED,
				NULL, NULL) == VCHIQ_RETRY) {
				/* Bail out if not ready */
				service->remoteport = VCHIQ_PORT_FREE;
				return 0;
			}

			/* Success - the message has been dealt with */
			return 1;
		}
	}

fail_open:
	/* No available service, or an invalid request - send a CLOSE */
	if (queue_message(state, NULL,
		VCHIQ_MAKE_MSG(VCHIQ_MSG_CLOSE, 0, VCHIQ_MSG_SRCPORT(msgid)),
		NULL, 0, 0, 0) == VCHIQ_RETRY)
		return 0;  /* Bail out if not ready */

	return 1;
}

/* Called by the slot handler thread */
static void
parse_rx_slots(VCHIQ_STATE_T *state)
{
	VCHIQ_SHARED_STATE_T *remote = state->remote;
	int tx_pos;
	char *ping_response = "ping success";
	int error_code_size = 4;
	DEBUG_INITIALISE(state->local)

	tx_pos = remote->tx_pos;

	while (state->rx_pos != tx_pos) {
		VCHIQ_SERVICE_T *service = NULL;
		VCHIQ_HEADER_T *header;
		int msgid, size;
		int type;
		unsigned int localport, remoteport;

		DEBUG_TRACE(PARSE_LINE);
		if (!state->rx_data) {
			int rx_index;
			WARN_ON(!((state->rx_pos & VCHIQ_SLOT_MASK) == 0));
			rx_index = remote->slot_queue[
				SLOT_QUEUE_INDEX_FROM_POS(state->rx_pos) &
				VCHIQ_SLOT_QUEUE_MASK];
			state->rx_data = (char *)SLOT_DATA_FROM_INDEX(state,
				rx_index);
			state->rx_info = SLOT_INFO_FROM_INDEX(state, rx_index);

			/* Initialise use_count to one, and increment
			** release_count at the end of the slot to avoid
			** releasing the slot prematurely. */
			state->rx_info->use_count = 1;
			state->rx_info->release_count = 0;
		}

		header = (VCHIQ_HEADER_T *)(state->rx_data +
			(state->rx_pos & VCHIQ_SLOT_MASK));
		DEBUG_VALUE(PARSE_HEADER, (int)header);
		msgid = header->msgid;
		DEBUG_VALUE(PARSE_MSGID, msgid);
		size = header->size;
		type = VCHIQ_MSG_TYPE(msgid);
		localport = VCHIQ_MSG_DSTPORT(msgid);
		remoteport = VCHIQ_MSG_SRCPORT(msgid);

		if (type != VCHIQ_MSG_DATA)
			VCHIQ_STATS_INC(state, ctrl_rx_count);

		switch (type) {
		case VCHIQ_MSG_OPENACK:
		case VCHIQ_MSG_CLOSE:
		case VCHIQ_MSG_DATA:
		case VCHIQ_MSG_BULK_RX:
		case VCHIQ_MSG_BULK_TX:
		case VCHIQ_MSG_BULK_RX_DONE:
		case VCHIQ_MSG_BULK_TX_DONE:
			if (localport <= VCHIQ_PORT_MAX) {
				service = state->services[localport];
				if (service && (service->srvstate ==
					VCHIQ_SRVSTATE_FREE))
					service = NULL;
			}
			if (!service) {
				vchiq_log_error(vchiq_core_log_level,
					"%d: prs %s@%x (%d->%d) - "
					"invalid/closed service %d",
					state->id, msg_type_str(type),
					(unsigned int)header,
					remoteport, localport, localport);
				goto skip_message;
			}
			if (size == strlen(ping_response) + error_code_size + 1 && type == VCHIQ_MSG_DATA) {
				//first 4-byte is error code
				if (strncmp((char*)header->data+error_code_size, ping_response, strlen(ping_response)) == 0)
					complete(&state->ping_rcvd);
			}
		default:
			break;
		}

		if (vchiq_core_msg_log_level >= VCHIQ_LOG_INFO) {
			int svc_fourcc;

			svc_fourcc = service
				? service->base.fourcc
				: VCHIQ_MAKE_FOURCC('?', '?', '?', '?');
			vchiq_log_info(vchiq_core_msg_log_level,
				"Rcvd Msg %s(%u) from %c%c%c%c s:%d d:%d len:%d",
				msg_type_str(type), type,
				VCHIQ_FOURCC_AS_4CHARS(svc_fourcc),
				remoteport, localport, size);
			if (size > 0)
				vchiq_log_dump_mem("Rcvd", 0, header->data,
					min(64, size));
		}

		if (((unsigned int)header & VCHIQ_SLOT_MASK) + calc_stride(size)
			> VCHIQ_SLOT_SIZE) {
			vchiq_log_error(vchiq_core_log_level,
				"header %x (msgid %x) - size %x too big for "
				"slot",
				(unsigned int)header, (unsigned int)msgid,
				(unsigned int)size);
			WARN(1, "oversized for slot\n");
		}

		switch (type) {
		case VCHIQ_MSG_OPEN:
			WARN_ON(!(VCHIQ_MSG_DSTPORT(msgid) == 0));
			if (!parse_open(state, header))
				return;
			break;
		case VCHIQ_MSG_OPENACK:
			vchiq_log_info(vchiq_core_log_level,
				"%d: prs OPENACK@%x (%d->%d)",
				state->id, (unsigned int)header,
				remoteport, localport);
			if (service->srvstate ==
				VCHIQ_SRVSTATE_OPENING) {
				service->remoteport = remoteport;
				vchiq_set_service_state(service,
					VCHIQ_SRVSTATE_OPEN);
				up(&service->remove_event);
			}
			break;
		case VCHIQ_MSG_CLOSE:
			WARN_ON(size != 0); /* There should be no data */

			vchiq_log_info(vchiq_core_log_level,
				"%d: prs CLOSE@%x (%d->%d)",
				state->id, (unsigned int)header,
				remoteport, localport);

			if ((service->remoteport != remoteport) &&
				VCHIQ_PORT_IS_VALID(service->remoteport)) {
				/* This could be from a client which hadn't yet
				** received the OPENACK - look for the
				** connected service */
				service = get_connected_service(state,
					remoteport);
				if (!service)
					break;
			}

			if (vchiq_close_service_internal(service,
				1/*close_recvd*/) == VCHIQ_RETRY)
				return;  /* Bail out if not ready */

			vchiq_log_info(vchiq_core_log_level,
				"Close Service %c%c%c%c s:%u d:%d",
				VCHIQ_FOURCC_AS_4CHARS(service->base.fourcc),
				service->localport,
				service->remoteport);
			break;
		case VCHIQ_MSG_DATA:
			vchiq_log_trace(vchiq_core_log_level,
				"%d: prs DATA@%x,%x (%d->%d)",
				state->id, (unsigned int)header, size,
				remoteport, localport);

			if ((service->remoteport == remoteport)
				&& (service->srvstate ==
				VCHIQ_SRVSTATE_OPEN)) {
				header->msgid = msgid | VCHIQ_MSGID_CLAIMED;
				claim_slot(state->rx_info);
				DEBUG_TRACE(PARSE_LINE);
				if (make_service_callback(service,
					VCHIQ_MESSAGE_AVAILABLE, header,
					NULL) == VCHIQ_RETRY) {
					DEBUG_TRACE(PARSE_LINE);
					return;  /* Bail out if not ready */
				}
				VCHIQ_SERVICE_STATS_INC(service, ctrl_rx_count);
				VCHIQ_SERVICE_STATS_ADD(service, ctrl_rx_bytes,
					size);
			} else {
				VCHIQ_STATS_INC(state, error_count);
			}
			break;
		case VCHIQ_MSG_CONNECT:
			vchiq_log_info(vchiq_core_log_level,
				"%d: prs CONNECT@%x",
				state->id, (unsigned int)header);
			up(&state->connect);
			break;
		case VCHIQ_MSG_BULK_RX:
		case VCHIQ_MSG_BULK_TX: {
			VCHIQ_BULK_QUEUE_T *queue;
			WARN_ON(!state->is_master);
			queue = (type == VCHIQ_MSG_BULK_RX) ?
				&service->bulk_tx : &service->bulk_rx;
			if ((service->remoteport == remoteport)
				&& (service->srvstate ==
				VCHIQ_SRVSTATE_OPEN)) {
				VCHIQ_BULK_T *bulk;
				int resolved;

				WARN_ON(!(queue->remote_insert < queue->remove +
					VCHIQ_NUM_SERVICE_BULKS));
				bulk = &queue->bulks[
					BULK_INDEX(queue->remote_insert)];
				bulk->remote_data =
					(void *)((int *)header->data)[0];
				bulk->remote_size = ((int *)header->data)[1];
				wmb();

				vchiq_log_info(vchiq_core_log_level,
					"%d: prs %s@%x (%d->%d) %x@%x",
					state->id, msg_type_str(type),
					(unsigned int)header,
					remoteport, localport,
					bulk->remote_size,
					(unsigned int)bulk->remote_data);

				queue->remote_insert++;

				if (state->conn_state !=
					VCHIQ_CONNSTATE_CONNECTED)
					break;

				DEBUG_TRACE(PARSE_LINE);
				if (mutex_lock_interruptible(
					&service->bulk_mutex) != 0) {
					DEBUG_TRACE(PARSE_LINE);
					return;
				}
				DEBUG_TRACE(PARSE_LINE);
				resolved = resolve_bulks(service, queue);
				mutex_unlock(&service->bulk_mutex);
				if (resolved)
					notify_bulks(service, queue);
			}
		} break;
		case VCHIQ_MSG_BULK_RX_DONE:
		case VCHIQ_MSG_BULK_TX_DONE:
			WARN_ON(state->is_master);
			if ((service->remoteport == remoteport)
				&& (service->srvstate !=
				VCHIQ_SRVSTATE_FREE)) {
				VCHIQ_BULK_QUEUE_T *queue;
				VCHIQ_BULK_T *bulk;

				queue = (type == VCHIQ_MSG_BULK_RX_DONE) ?
					&service->bulk_rx : &service->bulk_tx;

				bulk = &queue->bulks[
					BULK_INDEX(queue->process)];
				bulk->actual = *(int *)header->data;

				vchiq_log_info(vchiq_core_log_level,
					"%d: prs %s@%x (%d->%d) %x@%x",
					state->id, msg_type_str(type),
					(unsigned int)header,
					remoteport, localport,
					bulk->actual, (unsigned int)bulk->data);

				vchiq_log_trace(vchiq_core_log_level,
					"%d: prs:%d %cx li=%x ri=%x p=%x",
					state->id, localport,
					(type == VCHIQ_MSG_BULK_RX_DONE) ?
						'r' : 't',
					queue->local_insert,
					queue->remote_insert, queue->process);

				DEBUG_TRACE(PARSE_LINE);
				if (mutex_lock_interruptible(
					&service->bulk_mutex) != 0) {
					DEBUG_TRACE(PARSE_LINE);
					return;
				}
				DEBUG_TRACE(PARSE_LINE);
				WARN_ON(queue->process == queue->local_insert);
				vchiq_complete_bulk(bulk);
				queue->process++;
				mutex_unlock(&service->bulk_mutex);
				DEBUG_TRACE(PARSE_LINE);
				notify_bulks(service, queue);
				DEBUG_TRACE(PARSE_LINE);
			}
			break;
		case VCHIQ_MSG_PADDING:
			vchiq_log_trace(vchiq_core_log_level,
				"%d: prs PADDING@%x,%x",
				state->id, (unsigned int)header, size);
			break;
		case VCHIQ_MSG_PAUSE:
			/* If initiated, signal the application thread */
			vchiq_log_trace(vchiq_core_log_level,
				"%d: prs PAUSE@%x,%x",
				state->id, (unsigned int)header, size);
			if (state->conn_state == VCHIQ_CONNSTATE_PAUSED) {
				vchiq_log_error(vchiq_core_log_level,
					"%d: PAUSE received in state PAUSED",
					state->id);
				break;
			}
			if (state->conn_state != VCHIQ_CONNSTATE_PAUSE_SENT) {
				/* Send a PAUSE in response */
				if (queue_message(state, NULL,
					VCHIQ_MAKE_MSG(VCHIQ_MSG_PAUSE, 0, 0),
					NULL, 0, 0, 0) == VCHIQ_RETRY)
					return;  /* Bail out if not ready */
				if (state->is_master)
					pause_bulks(state);
			}
			/* At this point slot_mutex is held */
			vchiq_set_conn_state(state, VCHIQ_CONNSTATE_PAUSED);
			vchiq_platform_paused(state);
			break;
		case VCHIQ_MSG_RESUME:
			vchiq_log_trace(vchiq_core_log_level,
				"%d: prs RESUME@%x,%x",
				state->id, (unsigned int)header, size);
			/* Release the slot mutex */
			mutex_unlock(&state->slot_mutex);
			if (state->is_master)
				resume_bulks(state);
			vchiq_set_conn_state(state, VCHIQ_CONNSTATE_CONNECTED);
			vchiq_platform_resumed(state);
			break;

		case VCHIQ_MSG_REMOTE_USE:
			vchiq_on_remote_use(state);
			break;
		case VCHIQ_MSG_REMOTE_RELEASE:
			vchiq_on_remote_release(state);
			break;
		case VCHIQ_MSG_REMOTE_USE_ACTIVE:
			vchiq_on_remote_use_active(state);
			break;

		default:
			vchiq_log_error(vchiq_core_log_level,
				"%d: prs invalid msgid %x@%x,%x",
				state->id, msgid, (unsigned int)header, size);
			WARN(1, "invalid message\n");
			break;
		}

skip_message:
		state->rx_pos += calc_stride(size);

		DEBUG_TRACE(PARSE_LINE);
		/* Perform some housekeeping when the end of the slot is
		** reached. */
		if ((state->rx_pos & VCHIQ_SLOT_MASK) == 0) {
			/* Remove the extra reference count. */
			release_slot(state, state->rx_info);
			state->rx_data = NULL;
		}
	}
}

/* Called by the slot handler thread */
static int
slot_handler_func(void *v)
{
	VCHIQ_STATE_T *state = (VCHIQ_STATE_T *) v;
	VCHIQ_SHARED_STATE_T *local = state->local;
	DEBUG_INITIALISE(local)

	while (1) {
		DEBUG_COUNT(SLOT_HANDLER_COUNT);
		DEBUG_TRACE(SLOT_HANDLER_LINE);
		remote_event_wait(&local->trigger);

		rmb();

		DEBUG_TRACE(SLOT_HANDLER_LINE);
		if (state->poll_needed) {
			/* Check if we need to suspend - may change our
			 * conn_state */
			vchiq_platform_check_suspend(state);

			state->poll_needed = 0;

			/* Handle service polling and other rare conditions here
			** out of the mainline code */
			switch (state->conn_state) {
			case VCHIQ_CONNSTATE_CONNECTED:
				/* Poll the services as requested */
				poll_services(state);
				break;

			case VCHIQ_CONNSTATE_PAUSING:
				if (state->is_master)
					pause_bulks(state);
				if (queue_message(state, NULL,
					VCHIQ_MAKE_MSG(VCHIQ_MSG_PAUSE, 0, 0),
					NULL, 0, 0, 0) != VCHIQ_RETRY) {
					vchiq_set_conn_state(state,
						VCHIQ_CONNSTATE_PAUSE_SENT);
				} else {
					if (state->is_master)
						resume_bulks(state);
					/* Retry later */
					state->poll_needed = 1;
				}
				break;

			case VCHIQ_CONNSTATE_PAUSED:
				vchiq_platform_resume(state);
				break;

			case VCHIQ_CONNSTATE_RESUMING:
				if (queue_message(state, NULL,
					VCHIQ_MAKE_MSG(VCHIQ_MSG_RESUME, 0, 0),
					NULL, 0, 0, 0) != VCHIQ_RETRY) {
					if (state->is_master)
						resume_bulks(state);
					vchiq_set_conn_state(state,
						VCHIQ_CONNSTATE_CONNECTED);
					vchiq_platform_resumed(state);
				} else {
					/* This should really be impossible,
					** since the PAUSE should have flushed
					** through outstanding messages. */
					vchiq_log_error(vchiq_core_log_level,
						"Failed to send RESUME "
						"message");
					BUG();
				}
				break;
			default:
				break;
			}


		}

		DEBUG_TRACE(SLOT_HANDLER_LINE);
		parse_rx_slots(state);
	}
	return 0;
}

/* Called by the ping thread */
static int ping_func(void *v)
{
	VCHIQ_STATE_T *state = (VCHIQ_STATE_T *) v;
	unsigned n_timeout = 0;

	while (1) {
		if (wait_for_completion_interruptible(&state->ping_send) != 0)
			continue;

		if (wait_for_completion_interruptible_timeout(&state->ping_rcvd,
				msecs_to_jiffies(5000)) <= 0)
			printk(KERN_ERR "videocore ping timeout #%u\n", ++n_timeout);
	}
	return 0;
}

/* Called by the recycle thread */
static int
recycle_func(void *v)
{
	VCHIQ_STATE_T *state = (VCHIQ_STATE_T *) v;
	VCHIQ_SHARED_STATE_T *local = state->local;

	while (1) {
		remote_event_wait(&local->recycle);

		if (mutex_lock_interruptible(&state->slot_mutex) == 0) {
			process_free_queue(state);

			mutex_unlock(&state->slot_mutex);
		}
	}
	return 0;
}


/* Called by the sync thread */
static int
sync_func(void *v)
{
	VCHIQ_STATE_T *state = (VCHIQ_STATE_T *) v;
	VCHIQ_SHARED_STATE_T *local = state->local;
	VCHIQ_HEADER_T *header = (VCHIQ_HEADER_T *)SLOT_DATA_FROM_INDEX(state,
		state->remote->slot_sync);

	while (1) {
		VCHIQ_SERVICE_T *service;
		int msgid, size;
		int type;
		unsigned int localport, remoteport;

		remote_event_wait(&local->sync_trigger);

		rmb();

		msgid = header->msgid;
		size = header->size;
		type = VCHIQ_MSG_TYPE(msgid);
		localport = VCHIQ_MSG_DSTPORT(msgid);
		remoteport = VCHIQ_MSG_SRCPORT(msgid);

		if (localport <= VCHIQ_PORT_MAX) {
			service = state->services[localport];
			if (service &&
				(service->srvstate == VCHIQ_SRVSTATE_FREE))
				service = NULL;
		}

		if (!service) {
			vchiq_log_error(vchiq_sync_log_level,
				"%d: sf %s@%x (%d->%d) - invalid/closed service %d",
				state->id, msg_type_str(type),
				(unsigned int)header,
				remoteport, localport, localport);
			release_message_sync(state, header);
			continue;
		}

		if (vchiq_sync_log_level >= VCHIQ_LOG_TRACE) {
			int svc_fourcc;

			svc_fourcc = service
				? service->base.fourcc
				: VCHIQ_MAKE_FOURCC('?', '?', '?', '?');
			vchiq_log_trace(vchiq_sync_log_level,
				"Rcvd Msg %s from %c%c%c%c s:%d d:%d len:%d",
				msg_type_str(type),
				VCHIQ_FOURCC_AS_4CHARS(svc_fourcc),
				remoteport, localport, size);
			if (size > 0)
				vchiq_log_dump_mem("Rcvd", 0, header->data,
					min(64, size));
		}

		switch (type) {
		case VCHIQ_MSG_OPENACK:
			vchiq_log_info(vchiq_sync_log_level,
				"%d: sf OPENACK@%x (%d->%d)",
				state->id, (unsigned int)header,
				remoteport, localport);
			if (service->srvstate == VCHIQ_SRVSTATE_OPENING) {
				service->remoteport = remoteport;
				vchiq_set_service_state(service,
					VCHIQ_SRVSTATE_OPENSYNC);
				up(&service->remove_event);
			}
			release_message_sync(state, header);
			break;

		case VCHIQ_MSG_DATA:
			vchiq_log_trace(vchiq_sync_log_level,
				"%d: sf DATA@%x,%x (%d->%d)",
				state->id, (unsigned int)header, size,
				remoteport, localport);

			if ((service->remoteport == remoteport) &&
				(service->srvstate ==
				VCHIQ_SRVSTATE_OPENSYNC)) {
				if (make_service_callback(service,
					VCHIQ_MESSAGE_AVAILABLE, header,
					NULL) == VCHIQ_RETRY)
					vchiq_log_error(vchiq_sync_log_level,
						"synchronous callback to service %d returns VCHIQ_RETRY",
						localport);
			}
			break;

		default:
			vchiq_log_error(vchiq_sync_log_level,
				"%d: sf unexpected msgid %x@%x,%x",
				state->id, msgid, (unsigned int)header, size);
			release_message_sync(state, header);
			break;
		}
	}

	return 0;
}


static void
init_bulk_queue(VCHIQ_BULK_QUEUE_T *queue)
{
	queue->local_insert = 0;
	queue->remote_insert = 0;
	queue->process = 0;
	queue->remote_notify = 0;
	queue->remove = 0;
}


inline const char *
get_conn_state_name(VCHIQ_CONNSTATE_T conn_state)
{
	return conn_state_names[conn_state];
}


VCHIQ_SLOT_ZERO_T *
vchiq_init_slots(void *mem_base, int mem_size)
{
	int mem_align = (VCHIQ_SLOT_SIZE - (int)mem_base) & VCHIQ_SLOT_MASK;
	VCHIQ_SLOT_ZERO_T *slot_zero =
		(VCHIQ_SLOT_ZERO_T *)((char *)mem_base + mem_align);
	int num_slots = (mem_size - mem_align)/VCHIQ_SLOT_SIZE;
	int first_data_slot = VCHIQ_SLOT_ZERO_SLOTS;

	/* Ensure there is enough memory to run an absolutely minimum system */
	num_slots -= first_data_slot;

	if (num_slots < 4) {
		vchiq_log_error(vchiq_core_log_level,
			"vchiq_init_slots - insufficient memory %x bytes",
			mem_size);
		return NULL;
	}

	memset(slot_zero, 0, sizeof(VCHIQ_SLOT_ZERO_T));

	slot_zero->magic = VCHIQ_MAGIC;
	slot_zero->version = VCHIQ_VERSION;
	slot_zero->version_min = VCHIQ_VERSION_MIN;
	slot_zero->slot_zero_size = sizeof(VCHIQ_SLOT_ZERO_T);
	slot_zero->slot_size = VCHIQ_SLOT_SIZE;
	slot_zero->max_slots = VCHIQ_MAX_SLOTS;
	slot_zero->max_slots_per_side = VCHIQ_MAX_SLOTS_PER_SIDE;

	slot_zero->master.slot_sync = first_data_slot;
	slot_zero->master.slot_first = first_data_slot + 1;
	slot_zero->master.slot_last = first_data_slot + (num_slots/2) - 1;
	slot_zero->slave.slot_sync = first_data_slot + (num_slots/2);
	slot_zero->slave.slot_first = first_data_slot + (num_slots/2) + 1;
	slot_zero->slave.slot_last = first_data_slot + num_slots - 1;

	return slot_zero;
}

VCHIQ_STATUS_T
vchiq_init_state(VCHIQ_STATE_T *state, VCHIQ_SLOT_ZERO_T *slot_zero,
		 int is_master)
{
	VCHIQ_SHARED_STATE_T *local;
	VCHIQ_SHARED_STATE_T *remote;
	VCHIQ_STATUS_T status;
	char threadname[10];
	static int id;
	int i;

	vchiq_log_warning(vchiq_core_log_level,
		"%s: slot_zero = 0x%08lx, is_master = %d",
		__func__, (unsigned long)slot_zero, is_master);

	/* Check the input configuration */

	if (slot_zero->magic != VCHIQ_MAGIC) {
		vchiq_loud_error_header();
		vchiq_loud_error("Invalid VCHIQ magic value found.");
		vchiq_loud_error("slot_zero=%x: magic=%x (expected %x)",
			(unsigned int)slot_zero, slot_zero->magic, VCHIQ_MAGIC);
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}

	if (slot_zero->version < VCHIQ_VERSION_MIN) {
		vchiq_loud_error_header();
		vchiq_loud_error("Incompatible VCHIQ versions found.");
		vchiq_loud_error("slot_zero=%x: VideoCore version=%d "
			"(minimum %d)",
			(unsigned int)slot_zero, slot_zero->version,
			VCHIQ_VERSION_MIN);
		vchiq_loud_error("Restart with a newer VideoCore image.");
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}

	if (VCHIQ_VERSION < slot_zero->version_min) {
		vchiq_loud_error_header();
		vchiq_loud_error("Incompatible VCHIQ versions found.");
		vchiq_loud_error("slot_zero=%x: version=%d (VideoCore "
			"minimum %d)",
			(unsigned int)slot_zero, VCHIQ_VERSION,
			slot_zero->version_min);
		vchiq_loud_error("Restart with a newer kernel.");
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}

	if ((slot_zero->slot_zero_size != sizeof(VCHIQ_SLOT_ZERO_T)) ||
		 (slot_zero->slot_size != VCHIQ_SLOT_SIZE) ||
		 (slot_zero->max_slots != VCHIQ_MAX_SLOTS) ||
		 (slot_zero->max_slots_per_side != VCHIQ_MAX_SLOTS_PER_SIDE)) {
		vchiq_loud_error_header();
		if (slot_zero->slot_zero_size != sizeof(VCHIQ_SLOT_ZERO_T))
			vchiq_loud_error("slot_zero=%x: slot_zero_size=%x "
				"(expected %x)",
				(unsigned int)slot_zero,
				slot_zero->slot_zero_size,
				sizeof(VCHIQ_SLOT_ZERO_T));
		if (slot_zero->slot_size != VCHIQ_SLOT_SIZE)
			vchiq_loud_error("slot_zero=%x: slot_size=%d "
				"(expected %d",
				(unsigned int)slot_zero, slot_zero->slot_size,
				VCHIQ_SLOT_SIZE);
		if (slot_zero->max_slots != VCHIQ_MAX_SLOTS)
			vchiq_loud_error("slot_zero=%x: max_slots=%d "
				"(expected %d)",
				(unsigned int)slot_zero, slot_zero->max_slots,
				VCHIQ_MAX_SLOTS);
		if (slot_zero->max_slots_per_side != VCHIQ_MAX_SLOTS_PER_SIDE)
			vchiq_loud_error("slot_zero=%x: max_slots_per_side=%d "
				"(expected %d)",
				(unsigned int)slot_zero,
				slot_zero->max_slots_per_side,
				VCHIQ_MAX_SLOTS_PER_SIDE);
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}

	if (is_master) {
		local = &slot_zero->master;
		remote = &slot_zero->slave;
	} else {
		local = &slot_zero->slave;
		remote = &slot_zero->master;
	}

	if (local->initialised) {
		vchiq_loud_error_header();
		if (remote->initialised)
			vchiq_loud_error("local state has already been "
				"initialised");
		else
			vchiq_loud_error("master/slave mismatch - two %ss",
				is_master ? "master" : "slave");
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}

	memset(state, 0, sizeof(VCHIQ_STATE_T));

	state->id = id++;
	state->is_master = is_master;

	/*
		initialize shared state pointers
	 */

	state->local = local;
	state->remote = remote;
	state->slot_data = (VCHIQ_SLOT_T *)slot_zero;

	/*
		initialize events and mutexes
	 */

	sema_init(&state->connect, 0);
	mutex_init(&state->mutex);
	sema_init(&state->trigger_event, 0);
	sema_init(&state->recycle_event, 0);
	sema_init(&state->sync_trigger_event, 0);
	sema_init(&state->sync_release_event, 0);

	mutex_init(&state->slot_mutex);
	mutex_init(&state->recycle_mutex);
	mutex_init(&state->sync_mutex);
	mutex_init(&state->bulk_transfer_mutex);

	sema_init(&state->slot_available_event, 0);
	sema_init(&state->slot_remove_event, 0);

	init_completion(&state->ping_send);
	init_completion(&state->ping_rcvd);

	state->slot_queue_available = 0;

	for (i = 0; i < VCHIQ_MAX_SERVICES; i++) {
		VCHIQ_SERVICE_QUOTA_T *service_quota =
			&state->service_quotas[i];
		sema_init(&service_quota->quota_event, 0);
	}

	for (i = local->slot_first; i <= local->slot_last; i++)
		local->slot_queue[state->slot_queue_available++] = i;

	state->default_slot_quota = state->slot_queue_available/2;
	state->default_message_quota =
		min((unsigned short)(state->default_slot_quota * 256),
		(unsigned short)~0);

	local->trigger.event = &state->trigger_event;
	remote_event_create(&local->trigger);
	local->tx_pos = 0;

	local->recycle.event = &state->recycle_event;
	remote_event_create(&local->recycle);
	local->slot_queue_recycle = state->slot_queue_available;

	local->sync_trigger.event = &state->sync_trigger_event;
	remote_event_create(&local->sync_trigger);

	local->sync_release.event = &state->sync_release_event;
	remote_event_create(&local->sync_release);

	/* At start-of-day, the slot is empty and available */
	((VCHIQ_HEADER_T *)SLOT_DATA_FROM_INDEX(state, local->slot_sync))->msgid
		= VCHIQ_MSGID_PADDING;
	remote_event_signal_local(&local->sync_release);

	local->debug[DEBUG_ENTRIES] = DEBUG_MAX;

	status = vchiq_platform_init_state(state);

	/*
		bring up slot handler thread
	 */
	snprintf(threadname, sizeof(threadname), "VCHIQ-%d", state->id);
	state->slot_handler_thread = kthread_create(&slot_handler_func,
		(void *)state,
		threadname);

	if (state->slot_handler_thread == NULL) {
		vchiq_loud_error_header();
		vchiq_loud_error("couldn't create thread %s", threadname);
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}
	{
		static struct sched_param param = { .sched_priority = 3 };
		sched_setscheduler(state->slot_handler_thread, SCHED_FIFO, &param);
	}
	wake_up_process(state->slot_handler_thread);

	snprintf(threadname, sizeof(threadname), "VCHIQr-%d", state->id);
	state->recycle_thread = kthread_create(&recycle_func,
		(void *)state,
		threadname);
	if (state->recycle_thread == NULL) {
		vchiq_loud_error_header();
		vchiq_loud_error("couldn't create thread %s", threadname);
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}
	set_user_nice(state->recycle_thread, -19);
	wake_up_process(state->recycle_thread);

	snprintf(threadname, sizeof(threadname), "VCHIQs-%d", state->id);
	state->sync_thread = kthread_create(&sync_func,
		(void *)state,
		threadname);
	if (state->sync_thread == NULL) {
		vchiq_loud_error_header();
		vchiq_loud_error("couldn't create thread %s", threadname);
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}
	set_user_nice(state->sync_thread, -20);
	wake_up_process(state->sync_thread);

	snprintf(threadname, sizeof(threadname), "VCHIQp-%d", state->id);
	state->ping_thread = kthread_create(&ping_func,
		(void *)state,
		threadname);
	if (state->ping_thread == NULL) {
		vchiq_loud_error_header();
		vchiq_loud_error("couldn't create thread %s", threadname);
		vchiq_loud_error_footer();
		return VCHIQ_ERROR;
	}
	wake_up_process(state->ping_thread);

	/* Indicate readiness to the other side */
	local->initialised = 1;

	return status;
}

/* Called from application thread when a client or server service is created. */
VCHIQ_SERVICE_T *
vchiq_add_service_internal(VCHIQ_STATE_T *state,
	const VCHIQ_SERVICE_PARAMS_T *params, int srvstate,
	VCHIQ_INSTANCE_T instance)
{
	VCHIQ_SERVICE_T **pservice = NULL;
	VCHIQ_SERVICE_T *service = NULL;
	int i;

	/* Prepare to use a previously unused service */
	if (state->unused_service < VCHIQ_MAX_SERVICES)
		pservice = &state->services[state->unused_service];

	if (srvstate == VCHIQ_SRVSTATE_OPENING) {
		for (i = 0; i < state->unused_service; i++) {
			VCHIQ_SERVICE_T *srv = state->services[i];
			if (!srv) {
				pservice = &state->services[i];
				break;
			}
			if (srv->srvstate == VCHIQ_SRVSTATE_FREE) {
				service = srv;
				break;
			}
		}
	} else {
		for (i = (state->unused_service - 1); i >= 0; i--) {
			VCHIQ_SERVICE_T *srv = state->services[i];
			if (!srv)
				pservice = &state->services[i];
			else if (srv->srvstate == VCHIQ_SRVSTATE_FREE) {
				service = srv;
			} else if ((srv->public_fourcc == params->fourcc) &&
				((srv->instance != instance)
				|| (srv->base.callback != params->callback))) {
				/* There is another server using this fourcc
				** which doesn't match */
				pservice = NULL;
				service = NULL;
			}
		}
	}

	if (pservice && !service) {
		service = kzalloc(sizeof(VCHIQ_SERVICE_T), GFP_KERNEL);
		if (service) {
			service->magic = VCHIQ_MAGIC;
			service->srvstate = VCHIQ_SRVSTATE_FREE;
			service->localport = (pservice - state->services);
			sema_init(&service->remove_event, 0);
			sema_init(&service->bulk_remove_event, 0);
			mutex_init(&service->bulk_mutex);
			*pservice = service;
		} else {
			vchiq_log_error(vchiq_core_log_level,
				"vchiq: Out of memory");
		}
	}

	if (service) {
		VCHIQ_SERVICE_QUOTA_T *service_quota =
			&state->service_quotas[service->localport];
		vchiq_log_info(vchiq_core_msg_log_level,
			"%s Service %c%c%c%c SrcPort:%d",
			(srvstate == VCHIQ_SRVSTATE_OPENING)
			? "Open" : "Add",
			VCHIQ_FOURCC_AS_4CHARS(params->fourcc),
			service->localport);

		service->state = state;
		service->base.fourcc   = params->fourcc;
		service->base.callback = params->callback;
		service->base.userdata = params->userdata;
		service->version       = params->version;
		service->version_min   = params->version_min;
		vchiq_set_service_state(service, srvstate);
		service->public_fourcc = (srvstate == VCHIQ_SRVSTATE_OPENING) ?
			VCHIQ_FOURCC_INVALID : params->fourcc;
		service->instance = instance;
		service->remoteport = VCHIQ_PORT_FREE;
		service->client_id = 0;
		service->auto_close = 1;
		service->sync = 0;
		service->service_use_count = 0;
		init_bulk_queue(&service->bulk_tx);
		init_bulk_queue(&service->bulk_rx);
		service_quota->slot_quota = state->default_slot_quota;
		service_quota->message_quota = state->default_message_quota;
		if (service_quota->slot_use_count == 0)
			service_quota->previous_tx_index =
				SLOT_QUEUE_INDEX_FROM_POS(state->local_tx_pos)
				- 1;
		memset(&service->stats, 0, sizeof(service->stats));
		atomic_set(&service->poll_flags, 0);

		/* Ensure the events are unsignalled */
		while (down_trylock(&service->remove_event) == 0)
			continue;
		while (down_trylock(&service_quota->quota_event) == 0)
			continue;

		if (pservice == &state->services[state->unused_service])
			state->unused_service++;
	}

	return service;
}

VCHIQ_STATUS_T
vchiq_open_service_internal(VCHIQ_SERVICE_T *service, int client_id)
{
	struct vchiq_open_payload payload = {
		service->base.fourcc,
		client_id,
		service->version,
		service->version_min
	};
	VCHIQ_ELEMENT_T body = { &payload, sizeof(payload) };
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;

	service->client_id = client_id;
	vchiq_use_service(&service->base);
	status = queue_message(service->state, NULL,
		VCHIQ_MAKE_MSG(VCHIQ_MSG_OPEN, service->localport, 0),
		&body, 1, sizeof(payload), 1);
	if (status == VCHIQ_SUCCESS) {
		if (down_interruptible(&service->remove_event) != 0) {
			status = VCHIQ_RETRY;
			vchiq_release_service(&service->base);
		} else if ((service->srvstate != VCHIQ_SRVSTATE_OPEN) &&
			(service->srvstate != VCHIQ_SRVSTATE_OPENSYNC)) {
			vchiq_log_info(vchiq_core_log_level,
				"%d: osi - srvstate = %d",
				service->state->id, service->srvstate);
			WARN_ON(service->srvstate != VCHIQ_SRVSTATE_CLOSEWAIT);
			status = VCHIQ_ERROR;
			VCHIQ_SERVICE_STATS_INC(service, error_count);
			vchiq_release_service(&service->base);
		}
	}
	return status;
}

/* Called by the slot handler */
VCHIQ_STATUS_T
vchiq_close_service_internal(VCHIQ_SERVICE_T *service, int close_recvd)
{
	VCHIQ_STATE_T *state = service->state;
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;

	vchiq_log_trace(vchiq_core_log_level, "%d: csi:%d (%s)",
		service->state->id, service->localport,
		srvstate_names[service->srvstate]);

	switch (service->srvstate) {
	case VCHIQ_SRVSTATE_OPENING:
		if (close_recvd) {
			/* The open was rejected - tell the user */
			vchiq_set_service_state(service,
				VCHIQ_SRVSTATE_CLOSEWAIT);
			up(&service->remove_event);
		} else {
			/* Shutdown mid-open - let the other side know */
			status = queue_message(state, NULL,
				VCHIQ_MAKE_MSG
				(VCHIQ_MSG_CLOSE,
				service->localport,
				VCHIQ_MSG_DSTPORT(service->remoteport)),
				NULL, 0, 0, 0);

			if (status == VCHIQ_SUCCESS)
				vchiq_set_service_state(service,
					VCHIQ_SRVSTATE_CLOSESENT);
		}
		break;

	case VCHIQ_SRVSTATE_OPENSYNC:
		mutex_lock(&state->sync_mutex);
		/* Drop through */

	case VCHIQ_SRVSTATE_OPEN:
		if (state->is_master) {
			/* Abort any outstanding bulk transfers */
			if (mutex_lock_interruptible(&service->bulk_mutex)
				== 0) {
				abort_outstanding_bulks(service,
					&service->bulk_tx);
				abort_outstanding_bulks(service,
					&service->bulk_rx);
				mutex_unlock(&service->bulk_mutex);

				status = notify_bulks(service,
					&service->bulk_tx);
				if (status == VCHIQ_SUCCESS)
					status = notify_bulks(service,
						&service->bulk_rx);
			}
		}

		if (status == VCHIQ_SUCCESS)
			status = queue_message(state, NULL,
				VCHIQ_MAKE_MSG
				(VCHIQ_MSG_CLOSE,
				service->localport,
				VCHIQ_MSG_DSTPORT(service->remoteport)),
				NULL, 0, 0, 0);

		if (status == VCHIQ_SUCCESS)
			vchiq_set_service_state(service,
				close_recvd ?  VCHIQ_SRVSTATE_CLOSING :
				VCHIQ_SRVSTATE_CLOSESENT);
		else if (service->srvstate == VCHIQ_SRVSTATE_OPENSYNC)
			mutex_unlock(&state->sync_mutex);
		break;

	case VCHIQ_SRVSTATE_CLOSESENT:
		WARN_ON(!close_recvd);

		if (!state->is_master) {
			/* Abort any outstanding bulk transfers */
			if (mutex_lock_interruptible(&service->bulk_mutex)
				== 0) {
				abort_outstanding_bulks(service,
					&service->bulk_tx);
				abort_outstanding_bulks(service,
					&service->bulk_rx);
				mutex_unlock(&service->bulk_mutex);

				status = notify_bulks(service,
					&service->bulk_tx);
				if (status == VCHIQ_SUCCESS)
					status = notify_bulks(service,
						&service->bulk_rx);
			}
		}

		if (status == VCHIQ_SUCCESS)
			vchiq_set_service_state(service,
				VCHIQ_SRVSTATE_CLOSING);
		break;

	case VCHIQ_SRVSTATE_CLOSING:
		/* We may come here after a retry */
		WARN_ON(close_recvd);
		break;

	case VCHIQ_SRVSTATE_CLOSEWAIT:
		/* We may come here on process death */
		break;

	case VCHIQ_SRVSTATE_HIDDEN:
	case VCHIQ_SRVSTATE_LISTENING:
		vchiq_set_service_state(service, VCHIQ_SRVSTATE_CLOSING);
		break;

	default:
		vchiq_log_error(vchiq_core_log_level,
			"vchiq_close_service_internal(%d) called in state %s",
			close_recvd, srvstate_names[service->srvstate]);
		WARN(1, "invalid state for close\n");
		break;
	}

	if (service->srvstate == VCHIQ_SRVSTATE_CLOSING) {
		int i;
		int uc = service->service_use_count;
		/* Complete the close process */
		for (i = 0; i < uc; i++)
			/* cater for cases where close is forced and the
			** client may not close all it's handles */
			vchiq_release_service_internal(service);

		service->client_id = 0;

		/* Now tell the client that the service is closed */
		if (service->instance) {
			int oldstate = service->srvstate;

			/* Change the service state now for the benefit
			** of the callback */
			vchiq_set_service_state(service,
				((service->public_fourcc ==
				VCHIQ_FOURCC_INVALID) ||
				!service->auto_close) ?
				VCHIQ_SRVSTATE_CLOSEWAIT :
				VCHIQ_SRVSTATE_LISTENING);

			status = make_service_callback(service,
				VCHIQ_SERVICE_CLOSED, NULL, NULL);

			if (status == VCHIQ_RETRY)
				/* Restore the old state, to be retried later */
				vchiq_set_service_state(service, oldstate);
			else if (status == VCHIQ_ERROR)
				/* Signal an error (fatal, since the other end
				** will probably have closed) */
				vchiq_set_service_state(service,
					VCHIQ_SRVSTATE_OPEN);
		}

		if (status != VCHIQ_RETRY) {
			if (service->srvstate == VCHIQ_SRVSTATE_CLOSING)
				vchiq_set_service_state(service,
					VCHIQ_SRVSTATE_CLOSEWAIT);
			up(&service->remove_event);
		}
	}

	return status;
}

/* Called from the application process upon process death */
void
vchiq_terminate_service_internal(VCHIQ_SERVICE_T *service)
{
	VCHIQ_STATE_T *state = service->state;

	vchiq_log_info(vchiq_core_log_level, "%d: tsi - (%d<->%d)",
		state->id, service->localport, service->remoteport);

	/* Disconnect from the instance, to prevent any callbacks */
	service->instance = NULL;

	/* Mark the service for termination by the slot handler */
	request_poll(state, service, VCHIQ_POLL_TERMINATE);
}

/* Called from the application process upon process death, and from
	vchiq_remove_service */
void
vchiq_free_service_internal(VCHIQ_SERVICE_T *service)
{
	VCHIQ_STATE_T *state = service->state;
	int slot_last = state->remote->slot_last;
	int i;

	vchiq_log_info(vchiq_core_log_level, "%d: fsi - (%d)",
		state->id, service->localport);

	if (mutex_lock_interruptible(&state->mutex) != 0)
		return;

	/* Release any claimed messages */
	for (i = state->remote->slot_first; i <= slot_last; i++) {
		VCHIQ_SLOT_INFO_T *slot_info =
			SLOT_INFO_FROM_INDEX(state, i);
		if (slot_info->release_count != slot_info->use_count) {
			char *data =
				(char *)SLOT_DATA_FROM_INDEX(state, i);
			unsigned int pos, end;

			end = VCHIQ_SLOT_SIZE;
			if (data == state->rx_data)
				/* This buffer is still being read from - stop
				** at the current read position */
				end = state->rx_pos & VCHIQ_SLOT_MASK;

			pos = 0;

			while (pos < end) {
				VCHIQ_HEADER_T *header =
					(VCHIQ_HEADER_T *)(data + pos);
				int msgid = header->msgid;
				int port = VCHIQ_MSG_DSTPORT(msgid);
				if ((port == service->localport) &&
					(msgid & VCHIQ_MSGID_CLAIMED)) {
					header->msgid = msgid &
						~VCHIQ_MSGID_CLAIMED;
					vchiq_log_info(vchiq_core_log_level,
						"  fsi - hdr %x",
						(unsigned int)header);
					release_slot(state, slot_info);
				}
				pos += calc_stride(header->size);
				if (pos > VCHIQ_SLOT_SIZE) {
					vchiq_log_error(vchiq_core_log_level,
						"fsi - pos %x: header %x, "
						"msgid %x, header->msgid %x, "
						"header->size %x",
						pos, (unsigned int)header,
						msgid, header->msgid,
						header->size);
					WARN(1, "invalid slot position\n");
				}
			}
		}
	}

	WARN_ON(state->services[service->localport] != service);
	vchiq_set_service_state(service, VCHIQ_SRVSTATE_FREE);
	state->services[service->localport] = NULL;
	service->magic = 0;
	kfree(service);
	mutex_unlock(&state->mutex);
}

VCHIQ_STATUS_T
vchiq_connect_internal(VCHIQ_STATE_T *state, VCHIQ_INSTANCE_T instance)
{
	int i;

	/* Find all services registered to this client and enable them. */
	for (i = 0; i < state->unused_service; i++) {
		VCHIQ_SERVICE_T *service = state->services[i];
		if (service && (service->instance == instance)) {
			if (service->srvstate == VCHIQ_SRVSTATE_HIDDEN)
				vchiq_set_service_state(service,
					VCHIQ_SRVSTATE_LISTENING);
		}
	}

	if (state->conn_state == VCHIQ_CONNSTATE_DISCONNECTED) {
		if (queue_message(state, NULL,
			VCHIQ_MAKE_MSG(VCHIQ_MSG_CONNECT, 0, 0), NULL, 0,
			0, 1) == VCHIQ_RETRY)
			return VCHIQ_RETRY;

		vchiq_set_conn_state(state, VCHIQ_CONNSTATE_CONNECTING);
	}

	if (state->conn_state == VCHIQ_CONNSTATE_CONNECTING) {
		if (down_interruptible(&state->connect) != 0)
			return VCHIQ_RETRY;

		vchiq_set_conn_state(state, VCHIQ_CONNSTATE_CONNECTED);
		up(&state->connect);
	}

	return VCHIQ_SUCCESS;
}

VCHIQ_STATUS_T
vchiq_shutdown_internal(VCHIQ_STATE_T *state, VCHIQ_INSTANCE_T instance)
{
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;
	int i;

	/* Find all services registered to this client and close them. */
	for (i = 0; i < state->unused_service; i++) {
		VCHIQ_SERVICE_T *service = state->services[i];
		if (service && (service->instance == instance) &&
			((service->srvstate == VCHIQ_SRVSTATE_OPEN) ||
			(service->srvstate == VCHIQ_SRVSTATE_LISTENING))) {
			status = vchiq_remove_service(&service->base);
			if (status != VCHIQ_SUCCESS)
				break;
		}
	}

	return status;
}

VCHIQ_STATUS_T
vchiq_pause_internal(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;

	switch (state->conn_state) {
	case VCHIQ_CONNSTATE_CONNECTED:
		/* Request a pause */
		vchiq_set_conn_state(state, VCHIQ_CONNSTATE_PAUSING);
		request_poll(state, NULL, 0);
		break;
	default:
		vchiq_log_error(vchiq_core_log_level,
			"vchiq_pause_internal in state %s\n",
			conn_state_names[state->conn_state]);
		status = VCHIQ_ERROR;
		VCHIQ_STATS_INC(state, error_count);
		break;
	}

	return status;
}

VCHIQ_STATUS_T
vchiq_resume_internal(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;

	if (state->conn_state == VCHIQ_CONNSTATE_PAUSED) {
		vchiq_set_conn_state(state, VCHIQ_CONNSTATE_RESUMING);
		request_poll(state, NULL, 0);
	} else {
		status = VCHIQ_ERROR;
		VCHIQ_STATS_INC(state, error_count);
	}

	return status;
}

VCHIQ_STATUS_T
vchiq_close_service(VCHIQ_SERVICE_HANDLE_T handle)
{
	/* Unregister the service */
	VCHIQ_SERVICE_T *service = (VCHIQ_SERVICE_T *) handle;
	VCHIQ_STATUS_T status = VCHIQ_ERROR;

	if (!is_valid_service(service))
		return VCHIQ_ERROR;

	vchiq_log_info(vchiq_core_log_level,
		"%d: close_service:%d",
		service->state->id, service->localport);

	if (service->public_fourcc != VCHIQ_FOURCC_INVALID) {
		if (service->srvstate == VCHIQ_SRVSTATE_CLOSEWAIT) {
			/* This is a non-auto-close server */
			vchiq_set_service_state(service,
				VCHIQ_SRVSTATE_LISTENING);
			status = VCHIQ_SUCCESS;
		}
	} else
		/* For clients, make it an alias of vchiq_remove_service */
		status = vchiq_remove_service(handle);

	return status;
}

VCHIQ_STATUS_T
vchiq_remove_service(VCHIQ_SERVICE_HANDLE_T handle)
{
	/* Unregister the service */
	VCHIQ_SERVICE_T *service = (VCHIQ_SERVICE_T *) handle;
	VCHIQ_STATUS_T status = VCHIQ_SUCCESS;

	if (!is_valid_service(service))
		return VCHIQ_ERROR;

	vchiq_log_info(vchiq_core_log_level,
		"%d: remove_service:%d",
		service->state->id, service->localport);

	switch (service->srvstate) {
	case VCHIQ_SRVSTATE_OPENING:
	case VCHIQ_SRVSTATE_OPEN:
		/* Mark the service for termination by the slot handler */
		request_poll(service->state, service, VCHIQ_POLL_TERMINATE);

		/* Drop through... */
	case VCHIQ_SRVSTATE_CLOSESENT:
	case VCHIQ_SRVSTATE_CLOSING:
		while ((service->srvstate != VCHIQ_SRVSTATE_CLOSEWAIT) &&
			(service->srvstate != VCHIQ_SRVSTATE_LISTENING)) {
			if (down_interruptible(&service->remove_event) != 0) {
				status = VCHIQ_RETRY;
				break;
			}
		}
		break;

	default:
		break;
	}

	if (status == VCHIQ_SUCCESS) {
		if (service->srvstate == VCHIQ_SRVSTATE_OPEN)
			status = VCHIQ_ERROR;
		else {
			service->instance = NULL;
			vchiq_free_service_internal(service);
		}
	}

	return status;
}


/* This function may be called by kernel threads or user threads.
 * User threads may receive VCHIQ_RETRY to indicate that a signal has been
 * received and the call should be retried after being returned to user
 * context.
 * When called in blocking mode, the userdata field points to a bulk_waiter
 * structure.
 */
VCHIQ_STATUS_T
vchiq_bulk_transfer(VCHIQ_SERVICE_T *service,
	VCHI_MEM_HANDLE_T memhandle, void *offset, int size, void *userdata,
	VCHIQ_BULK_MODE_T mode, VCHIQ_BULK_DIR_T dir)
{
	VCHIQ_BULK_QUEUE_T *queue = (dir == VCHIQ_BULK_TRANSMIT) ?
		&service->bulk_tx : &service->bulk_rx;
	VCHIQ_BULK_T *bulk;
	VCHIQ_STATE_T *state;
	struct bulk_waiter *bulk_waiter = NULL;
	const char dir_char = (dir == VCHIQ_BULK_TRANSMIT) ? 't' : 'r';
	const int dir_msgtype = (dir == VCHIQ_BULK_TRANSMIT) ?
		VCHIQ_MSG_BULK_TX : VCHIQ_MSG_BULK_RX;
	VCHIQ_STATUS_T status = VCHIQ_ERROR;

	if (!is_valid_service(service) ||
		 (service->srvstate != VCHIQ_SRVSTATE_OPEN) ||
		 ((memhandle == VCHI_MEM_HANDLE_INVALID) && (offset == NULL)) ||
		 (vchiq_check_service(service) != VCHIQ_SUCCESS))
		return VCHIQ_ERROR;

	switch (mode) {
	case VCHIQ_BULK_MODE_NOCALLBACK:
	case VCHIQ_BULK_MODE_CALLBACK:
		break;
	case VCHIQ_BULK_MODE_BLOCKING:
		bulk_waiter = (struct bulk_waiter *)userdata;
		sema_init(&bulk_waiter->event, 0);
		bulk_waiter->actual = 0;
		bulk_waiter->bulk = NULL;
		break;
	case VCHIQ_BULK_MODE_WAITING:
		bulk_waiter = (struct bulk_waiter *)userdata;
		bulk = bulk_waiter->bulk;
		goto waiting;
	default:
		return VCHIQ_ERROR;
	}

	state = service->state;

	if (mutex_lock_interruptible(&service->bulk_mutex) != 0)
		return VCHIQ_RETRY;

	if (queue->local_insert == queue->remove + VCHIQ_NUM_SERVICE_BULKS) {
		VCHIQ_SERVICE_STATS_INC(service, bulk_stalls);
		do {
			mutex_unlock(&service->bulk_mutex);
			if (down_interruptible(&service->bulk_remove_event)
				!= 0)
				return VCHIQ_RETRY;
			if (mutex_lock_interruptible(&service->bulk_mutex) != 0)
				return VCHIQ_RETRY;
		} while (queue->local_insert == queue->remove +
				VCHIQ_NUM_SERVICE_BULKS);
	}

	bulk = &queue->bulks[BULK_INDEX(queue->local_insert)];

	bulk->mode = mode;
	bulk->dir = dir;
	bulk->userdata = userdata;
	bulk->size = size;
	bulk->actual = VCHIQ_BULK_ACTUAL_ABORTED;
	if (bulk_waiter)
		bulk_waiter->bulk = bulk;

	if (vchiq_prepare_bulk_data(bulk, memhandle, offset, size, dir) !=
		VCHIQ_SUCCESS)
		goto error_exit;

	wmb();

	vchiq_log_info(vchiq_core_log_level,
		"%d: bt (%d->%d) %cx %x@%x %x",
		state->id,
		service->localport, service->remoteport, dir_char,
		size, (unsigned int)bulk->data, (unsigned int)userdata);

	if (state->is_master) {
		queue->local_insert++;
		if (resolve_bulks(service, queue))
			request_poll(state, service,
				(dir == VCHIQ_BULK_TRANSMIT) ?
				VCHIQ_POLL_TXNOTIFY : VCHIQ_POLL_RXNOTIFY);
	} else {
		int payload[2] = { (int)bulk->data, bulk->size };
		VCHIQ_ELEMENT_T element = { payload, sizeof(payload) };

		if (queue_message(state, NULL,
			VCHIQ_MAKE_MSG(dir_msgtype,
				service->localport, service->remoteport),
			&element, 1, sizeof(payload), 1) != VCHIQ_SUCCESS) {
			vchiq_complete_bulk(bulk);
			goto error_exit;
		}
		queue->local_insert++;
		queue->remote_insert++;
	}

	mutex_unlock(&service->bulk_mutex);

	vchiq_log_trace(vchiq_core_log_level,
		"%d: bt:%d %cx li=%x ri=%x p=%x",
		state->id,
		service->localport, dir_char,
		queue->local_insert, queue->remote_insert, queue->process);

waiting:
	status = VCHIQ_SUCCESS;

	if (bulk_waiter) {
		if (down_interruptible(&bulk_waiter->event) != 0) {
#if 0
			vchiq_log_warning(vchiq_core_log_level,
				"bulk wait interrupted");
#endif
			status = VCHIQ_RETRY;
		} else if (bulk_waiter->actual == VCHIQ_BULK_ACTUAL_ABORTED)
			status = VCHIQ_ERROR;
	}

	return status;

error_exit:
	mutex_unlock(&service->bulk_mutex);

	return status;
}

VCHIQ_STATUS_T
vchiq_queue_bulk_transmit(VCHIQ_SERVICE_HANDLE_T handle,
	const void *data, int size, void *userdata)
{
	return vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		VCHI_MEM_HANDLE_INVALID, (void *)data, size, userdata,
		VCHIQ_BULK_MODE_CALLBACK, VCHIQ_BULK_TRANSMIT);
}

VCHIQ_STATUS_T
vchiq_queue_bulk_receive(VCHIQ_SERVICE_HANDLE_T handle, void *data, int size,
	void *userdata)
{
	return vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		VCHI_MEM_HANDLE_INVALID, data, size, userdata,
		VCHIQ_BULK_MODE_CALLBACK, VCHIQ_BULK_RECEIVE);
}

VCHIQ_STATUS_T
vchiq_queue_bulk_transmit_handle(VCHIQ_SERVICE_HANDLE_T handle,
	VCHI_MEM_HANDLE_T memhandle, const void *offset, int size,
	void *userdata)
{
	return vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		memhandle, (void *)offset, size, userdata,
		VCHIQ_BULK_MODE_CALLBACK, VCHIQ_BULK_TRANSMIT);
}

VCHIQ_STATUS_T
vchiq_queue_bulk_receive_handle(VCHIQ_SERVICE_HANDLE_T handle,
	VCHI_MEM_HANDLE_T memhandle, void *offset, int size,
	void *userdata)
{
	return vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		memhandle, offset, size, userdata,
		VCHIQ_BULK_MODE_CALLBACK, VCHIQ_BULK_RECEIVE);
}

VCHIQ_STATUS_T
vchiq_bulk_transmit(VCHIQ_SERVICE_HANDLE_T handle, const void *data, int size,
	void *userdata, VCHIQ_BULK_MODE_T mode)
{
	struct bulk_waiter bulk_waiter;
	VCHIQ_STATUS_T status;

	switch (mode) {
	case VCHIQ_BULK_MODE_NOCALLBACK:
	case VCHIQ_BULK_MODE_CALLBACK:
		break;
	case VCHIQ_BULK_MODE_BLOCKING:
		userdata = &bulk_waiter;
		break;
	default:
		return VCHIQ_ERROR;
	}

	status = vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		VCHI_MEM_HANDLE_INVALID, (void *)data, size, userdata,
		mode, VCHIQ_BULK_TRANSMIT);

	/* This call is for kernel thread use and should not be interrupted */
	BUG_ON(status == VCHIQ_RETRY);
	return status;
}

VCHIQ_STATUS_T
vchiq_bulk_receive(VCHIQ_SERVICE_HANDLE_T handle, void *data, int size,
	void *userdata, VCHIQ_BULK_MODE_T mode)
{
	struct bulk_waiter bulk_waiter;
	VCHIQ_STATUS_T status;

	switch (mode) {
	case VCHIQ_BULK_MODE_NOCALLBACK:
	case VCHIQ_BULK_MODE_CALLBACK:
		break;
	case VCHIQ_BULK_MODE_BLOCKING:
		userdata = &bulk_waiter;
		break;
	default:
		return VCHIQ_ERROR;
	}

	status = vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		VCHI_MEM_HANDLE_INVALID, data, size, userdata,
		mode, VCHIQ_BULK_RECEIVE);

	/* This call is for kernel thread use and should not be interrupted */
	BUG_ON(status == VCHIQ_RETRY);
	return status;
}

VCHIQ_STATUS_T
vchiq_bulk_transmit_handle(VCHIQ_SERVICE_HANDLE_T handle,
	VCHI_MEM_HANDLE_T memhandle, const void *offset, int size,
	void *userdata, VCHIQ_BULK_MODE_T mode)
{
	struct bulk_waiter bulk_waiter;
	VCHIQ_STATUS_T status;

	switch (mode) {
	case VCHIQ_BULK_MODE_NOCALLBACK:
	case VCHIQ_BULK_MODE_CALLBACK:
		break;
	case VCHIQ_BULK_MODE_BLOCKING:
		userdata = &bulk_waiter;
		break;
	default:
		return VCHIQ_ERROR;
	}

	status = vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		memhandle, (void *)offset, size, userdata,
		mode, VCHIQ_BULK_TRANSMIT);

	/* This call is for kernel thread use and should not be interrupted */
	BUG_ON(status == VCHIQ_RETRY);
	return status;
}

VCHIQ_STATUS_T
vchiq_bulk_receive_handle(VCHIQ_SERVICE_HANDLE_T handle,
	VCHI_MEM_HANDLE_T memhandle, void *offset, int size, void *userdata,
	VCHIQ_BULK_MODE_T mode)
{
	struct bulk_waiter bulk_waiter;
	VCHIQ_STATUS_T status;

	switch (mode) {
	case VCHIQ_BULK_MODE_NOCALLBACK:
	case VCHIQ_BULK_MODE_CALLBACK:
		break;
	case VCHIQ_BULK_MODE_BLOCKING:
		userdata = &bulk_waiter;
		break;
	default:
		return VCHIQ_ERROR;
	}

	status = vchiq_bulk_transfer((VCHIQ_SERVICE_T *)handle,
		memhandle, offset, size, userdata,
		mode, VCHIQ_BULK_RECEIVE);

	/* This call is for kernel thread use and should not be interrupted */
	BUG_ON(status == VCHIQ_RETRY);
	return status;
}

VCHIQ_STATUS_T
vchiq_queue_message(VCHIQ_SERVICE_HANDLE_T handle,
	const VCHIQ_ELEMENT_T *elements, int count)
{
	VCHIQ_SERVICE_T *service = (VCHIQ_SERVICE_T *) handle;

	unsigned int size = 0;
	unsigned int i;
	char ping[] = "ping";

	if (!is_valid_service(service) ||
		(vchiq_check_service(service) != VCHIQ_SUCCESS))
		return VCHIQ_ERROR;

	for (i = 0; i < (unsigned int)count; i++) {
		if (elements[i].size) {
			if (elements[i].data == NULL) {
				VCHIQ_SERVICE_STATS_INC(service, error_count);
				return VCHIQ_ERROR;
			}
			size += elements[i].size;
			if (i == 0 &&
			    elements[i].size == strlen(ping) + 1 &&
			    strncmp(ping, (char*)elements[i].data, elements[i].size) == 0) {
				complete(&service->state->ping_send);
			}
		}
	}

	if (size > VCHIQ_MAX_MSG_SIZE) {
		VCHIQ_SERVICE_STATS_INC(service, error_count);
		return VCHIQ_ERROR;
	}

	if (service->srvstate != VCHIQ_SRVSTATE_OPEN) {
		if (service->srvstate == VCHIQ_SRVSTATE_OPENSYNC)
			return queue_message_sync(service->state, service,
				VCHIQ_MAKE_MSG(VCHIQ_MSG_DATA,
					service->localport,
					service->remoteport),
				elements, count, size, 1);
		return VCHIQ_ERROR;
	}

	return queue_message(service->state, service,
				VCHIQ_MAKE_MSG(VCHIQ_MSG_DATA,
					service->localport,
					service->remoteport),
				elements, count, size, 1);
}

void
vchiq_release_message(VCHIQ_SERVICE_HANDLE_T handle, VCHIQ_HEADER_T *header)
{
	VCHIQ_SERVICE_T *service = (VCHIQ_SERVICE_T *)handle;
	VCHIQ_SHARED_STATE_T *remote;
	VCHIQ_STATE_T *state;
	int slot_index;

	if (!is_valid_service(service))
		return;

	state = service->state;
	remote = state->remote;

	slot_index = SLOT_INDEX_FROM_DATA(state, (void *)header);

	if ((slot_index >= remote->slot_first) &&
		(slot_index <= remote->slot_last)) {
		int msgid = header->msgid;
		if (msgid & VCHIQ_MSGID_CLAIMED) {
			VCHIQ_SLOT_INFO_T *slot_info =
				SLOT_INFO_FROM_INDEX(state, slot_index);

			/* Rewrite the message header to prevent a double
			** release */
			header->msgid = msgid & ~VCHIQ_MSGID_CLAIMED;

			release_slot(state, slot_info);
		}
	} else if (slot_index == remote->slot_sync)
		release_message_sync(state, header);
}

static void
release_message_sync(VCHIQ_STATE_T *state, VCHIQ_HEADER_T *header)
{
	header->msgid = VCHIQ_MSGID_PADDING;
	wmb();
	remote_event_signal(&state->remote->sync_release);
}

int
vchiq_get_client_id(VCHIQ_SERVICE_HANDLE_T handle)
{
	VCHIQ_SERVICE_T *service = (VCHIQ_SERVICE_T *)handle;
	return service ? service->client_id : 0;
}

VCHIQ_STATUS_T
vchiq_get_config(VCHIQ_INSTANCE_T instance,
	int config_size, VCHIQ_CONFIG_T *pconfig)
{
	VCHIQ_CONFIG_T config;

	(void)instance;

	config.max_msg_size           = VCHIQ_MAX_MSG_SIZE;
	config.bulk_threshold         = VCHIQ_MAX_MSG_SIZE;
	config.max_outstanding_bulks  = VCHIQ_NUM_SERVICE_BULKS;
	config.max_services           = VCHIQ_MAX_SERVICES;
	config.version                = VCHIQ_VERSION;
	config.version_min            = VCHIQ_VERSION_MIN;

	if (config_size > sizeof(VCHIQ_CONFIG_T))
		return VCHIQ_ERROR;

	memcpy(pconfig, &config,
		min(config_size, (int)(sizeof(VCHIQ_CONFIG_T))));

	return VCHIQ_SUCCESS;
}

VCHIQ_STATUS_T
vchiq_set_service_option(VCHIQ_SERVICE_HANDLE_T handle,
	VCHIQ_SERVICE_OPTION_T option, int value)
{
	VCHIQ_SERVICE_T *service = (VCHIQ_SERVICE_T *)handle;
	VCHIQ_STATUS_T status = VCHIQ_ERROR;

	if (is_valid_service(service)) {
		switch (option) {
		case VCHIQ_SERVICE_OPTION_AUTOCLOSE:
			service->auto_close = value;
			status = VCHIQ_SUCCESS;
			break;

		case VCHIQ_SERVICE_OPTION_SLOT_QUOTA: {
			VCHIQ_SERVICE_QUOTA_T *service_quota =
				&service->state->service_quotas[
					service->localport];
			if (value == 0)
				value = service->state->default_slot_quota;
			if ((value >= service_quota->slot_use_count) &&
				 (value < (unsigned short)~0)) {
				service_quota->slot_quota = value;
				if ((value >= service_quota->slot_use_count) &&
					(service_quota->message_quota >=
					 service_quota->message_use_count)) {
					/* Signal the service that it may have
					** dropped below its quota */
					up(&service_quota->quota_event);
				}
				status = VCHIQ_SUCCESS;
			}
		} break;

		case VCHIQ_SERVICE_OPTION_MESSAGE_QUOTA: {
			VCHIQ_SERVICE_QUOTA_T *service_quota =
				&service->state->service_quotas[
					service->localport];
			if (value == 0)
				value = service->state->default_message_quota;
			if ((value >= service_quota->message_use_count) &&
				 (value < (unsigned short)~0)) {
				service_quota->message_quota = value;
				if ((value >=
					service_quota->message_use_count) &&
					(service_quota->slot_quota >=
					service_quota->slot_use_count))
					/* Signal the service that it may have
					** dropped below its quota */
					up(&service_quota->quota_event);
				status = VCHIQ_SUCCESS;
			}
		} break;

		case VCHIQ_SERVICE_OPTION_SYNCHRONOUS:
			if ((service->srvstate == VCHIQ_SRVSTATE_HIDDEN) ||
				(service->srvstate ==
				VCHIQ_SRVSTATE_LISTENING)) {
				service->sync = value;
				status = VCHIQ_SUCCESS;
			}
			break;

		default:
			break;
		}
	}

	return status;
}

void
vchiq_dump_shared_state(void *dump_context, VCHIQ_STATE_T *state,
	VCHIQ_SHARED_STATE_T *shared, const char *label)
{
	static const char *const debug_names[] = {
		"<entries>",
		"SLOT_HANDLER_COUNT",
		"SLOT_HANDLER_LINE",
		"PARSE_LINE",
		"PARSE_HEADER",
		"PARSE_MSGID",
		"AWAIT_COMPLETION_LINE",
		"DEQUEUE_MESSAGE_LINE",
		"SERVICE_CALLBACK_LINE",
		"MSG_QUEUE_FULL_COUNT",
		"COMPLETION_QUEUE_FULL_COUNT"
	};
	int i;

	char buf[80];
	int len;
	len = snprintf(buf, sizeof(buf),
		"  %s: slots %d-%d tx_pos=%x recycle=%x",
		label, shared->slot_first, shared->slot_last,
		shared->tx_pos, shared->slot_queue_recycle);
	vchiq_dump(dump_context, buf, len + 1);

	len = snprintf(buf, sizeof(buf),
		"    Slots claimed:");
	vchiq_dump(dump_context, buf, len + 1);

	for (i = shared->slot_first; i <= shared->slot_last; i++) {
		VCHIQ_SLOT_INFO_T slot_info = *SLOT_INFO_FROM_INDEX(state, i);
		if (slot_info.use_count != slot_info.release_count) {
			len = snprintf(buf, sizeof(buf),
				"      %d: %d/%d", i, slot_info.use_count,
				slot_info.release_count);
			vchiq_dump(dump_context, buf, len + 1);
		}
	}

	for (i = 1; i < shared->debug[DEBUG_ENTRIES]; i++) {
		len = snprintf(buf, sizeof(buf), "    DEBUG: %s = %d(%x)",
			debug_names[i], shared->debug[i], shared->debug[i]);
		vchiq_dump(dump_context, buf, len + 1);
	}
}

void
vchiq_dump_state(void *dump_context, VCHIQ_STATE_T *state)
{
	char buf[80];
	int len;
	int i;

	len = snprintf(buf, sizeof(buf), "State %d: %s", state->id,
		conn_state_names[state->conn_state]);
	vchiq_dump(dump_context, buf, len + 1);

	len = snprintf(buf, sizeof(buf),
		"  tx_pos=%x(@%x), rx_pos=%x(@%x)",
		state->local->tx_pos,
		(uint32_t)state->tx_data +
			(state->local_tx_pos & VCHIQ_SLOT_MASK),
		state->rx_pos,
		(uint32_t)state->rx_data +
			(state->rx_pos & VCHIQ_SLOT_MASK));
	vchiq_dump(dump_context, buf, len + 1);

	len = snprintf(buf, sizeof(buf),
		"  Version: %d (min %d)",
		VCHIQ_VERSION, VCHIQ_VERSION_MIN);
	vchiq_dump(dump_context, buf, len + 1);

	if (VCHIQ_ENABLE_STATS) {
		len = snprintf(buf, sizeof(buf),
			"  Stats: ctrl_tx_count=%d, ctrl_rx_count=%d, "
			"error_count=%d",
			state->stats.ctrl_tx_count, state->stats.ctrl_rx_count,
			state->stats.slot_stalls);
		vchiq_dump(dump_context, buf, len + 1);
	}

	len = snprintf(buf, sizeof(buf),
		"  Slots: %d available, %d recyclable, %d stalls",
		state->slot_queue_available -
			SLOT_QUEUE_INDEX_FROM_POS(state->local_tx_pos),
		state->local->slot_queue_recycle - state->slot_queue_available,
		state->stats.slot_stalls);
	vchiq_dump(dump_context, buf, len + 1);

	vchiq_dump_platform_state(dump_context);

	vchiq_dump_shared_state(dump_context, state, state->local, "Local");
	vchiq_dump_shared_state(dump_context, state, state->remote, "Remote");

	vchiq_dump_platform_instances(dump_context);

	for (i = 0; i < state->unused_service; i++) {
		VCHIQ_SERVICE_T *service = state->services[i];

		if (service && (service->srvstate != VCHIQ_SRVSTATE_FREE))
			vchiq_dump_service_state(dump_context, service);
	}
}

void
vchiq_dump_service_state(void *dump_context, VCHIQ_SERVICE_T *service)
{
	char buf[80];
	int len;

	len = snprintf(buf, sizeof(buf), "Service %d: %s",
		service->localport, srvstate_names[service->srvstate]);

	if (service->srvstate != VCHIQ_SRVSTATE_FREE) {
		char remoteport[30];
		VCHIQ_SERVICE_QUOTA_T *service_quota =
			&service->state->service_quotas[service->localport];
		int fourcc = service->base.fourcc;
		if (service->remoteport != VCHIQ_PORT_FREE) {
			int len2 = snprintf(remoteport, sizeof(remoteport),
				"%d", service->remoteport);
			if (service->public_fourcc != VCHIQ_FOURCC_INVALID)
				snprintf(remoteport + len2,
					sizeof(remoteport) - len2,
					" (client %x)", service->client_id);
		} else
			strcpy(remoteport, "n/a");

		len += snprintf(buf + len, sizeof(buf) - len,
			" '%c%c%c%c' remote %s (msg use %d/%d, slot use %d/%d)",
			VCHIQ_FOURCC_AS_4CHARS(fourcc),
			remoteport,
			service_quota->message_use_count,
			service_quota->message_quota,
			service_quota->slot_use_count,
			service_quota->slot_quota);

		if (VCHIQ_ENABLE_STATS) {
			vchiq_dump(dump_context, buf, len + 1);

			len = snprintf(buf, sizeof(buf),
				"  Ctrl: tx_count=%d, tx_bytes=%llu, "
				"rx_count=%d, rx_bytes=%llu",
				service->stats.ctrl_tx_count,
				service->stats.ctrl_tx_bytes,
				service->stats.ctrl_rx_count,
				service->stats.ctrl_rx_bytes);
			vchiq_dump(dump_context, buf, len + 1);

			len = snprintf(buf, sizeof(buf),
				"  Bulk: tx_count=%d, tx_bytes=%llu, "
				"rx_count=%d, rx_bytes=%llu",
				service->stats.bulk_tx_count,
				service->stats.bulk_tx_bytes,
				service->stats.bulk_rx_count,
				service->stats.bulk_rx_bytes);
			vchiq_dump(dump_context, buf, len + 1);

			len = snprintf(buf, sizeof(buf),
				"  %d quota stalls, %d slot stalls, "
				"%d bulk stalls, %d aborted, %d errors",
				service->stats.quota_stalls,
				service->stats.slot_stalls,
				service->stats.bulk_stalls,
				service->stats.bulk_aborted_count,
				service->stats.error_count);
		 }
	}

	vchiq_dump(dump_context, buf, len + 1);

	vchiq_dump_platform_service_state(dump_context, service);
}


void
vchiq_loud_error_header(void)
{
	vchiq_log_error(vchiq_core_log_level,
		"============================================================"
		"================");
	vchiq_log_error(vchiq_core_log_level,
		"============================================================"
		"================");
	vchiq_log_error(vchiq_core_log_level, "=====");
}

void
vchiq_loud_error_footer(void)
{
	vchiq_log_error(vchiq_core_log_level, "=====");
	vchiq_log_error(vchiq_core_log_level,
		"============================================================"
		"================");
	vchiq_log_error(vchiq_core_log_level,
		"============================================================"
		"================");
}


VCHIQ_STATUS_T vchiq_send_remote_use(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_RETRY;
	if (state->conn_state != VCHIQ_CONNSTATE_DISCONNECTED)
		status = queue_message(state, NULL,
			VCHIQ_MAKE_MSG(VCHIQ_MSG_REMOTE_USE, 0, 0),
			NULL, 0, 0, 0);
	return status;
}

VCHIQ_STATUS_T vchiq_send_remote_release(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_RETRY;
	if (state->conn_state != VCHIQ_CONNSTATE_DISCONNECTED)
		status = queue_message(state, NULL,
			VCHIQ_MAKE_MSG(VCHIQ_MSG_REMOTE_RELEASE, 0, 0),
			NULL, 0, 0, 0);
	return status;
}

VCHIQ_STATUS_T vchiq_send_remote_use_active(VCHIQ_STATE_T *state)
{
	VCHIQ_STATUS_T status = VCHIQ_RETRY;
	if (state->conn_state != VCHIQ_CONNSTATE_DISCONNECTED)
		status = queue_message(state, NULL,
			VCHIQ_MAKE_MSG(VCHIQ_MSG_REMOTE_USE_ACTIVE, 0, 0),
			NULL, 0, 0, 0);
	return status;
}

void vchiq_log_dump_mem(const char *label, uint32_t addr, const void *voidMem,
	size_t numBytes)
{
	const uint8_t  *mem = (const uint8_t *)voidMem;
	size_t          offset;
	char            lineBuf[100];
	char           *s;

	while (numBytes > 0) {
		s = lineBuf;

		for (offset = 0; offset < 16; offset++) {
			if (offset < numBytes)
				s += snprintf(s, 4, "%02x ", mem[offset]);
			else
				s += snprintf(s, 4, "   ");
		}

		for (offset = 0; offset < 16; offset++) {
			if (offset < numBytes) {
				uint8_t ch = mem[offset];

				if ((ch < ' ') || (ch > '~'))
					ch = '.';
				*s++ = (char)ch;
			}
		}
		*s++ = '\0';

		if ((label != NULL) && (*label != '\0'))
			vchiq_log_trace(VCHIQ_LOG_TRACE,
				"%s: %08x: %s", label, addr, lineBuf);
		else
			vchiq_log_trace(VCHIQ_LOG_TRACE,
				"%08x: %s", addr, lineBuf);

		addr += 16;
		mem += 16;
		if (numBytes > 16)
			numBytes -= 16;
		else
			numBytes = 0;
	}
}
