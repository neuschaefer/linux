#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <asm/io.h>

#include "os.h"
#include "ir_if.h"
#include "u_drv_cust.h"

extern INT32 IRRC_Init(UINT32 u4PriType);
extern INT32 IRRC_WaitMtkIr(UINT32* prBtnState, UINT32* prKey, IRRX_RAW_DATA_T* rRawData);
typedef int (*mtkinp_event_notify)(unsigned type, unsigned code, int value, void * tag);

static mtkinp_event_notify ir_nfy = NULL;
static void * ir_nfy_tag = NULL;
#define UNDEFINED_KEY -1
static VOID mtkinp_ir_loop(VOID*  pv_arg)
{    
    UINT32 state;
    UINT32 key;
    IRRX_RAW_DATA_T raw;
    int value;
    unsigned type;
    unsigned code;
    
    printk("Start ir loop \n");    
    while(1)
    {
        IRRC_WaitMtkIr(&state, &key, &raw);
        type = EV_KEY;
        if(state == IRRX_COND_BTN_DOWN )
        {
            value = 1;
        }
        else
        {
            value = 0;
        }
		
        switch(key & KEY_GROUP_MASK)
        {
            case KEY_GROUP_CURSOR:
                printk("Got CURSOR MOVING\n");
                switch(key)
                {
                    case BTN_CURSOR_LEFT:
                        code = KEY_K;
                        break;
                    case BTN_CURSOR_RIGHT:
                        code = KEY_SEMICOLON;
                        break;
                    case BTN_CURSOR_UP:
                        code = KEY_H;
                        break;
                    case BTN_CURSOR_DOWN:
                        code = KEY_GRAVE;
                        break;
					default:
						code = UNDEFINED_KEY;
						break;
                }
                break;
			case KEY_GROUP_DIGIT:
                printk("Got DIGIT MOVING\n");
                switch(key)
                {
                    case BTN_DIGIT_0:
                        code = KEY_LEFTBRACE;
                        break;
                    case BTN_DIGIT_1:
                        code = KEY_Q;
                        break;
                    case BTN_DIGIT_2:
                        code = KEY_W;
                        break;
                    case BTN_DIGIT_3:
                        code = KEY_E;
                        break;
					case BTN_DIGIT_4:
                        code = KEY_R;
                        break;
                    case BTN_DIGIT_5:
                        code = KEY_T;
                        break;
                    case BTN_DIGIT_6:
                        code = KEY_Y;
                        break;
                    case BTN_DIGIT_7:
                        code = KEY_U;
                        break;
					 case BTN_DIGIT_8:
                        code = KEY_I;
                        break;
                    case BTN_DIGIT_9:
                        code = KEY_O;
                        break;
					case BTN_DIGIT_DOT:
						code = KEY_RIGHTBRACE;
						break;
					default:
						code = UNDEFINED_KEY;
						break;
                }
                break;
            case KEY_GROUP_SEL_CTRL:
                printk("Got SEL \n");
                switch(key)
                {
                    case BTN_SELECT:
                        printk("ENTER\n");
                        code = KEY_L;
                        break;
					case BTN_EXIT:
                        printk("EXIT\n");
                        code = KEY_LEFTSHIFT;
                        break;
                    default:
						code = UNDEFINED_KEY;
						break;                        
                }
                break;
				
			case KEY_GROUP_USER_DEF:
                printk("Got USER_DEF \n");
                switch(key)
                {
                    case BTN_RED:
                        code = KEY_KPASTERISK;
                        break;
					case BTN_GREEN:
						code = KEY_LEFTALT;
						break;
					case BTN_YELLOW:
						code = KEY_SPACE;
						break;
					case BTN_BLUE:
						code = KEY_CAPSLOCK;
						break;
                    default:
						code = UNDEFINED_KEY;
						break;                        
                }
                break;
				
			case KEY_GROUP_DVD_CTRL:
                printk("Got DVD \n");
                switch(key)
                {
                    case BTN_ROOT_MENU:
                        code = KEY_1;
                        break;
					case BTN_TITLE_PBC:
						code = KEY_COMMA;
						break;
					case BTN_SUB_TITLE:
						code = KEY_DOT;
						break;
					case BTN_REPEAT:
						code = KEY_SLASH;
						break;
                    default:
						code = UNDEFINED_KEY;
						break;                        
                }
                break;
				
			case KEY_GROUP_STRM_CTRL:
                printk("Got STRM \n");
                switch(key)
                {
                    case BTN_RECORD:
                        code = KEY_C;
                        break;
					case BTN_FR:
						code = KEY_V;
						break;
					case BTN_FF:
						code = KEY_B;
						break;
					case BTN_PREV:
						code = KEY_N;
						break;
					case BTN_NEXT:
						code = KEY_M;
						break;
                    default:
						code = UNDEFINED_KEY;
						break;                        
                }
                break;
				
			  case KEY_GROUP_FCT_CTRL:
                printk("Got FCT \n");
                switch(key)
                {
                    case BTN_MENU:
                        printk("MENU\n");
                        code = KEY_G;
                        break;
					case BTN_POWER:
                        printk("POWER\n");
                        code = KEY_ESC;
                        break;
					case BTN_EPG:
						code = KEY_APOSTROPHE;
						break;
					case BTN_MEM_CARD:
						code = KEY_RIGHTSHIFT;
						break;
                    default:
						code = UNDEFINED_KEY;
						break;                        
                }
                break;

			 case KEY_GROUP_AUD_CTRL:
                printk("Got AUD \n");
                switch(key)
                {
                    case BTN_VOL_UP:
                        code = KEY_A;
                        break;
					case BTN_VOL_DOWN:
                        code = KEY_F;
                        break;
					case BTN_MUTE:
                        code = KEY_D;
                        break;
					case BTN_S_EFFECT:
						code = KEY_7;
						break;
					case BTN_MTS:
						code = 13;
						break;
                    default:
						code = UNDEFINED_KEY;
						break;                        
                }
                break;	
				
			 case KEY_GROUP_PRG_CTRL:
                printk("Got PRG MOVING\n");
                switch(key)
                {
                    case BTN_PRG_UP:
                        code = KEY_ENTER;
                        break;
					case BTN_PREV_PRG:
						code = KEY_LEFTCTRL;
						break;
					case BTN_PRG_DOWN:
                        code = KEY_S;
                        break;
					case BTN_INPUT_SRC:
						code = KEY_2;
						break;
					case BTN_TIMER:
						code = KEY_3;
						break;
					case BTN_SLEEP:
						code = KEY_4;
						break;
					case BTN_ZOOM:
						code = KEY_5;
						break;
					case BTN_P_EFFECT:
						code = KEY_6;
						break;
					case BTN_ASPECT:
						code = KEY_8;
						break;
					case BTN_PIP_POP:
						code = KEY_9;
						break;
					case BTN_PIP_POS:
						code = KEY_0;
						break;
					case BTN_PIP_SIZE:
						code = KEY_MINUS;
						break;
					case BTN_CC:
						code = KEY_BACKSPACE;
						break;
					case BTN_SWAP:
						code = KEY_TAB;
						break;
					case BTN_FREEZE:
						code = KEY_P;
						break;
					case BTN_PRG_INFO:
						code = KEY_J;
						break;
					case BTN_ADD_ERASE:
						code = KEY_BACKSLASH;
						break;
					case BTN_FAVORITE:
						code = KEY_Z;
						break;
					case BTN_FAV_CH:
						code = KEY_X;
						break;
					default:
						code = UNDEFINED_KEY;
						break;
                }
                break;
            default:
				code = UNDEFINED_KEY;
				break;
        }
        
        printk("KEY 0x%x map to %d\n", key, code);
		
		if (UNDEFINED_KEY == code)
		{	
			printk ("UNDEFINED KEY\n");
		}
		else
		{
			ir_nfy(type, code, value, ir_nfy_tag);
		}
    }
}

