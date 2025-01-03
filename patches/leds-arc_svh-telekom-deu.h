#ifndef LED_ARC_DT
#define LED_ARC_DT
#include "leds-arc_common.h"
static int startup_anim(void *unused)
{
	int i;
	while(1)
	{
		for(i=0;i<4;i++)
		{
			LED_FUNCTION(red, i, 51); LED_FUNCTION(green, i, 51); LED_FUNCTION(blue, i, 51);
		}
		if(kthread_should_stop()) do_exit(0);
		msleep(600);
		for(i=0;i<4;i++)
		{
			LED_FUNCTION(red, i, 128); LED_FUNCTION(green, i, 128); LED_FUNCTION(blue, i, 128);
		}
		if(kthread_should_stop()) do_exit(0);
		msleep(600);
		for(i=0;i<4;i++)
		{
			LED_FUNCTION(red, i, 204); LED_FUNCTION(green, i, 204); LED_FUNCTION(blue, i, 204);
		}	
		if(kthread_should_stop()) do_exit(0);
		msleep(800);
	}
}
#endif
