
#ifndef __MT53XX_SC_H__
#define __MT53XX_SC_H__

#define SC_FCT_0 0                // Set RR
#define SC_FCT_1 1                // Set Other
#define SC_FCT_SETAFFINITY 2      // Set CPU affinity
#define SC_FCT_100 100

typedef struct
{
    int i4Fct;
    int i4Arg0;
    int i4Arg1;
} SC_CB_T;

#endif /* __MT53XX_SC_H__ */
