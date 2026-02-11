#ifndef _LINUX_ROKU_H_
#define _LINUX_ROKU_H_ 1

#define ROKU_PID(a,b) (((a)<<8)|(b))

static inline int Roku_IsOUI6(char const* serial)
{
    int pid = ROKU_PID(serial[0], serial[1]);
    int bom = serial[2];

    switch (pid) {
        /* Jackson */
        case ROKU_PID('1', 'E'):
        case ROKU_PID('1', 'H'):
            return (bom >= 'L');
        /* Windsor */
        case ROKU_PID('1', 'M'):
            return (bom >= 'M');
        /* Sugarland */
        case ROKU_PID('2', 'L'):
        case ROKU_PID('2', 'M'):
            return (bom >= 'A');
        /* Austin NO ANGRY BIRDS */
        case ROKU_PID('4', '1'):
        case ROKU_PID('4', '2'):
            return (bom >= '2');
        /* Sky Italia/Deutschland */
        case ROKU_PID('4', '8'):
        case ROKU_PID('4', '9'):
            return (bom >= '3');
        /* Mustang 1/2 (US and ROW), Austin UK, WW Roku 2/3 */
        case ROKU_PID('4', 'A'):
        case ROKU_PID('4', 'E'):
        case ROKU_PID('4', 'C'):
        case ROKU_PID('4', 'D'):
        case ROKU_PID('5', 'F'):
        case ROKU_PID('5', 'G'):
        case ROKU_PID('5', 'H'):
        case ROKU_PID('5', 'J'):
        case ROKU_PID('5', 'K'):
            return 1;
        default:
            return 0;
    }
}

static inline int Roku_IsOUI5(char const* serial)
{
    int pid = ROKU_PID(serial[0], serial[1]);
    int bom = serial[2];
    
    switch (pid) {
        /* Austin */
        case ROKU_PID('1', 'G'):
        case ROKU_PID('1', 'W'):
            return (bom >= 'U');
        /* Jackson */
        case ROKU_PID('1', 'E'):
        case ROKU_PID('1', 'H'):
            return (bom >= 'J');
        /* Windsor */
        case ROKU_PID('1', 'M'):
            return (bom >= 'L');
        /* Tyler */
        case ROKU_PID('1', 'P'):
        case ROKU_PID('1', 'R'):
        case ROKU_PID('1', 'S'):
        case ROKU_PID('1', 'T'):
        case ROKU_PID('1', 'X'):
        case ROKU_PID('2', 'K'):
            return (bom >= 'G');
        /* Sugarland */
        case ROKU_PID('2', 'L'):
        case ROKU_PID('2', 'M'):
            return (bom >= '6');
        /* Victoria */
        case ROKU_PID('3', '2'):
            return (bom >= '4' && bom != 'S');
        /* Austin NO ANGRY BIRDS */
        case ROKU_PID('4', '1'):
        case ROKU_PID('4', '2'):
            return 1;
        /* Sky Italia/Deutschland */
        case ROKU_PID('4', '8'):
        case ROKU_PID('4', '9'):
            return 1;
        default:
            return 0;
    }
}

static inline int Roku_IsOUI4(char const* serial)
{
    int pid = ROKU_PID(serial[0], serial[1]);
    int bom = serial[2];

    switch (pid) {
        /* austin */
        case ROKU_PID('1','G'):
        case ROKU_PID('1','W'):
            return (bom >= 'S');
        /* jackson */
        case ROKU_PID('1','E'):
        case ROKU_PID('1','H'):
            return (bom >= 'H');
        /* windsor */
        case ROKU_PID('1','M'):
            return (bom >= 'K');
        /* tyler */
        case ROKU_PID('1','P'):
        case ROKU_PID('1','R'):
        case ROKU_PID('1','S'):
        case ROKU_PID('1','T'):
        case ROKU_PID('1','X'):
        case ROKU_PID('2','K'):
            return (bom >= 'D');
        /* sugarland */
        case ROKU_PID('2','L'):
        case ROKU_PID('2','M'):
            return 1;
        /* victoria */
        case ROKU_PID('3','2'):
            return 1;
        default:
            return 0;
    }
}

