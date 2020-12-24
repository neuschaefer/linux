
#include "IPMIRaw.h"
#include "chassis_power.h"

//#define DBG 1

int chassis_power_down(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,0};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie) && 3 > sRepSzie)
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_FAILED;
}

int chassis_power_up(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,1};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie) && 3 > sRepSzie)
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_FAILED;
}

int chassis_power_cycle(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,2};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie) && 3 > sRepSzie)
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_FAILED;
}

int chassis_power_hard_reset(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,3};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie) && 3 > sRepSzie)
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_FAILED;
}

int chassis_power_status(unsigned long *status)
{
    unsigned char request_buffer[]={0x20,0,0,1};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie) && 3 > sRepSzie)
    {
        return SEND_CMD_TO_BMC_FAILED;
    }

    if(0 == response_buffer[2])
    {
    	*status = response_buffer[3] & POWER_STATUS_ON;
    	return CMD_SUCCESS;
    }
    
    return CMD_FAILED;
}

#if DBG

int main(void)
{
	unsigned long status=0;
	printf("chassis_power_down()=%d \n",chassis_power_down());
	sleep(1);
	printf("chassis_power_status()=%d status=%d\n",chassis_power_status(&status), status);
	sleep(1);
	printf("chassis_power_cycle()=%d \n",chassis_power_cycle());
	sleep(1);
	printf("chassis_power_hard_reset()=%d \n",chassis_power_hard_reset());
	sleep(1);
	printf("chassis_power_up()=%d \n",chassis_power_up());
}

#endif
