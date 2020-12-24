/** method */
#define AESS_METHOD_VKCS 0x00
#define AESS_METHOD_IPC 0x01

/** error code */
#define AESS_DRIVER_SUCCESS 0x00
#define AESS_DRIVER_OPEN_ERROR 0x01
#define AESS_LENGTH_ERROR 0x02
#define AESS_POINTER_ERROR 0x03
#define AESS_DRIVER_BUSY 0x04
#define AESS_METHOD_ERROR 0x05
#define AESS_BMC_RESPONSE_TIMEOUT 0x06
#define AESS_DRIVER_WRITE_BUSY 0x07
#define AESS_DRIVER_READ_BUSY 0x08

#define FALSE 1
#define TRUE 0
#define STATUS_OK 0
#define STATUS_FAIL 1
#define ERROR_RAW_DATA 255

/** api */
int AESSIPMIMessage(int * iFd,int iMethod,int *iCommandLength,unsigned char * pucCommandBuffer,int* iResponseLength,unsigned char * pucResponseBuffer);
int AESSOpenNode(int *iFd);
int AESSCloseNode(int *iFd);

int send_ipmi_cmd(unsigned char *send_data,int *send_data_length,unsigned char *res_buffer,int *res_data_length);
