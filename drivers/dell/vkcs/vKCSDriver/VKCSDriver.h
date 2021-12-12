/** vKCS states */
#define VKCS_STATE_WAIT_MESSAGE        0x00
#define VKCS_STATE_PROCESS_DATA         0x01
#define VKCS_STATE_DELIVER_MESSAGE    0x02
#define VKCS_STATE_ERROR                        0x03

/** vKCS ioctl option */
#define VKCS_WRITE_MESSAGE 0x00
#define VKCS_READ_MESSAGE   0x01
#define VKCS_GET_REGISTERS   0x02
#define VKCS_SET_REGISTERS   0x04
#define VKCS_WRITE_READ_MESSAGE 0x05


#define VKCS_SUCCESS                0
#define VKCS_ERROR                  1
#define VKCS_BMC_NOT_EXISTENCE      2
#define VKCS_BUSY_TIMEOUT           3



