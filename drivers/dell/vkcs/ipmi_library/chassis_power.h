
/*
 Define API status
 SEND_CMD_TO_BMC_FAILED: vkcs channle can not work correctly
 CMD_FAILED: ipmi command : completion code is not 0 
 CMD_SUCCESS: ipmi command : completion code is  0 
*/
#define SEND_CMD_TO_BMC_FAILED -1
#define CMD_FAILED -2
#define CMD_SUCCESS 0

/*
 use for chassis_power_status() 
 if chassis_power_status() return CMD_SUCCESS
 the function also return power status by parameter "status" 
 currently, we only support on/off status
*/
#define POWER_STATUS_ON                0x01
#define POWER_STATUS_OFF               0x00


int chassis_power_down(void);
int chassis_power_up(void);
int chassis_power_cycle(void);
int chassis_power_hard_reset(void);
int chassis_power_status(unsigned long *status);