static int mtk_drv_ir_init(mtkinp_event_notify nfy, void * tag)
{
    INT32 ret;
    HANDLE_T h_thd;
    
    ir_nfy = nfy;
    ir_nfy_tag = tag;

    ret = IRRC_Init(IRRX_RC5_TYPE);
    printk("Call IRRC_Init ret:%d\n", ret);
		x_thread_create(&h_thd, "mtkinp_ir", 4*1024, 20, mtkinp_ir_loop, 0, 0);
   	return 0; 
}

/*
 * Avoid using driver function for waiting key code, we don't
 * want to bind with driver's OS layer. Because the waitting 
 * function may use special OS functions which require thread 
 * context.
 */
struct event_dev {
    struct input_dev *input;    
};

static int mtkinp_events_cb(unsigned type, unsigned code, int value,void * dev_id)
{
    struct event_dev *edev = dev_id;
    input_event(edev->input, type, code, value);
    return 0;
}


static int mtkinp_events_setup(void)
{
    struct input_dev *input_dev;
    struct event_dev *edev;
    int ret;
    printk("*** mtkinp events probe ***\n");
    
    edev = kzalloc(sizeof(struct event_dev), GFP_KERNEL);
    input_dev = input_allocate_device();
    edev->input = input_dev;

    mtk_drv_ir_init(mtkinp_events_cb, edev);
    
        /* indicate that we generate key events */
    set_bit(EV_KEY, input_dev->evbit);
    set_bit(EV_REL, input_dev->evbit);
    set_bit(EV_ABS, input_dev->evbit);

    /* indicate that we generate *any* key event */

    bitmap_fill(input_dev->keybit, KEY_MAX);
    bitmap_fill(input_dev->relbit, REL_MAX);
    bitmap_fill(input_dev->absbit, ABS_MAX);

        
    input_dev->name = "mtkinp_events";
/*    input_dev->private = edev; */
    
    ret = input_register_device(input_dev);
    (void)ret;
    return 0;

//fail:
    kfree(edev);
    input_free_device(input_dev);
    
    return -EINVAL;
}

/*static struct platform_driver mtkinp_events_driver = {
    .probe = mtkinp_events_probe,
    .driver = {
        .name = "mtkinp_events",
    },
};*/

int mtkinp_events_init(void)
{
    //int ret;
    /*ret = platform_driver_register(&mtkinp_events_driver);*/
    
    mtkinp_events_setup();
    return 0;
}


EXPORT_SYMBOL(mtkinp_events_init);
