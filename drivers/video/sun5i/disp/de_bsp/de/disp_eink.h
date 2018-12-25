#include "disp_eink_config.h"
#include "disp_eink_data.h"
#include "dev_eink_awf.h"


//the trigger for awf waveform file supported
//#define AWF_WAVEFORM_SUPPORTED 
//#define IS_DEBUG

#define	WF_FB_ID                1
#define	SCALER_OUT_FB_ID        2

#define	EINK_8BPP_FRAME_NUM		2
#define	EINK_WAV_BUFFER_NUM		8//24

#define	EINK_WAV_DU_PATH				"/system/du.bin"
#define	EINK_WAV_SHORT_DU_PATH			"/system/short_du.bin"
#define	EINK_WAV_GC16_PATH				"/system/gc16.bin"
#define	EINK_WAV_SHORT_GC16_LOCAL_PATH	"/system/short_gc16.bin"
#define	EINK_WAV_COMMON_PATH			"/system/local.bin"
#define	EINK_AWF_WAV_PATH				"/system/default.awf"

#define EPDK_OK         0
#define EPDK_FAIL       (-1)

#if 0
#define __EINK_INF(msg...)				{printk(KERN_WARNING "[EINK_INF] line:%d-",__LINE__);printk(msg);}
#define __EINK_MSG(msg...)				{printk(KERN_WARNING "[EINK_MSG] line:%d-",__LINE__);printk(msg);}	
#define __EINK_WRN(msg...)				{printk(KERN_WARNING "[EINK_WRN] line:%d-",__LINE__);printk(msg);}	
#else
#define __EINK_INF(msg...) 
#define __EINK_MSG(msg...) 
#define __EINK_WRN(msg...) 				{printk(KERN_WARNING "[EINK][%s] line:%d::",__func__,__LINE__);printk(msg);}
#define __EINK_ERR(msg...) 				{printk(KERN_WARNING "[EINK][%s] line:%d::",__func__,__LINE__);printk(msg);}
#endif

#ifdef IS_DEBUG
#define __EINK_DBG(msg...)				{printk(KERN_WARNING "[EINK][%s] line:%d::",__func__,__LINE__);printk(msg);}	
#define __EINK_LOG(msg...)				{printk(KERN_WARNING "[EINK][%s] line:%d::",__func__,__LINE__);printk(msg);}
#else
#define __EINK_DBG(msg...)
#define __EINK_LOG(msg...)
#endif



typedef struct file ES_FILE;

typedef	struct {
	__s32 		frame_valid;				// frame valid flag
	__u8 *		frame_address;				// 8BPP frame address
}Eink_frame_t;

typedef	struct {
	__s32		wav_index;					// wav index
	__u8 *		wav_address;				// wav address
}Eink_wav_t;

#define NUM_BUFFER_SLOTS 8

typedef enum  {
	FREE = 0, 
	DEQUEUED = 1, 
	QUEUED = 2,
}BufState;

typedef struct {
	__u8* buffer;
	BufState bufState;
	__s64 use_index;
	__s64 alloc_index;
	__s32 total_frame;            //total_frame	
	__s32 current_frame;		
	__s32 wav_compose_index;		// wav compose index
	__s32 wav_refresh_index;		// wav refresh index	
	__eink_update_mode flushMode;
} BufSlot;


typedef struct {
	__s64 useCount;
	__s64 allocCount;
	__s32 bufCount;						/*total buffer number*/
	__s32 curBufIndex;					/*current used buffer for showing on Eink Screen*/
	BufSlot slots[NUM_BUFFER_SLOTS];	/*slots is array of slots*/	
	int		fifo[NUM_BUFFER_SLOTS];
} Eink_BufQueue;