/**
 * Get appropriate OUI to use with a given serial number.
 */
static inline void Roku_GetOUI(char const* serial, unsigned char* oui)
{
    if (Roku_IsOUI6(serial)) {
        /* OUI-6 (8/5) */
        oui[0] = 0xAC;
        oui[1] = 0x3A;
        oui[2] = 0x7A;
    } else if (Roku_IsOUI5(serial)) {
        /* OUI-5 (3/14) */
        oui[0] = 0xB0;
        oui[1] = 0xA7;
        oui[2] = 0x37;
    } else if (Roku_IsOUI4(serial)) {
        /*  OUI-4 (9/13) */
        oui[0] = 0xDC;
        oui[1] = 0x3A;
        oui[2] = 0x5E;
    } else if (serial[0] == '2' || (serial[0] == '1' && (serial[2] >= 'G' || (serial[1] >= 'P' && serial[1] <= 'X')))) {
        /* giga, paolo, jackson or austin with BOM G or higher, and every tyler:
         *  OUI-3 (9/12) */
        oui[0] = 0xB8;
        oui[1] = 0x3E;
        oui[2] = 0x59;
    } else if (serial[2] == 'A') {
        /* BOM A of anything else:
         *  OUI-1 (original Roku OUI) */
        oui[0] = 0x00;
        oui[1] = 0x0d;
        oui[2] = 0x4b;
    } else {
        /* BOM B or greater:
         *  OUI-2 (8/11) */
        oui[0] = 0xcc;
        oui[1] = 0x6d;
        oui[2] = 0xa0;
    }
}

/**
 * Decode IB data to get a serial number.
 * ib_data should point to the aes_key member of the IB.
 */
static inline void Roku_GetESN(unsigned char const* ib_data, char* serial)
{
    /*
     * Format of the IB data:
     *
     * byte: |   [6]  |   [5]  |   [4]  |   [3]  |   [2]  |   [1]  |   [0]  |
     * data: |......PP|PPP11111|22222YYY|YYMMMMMC|CCCCNNNN|NNNNNNNN|NNNNNNNN|
     *
     *    P = product ID
     *    1 = revision 1
     *    2 = revision 2
     *    Y = production year
     *    M = production month
     *    C = check char
     *    N = unit number
     */
    #undef  IB_FIELD
    #define IB_FIELD(index, lobit, numbits) \
            ((ib_data[index] >> (lobit)) & ((1<<(numbits))-1))

    unsigned int last6   = ib_data[0] | (ib_data[1] << 8) | (IB_FIELD(2,0,4) << 16);
    unsigned char check  = IB_FIELD(2,4,4) | (IB_FIELD(3,0,1) << 4);
    unsigned char month  = IB_FIELD(3,1,5);
    unsigned char year   = IB_FIELD(3,6,2) | (IB_FIELD(4,0,3) << 2);
    unsigned char rev2   = IB_FIELD(4,3,5);
    unsigned char rev1   = IB_FIELD(5,0,5);
    unsigned char prefix = IB_FIELD(5,5,3) | (IB_FIELD(6,0,2) << 3);

    static const unsigned char base31tab[] = {
        '0','1','2','3','4','5','6','7','8','9',
        'A','C','D','E','F','G','H','J','K','L',
        'M','N','P','R','S','T','U','V','W','X',
        'Y','Z'
    };

    if (last6 > 999999)
        last6 = 999999;

    sprintf(serial, "%c%c%c%c%c%c%06u",
        base31tab[prefix], base31tab[rev1], base31tab[rev2],
        base31tab[year], base31tab[month], base31tab[check], last6);
}

#ifdef ROKU_OUI_TEST_PROGRAM
/* Test program */
#include <stdio.h>
int main(int argc, char** argv)
{
    int arg;
    for (arg = 1; arg < argc; arg++) {
        unsigned char oui[3];
        Roku_GetOUI(argv[arg], oui);
        printf("%s  %02x:%02x:%02x\n", argv[arg], oui[0], oui[1], oui[2]);
    }
    return 0;
}
#endif

#endif /* _LINUX_ROKU_H_ */
