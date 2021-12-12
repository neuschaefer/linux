#include <stdio.h>
#include <string.h>
#include "IPMIRaw.h"




int char_to_int(char char_a)
{
 
  switch (char_a)
   {
            case '0':
                return 0;
            case '1':
                return 1;
            case '2':
                return 2;
            case '3':
                return 3;
            case '4':
                return 4;
            case '5':
                return 5;
            case '6':
                return 6;
            case '7':
                return 7;
            case '8':
                return 8;
            case '9':
                return 9;
            case 'A':
                return 10;
            case 'B':
                return 11;
            case 'C':
                return 12;  
            case 'D':
                return 13;    
            case 'E':
                return 14;
            case 'F':
                return 15;
            case 'a':
                return 10;
            case 'b':
                return 11;
            case 'c':
                return 12;  
            case 'd':
                return 13;    
            case 'e':
                return 14;
            case 'f':
                return 15;              
            default:     
                return ERROR_RAW_DATA;       
    }                         
}   

int str_to_hex(char char_a, char char_b, int *value)             
{
 int int_a;
 int int_b;   
 int decimal_value;
   
  int_a = char_to_int(char_a);
  int_b = char_to_int(char_b);
  
  if((int_a==ERROR_RAW_DATA)||(int_b==ERROR_RAW_DATA))
  {     
        return STATUS_FAIL;
  }      
  
  decimal_value = int_a*16+int_b;
  *value = decimal_value;
  
  return STATUS_OK;
} 

           
     

int main(int argc, char* argv[])
{
  
    int iIpmiCommandlength = 41;
    int ipmi_cmd_length;
    char request_ipmi_cmd[4];
    char char_a;
    char char_b;
    int decimal_value;
    unsigned char request_ipmi_cmd_buffer[50];
    unsigned char response_ipmi_cmd_buffer[50];  
    int res_data_length;
    int i;
    int STATUS;
    int SEND_CMD_STATUS;
        
    
    if((0 == argc)||(argc<5))
    {
        printf("useg: Get Device ID \nIPMICmd 0x20 0x06 0x00 0x01\n");
        return -1;
    }

    if(argc>iIpmiCommandlength)
    {
        printf("IPMI command length error\n");
        return -1;
    }

    ipmi_cmd_length = 0;
    
    for(i = 1 ; i < argc;i++)
    {
         ipmi_cmd_length++;
        if(strlen(argv[i])!=4)
        {                
             printf("Cmmand formate error [0x00]\n");
             return -1;
        }
        strncpy(request_ipmi_cmd, argv[i], 4);
        if((request_ipmi_cmd[0] != '0') || (request_ipmi_cmd[1] != 'x'))
        {
             printf("[0x..] formate error!\n");
             return -1;
        }

        char_a = request_ipmi_cmd[2];
        char_b = request_ipmi_cmd[3];
        STATUS = str_to_hex(char_a,char_b, &decimal_value);
        if(STATUS == STATUS_FAIL)
        {    
            
            printf(" Not a number !!\n");
            return -1;
        }    
        request_ipmi_cmd_buffer[ipmi_cmd_length-1]= decimal_value;

    }
   
    if((request_ipmi_cmd_buffer[0]&0x01) == 0x01)
    {
          printf("Raw data error \n");
          return -1;
    }
    else
    {
        SEND_CMD_STATUS = send_ipmi_cmd(request_ipmi_cmd_buffer,&ipmi_cmd_length,response_ipmi_cmd_buffer,&res_data_length );                                                        
        if(SEND_CMD_STATUS == STATUS_FAIL)
        {
            printf("Send IPMI command fail !!Press enter to continue\n");
         }    
        
        printf("The response data is: \n");
        for(i=0 ; i < res_data_length ; i++)
        {    
            printf("0x%02x ",response_ipmi_cmd_buffer[i]);
        } 
        printf("\n");
    }

    return 0;
}

