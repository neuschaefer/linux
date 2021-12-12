

#include "foo.h"

#define DBG 1

int chassis_power_down(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,0};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie))
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(3 < sRepSzie && 0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_NO_SUCCESS;
}

int chassis_power_up(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,1};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie))
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(3 < sRepSzie && 0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_NO_SUCCESS;
}

int chassis_power_cycle(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,2};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie))
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(3 < sRepSzie && 0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_NO_SUCCESS;
}

int chassis_power_hard_reset(void)
{
    unsigned char request_buffer[]={0x20,0,0,2,3};
    unsigned char response_buffer[50];  
    int sReqSzie=sizeof(request_buffer);
    int sRepSzie=0;

    if(STATUS_FAIL == send_ipmi_cmd(request_buffer,&sReqSzie,response_buffer,&sRepSzie))
    {
        return SEND_CMD_TO_BMC_FAILED;
    }
    if(3 < sRepSzie && 0 == response_buffer[2])
    {
    	return CMD_SUCCESS;
    }
    
    return CMD_NO_SUCCESS;
}



#if DBG

int main(void)
{
	printf("chassis_power_down()=%d \n",chassis_power_down());
	printf("chassis_power_up()=%d \n",chassis_power_up());
	printf("chassis_power_cycle()=%d \n",chassis_power_cycle());
	printf("chassis_power_hard_reset()=%d \n",chassis_power_hard_reset());
	
}

#endif