typedef struct {
	__s32			frame_width;			// 8BPP buffer width
	__s32			frame_height;			// 8BPP buffer height
	__s32			frame_pixelfmt;			// 8BPP buffer pixelfmt
	__u8 *			u_address;				// ARGB->8BPP U address
	__u8 *			v_address;				// ARGB->8BPP Y address
	
	__u8 *			wav_last_frame;
	__u8 *			wav_form_index;
	
	__s32 			temperature;            //temperature
	__s32 *			pframe_data;            //pframe_data
	
	__s32			wav_width;				// wav width
	__s32			wav_height;				// wav height
	
	Eink_wav_t		wav_buffer[EINK_WAV_BUFFER_NUM];	// wav cache buffer
    	
	struct semaphore 	wav_compose_sem[1];		// wav compose sem
	struct task_struct	*wav_compose_task;		// wav compose task

	struct semaphore 	wav_refresh_sem[1];		// wav refresh sem
	struct task_struct	*wav_refresh_task;		// wav refresh task

	//20130402, yuanlian
	//add this internal update task for short du ghost update
	//when short du will cause badly effect for eink, need 
	//to be update with short gc16 local update automatic after
	struct semaphore 	internal_update_sem[1];	//semaphore of internal update, default is locking
	struct task_struct	*internal_update_task;		
	//the flag for doing internal update
	//1->do internal update, 0->nothing to do; default is 0
	//will update to 1 when after short du update, 
	//and reset to 0 if get a Eink_update() or finished un-short du update.
	__s32 				internal_update;
	//storage the update mode of the last frame
	__eink_update_mode	last_update_mode;
    

    Eink_stby_act_e     stby_act;

    struct semaphore 	stby_act_lock[1];
    __s32               update_status;
    
    struct semaphore 	power_off_sem[1];		// power off sem    
    struct semaphore 	buffer_lock_sem[1];

	__u32* 			wav_du_data;	
	__u32* 			wav_short_du_data;	
	__u32* 			wav_gc16_data;
	__u32* 			wav_short_gc16_local_data;
	__u32* 			wav_common_data;


#ifdef AWF_WAVEFORM_SUPPORTED
	char* 			p_wf_buffer;           //全文waveform指针
	__u8 wf_temp_area_tbl[C_TEMP_TBL_SIZE];
	__u32* p_init_wf;            //指向INIT mode waveform 数据块
	__u32* p_gc16_wf;            //指向GC16 waveform数据，
	__u32* p_gc16_local_wf;
	__u32* p_gc4_wf;             //指向GC4 waveform 数据.
	__u32* p_gc4_local_wf;       //指向局部GC4 waveform 数据.   
	__u32* p_du_wf;              //指向DU模式下waveform数据，
	__u32* p_A2_wf;              //指向A2模式下waveform数据，从bin文件中读取
#endif

	Eink_BufQueue bufQueue;


    __u8            eink_moudule_type;
    __u8            eink_version_type;
    __u8            eink_ctrl_data_type;

    __u8            *eink_init_T_tbl;
    eink_init_wf_t  *eink_init_mode_wf;
    __u32           (*eink_ctrl_tbl_GC16)[258];

    __u32           wf_len_divider;

    int             orientation;
}Disp_eink_t;

__s32 Disp_eink_init(__u32 sel, __eink_init_para* para);

__s32 Disp_eink_exit(__u32 sel);

__s32 Disp_eink_update(__u32 sel, __u32 fb_id, __u32 mode);

__s32 Disp_eink_set_mode(__u32 sel, __eink_update_mode mode);

__s32 Disp_eink_set_back_mode(__u32 sel, __eink_update_mode mode);

__s32 Disp_eink_set_temperature(__u32 sel, __u32 temperature);

void eink_refresh_wav_form_1(unsigned long);

void Disp_eink_set_stby_act(Eink_stby_act_e act);

Eink_stby_act_e Disp_eink_get_stby_act(void);

void Disp_eink_stby_act_lock(void);
void Disp_eink_stby_act_unlock(void);

__s32 Disp_eink_set_orientation(__u32 sel, __u32 fb_id, __u32 o);

