/*
 * linux/include/asm/arch/flash_chars.h
 *
 *  Copyright (C) 2007 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */

#ifndef FLASH_CHARS_H
#define FLASH_CHARS_H

struct sflash_dev {
  char *name;
  int device_id;
  int chipshift;
  int pageshift;
  int pagesize;
  unsigned long sectorsize;
  unsigned long size_in_bytes;
};

/**************************************************/
/* STW serial flash device ID M25 series*/
#define SFLASH_DEVID_M25P16             0x00152020
#define SFLASH_DEVID_M25P32             0x00162020
#define SFLASH_DEVID_M25P64             0x00172020


#define SFLASH_DEVID_M25P05             0x00102020
#define SFLASH_DEVID_M25P10             0x00112020
#define SFLASH_DEVID_M25P20             0x00122020
#define SFLASH_DEVID_M25P40             0x00132020
#define SFLASH_DEVID_M25P80             0x00142020

/* STW serial flash device ID M45 series */
#define SFLASH_DEVID_M45P10             0x00114020
#define SFLASH_DEVID_M45P20             0x00124020
#define SFLASH_DEVID_M45P40             0x00134020
#define SFLASH_DEVID_M45P80             0x00144020
/***** End of ST series memories ************************/


/*** serial flash device Atmel series  */
#define SFLASH_DEVID_AT25F512          0x0065001F
#define SFLASH_DEVID_AT25F1024         0x0060001F
#define SFLASH_DEVID_AT25F2048         0x0063001F
#define SFLASH_DEVID_AT25F4096         0x0064001F
#define SFLASH_DEVID_AT25FS040         0x0004661F


/*** serial flash device Spansion series  */
#define SFLASH_DEVID_S25FL004  0x00120201
#define SFLASH_DEVID_S25FL008  0x00130201
#define SFLASH_DEVID_S25FL016  0x00140201
#define SFLASH_DEVID_S25FL032  0x00150201
#define SFLASH_DEVID_S25FL064  0x00160201



/*** serial flash device Macronix series  */
#define SFLASH_DEVID_MX25L1005      0x001120C2
#define SFLASH_DEVID_MX25L1605      0x001520C2
#define SFLASH_DEVID_MX25L1605A     0x001520C2
#define SFLASH_DEVID_MX25L2005      0x001220C2
#define SFLASH_DEVID_MX25L3205      0x001620C2
#define SFLASH_DEVID_MX25L3205A     0x001620C2
#define SFLASH_DEVID_MX25L4005      0x001320C2
#define SFLASH_DEVID_MX25L4005A     0x001320C2

#define SFLASH_DEVID_MX25L512       0x001020C2
#define SFLASH_DEVID_MX25L6405      0x001720C2
#define SFLASH_DEVID_MX25L8005      0x001420C2



/*** serial flash device SST series    */
#define SFLASH_DEVID_SST25LF020  0x004300BF
#define SFLASH_DEVID_SST25LF040  0x004400BF
#define SFLASH_DEVID_SST25LF080  0x008000BF
#define SFLASH_DEVID_SST25VF010  0x004900BF
#define SFLASH_DEVID_SST25VF016  0x004125BF
#define SFLASH_DEVID_SST25VF040  0x008D25BF
#define SFLASH_DEVID_SST25VF080  0x008E25BF
#define SFLASH_DEVID_SST25VF512  0x004800BF

#define SFLASH_DEVID_NX25F011           0x000B2020


static struct sflash_dev sflash_ids[] = {

  {"ST M25P16", SFLASH_DEVID_M25P16,  
                21, 0x8, 0x100, 0x10000,0x200000},
  {"ST M25P32", SFLASH_DEVID_M25P32,
                22, 0x8, 0x100, 0x10000,0x400000},
  {"ST M25P64", SFLASH_DEVID_M25P64, 
                23, 0x8, 0x100, 0x10000,0x800000},
  {"ST M25P05", SFLASH_DEVID_M25P05, 
                16, 0x7, 0x80, 0x8000,0x10000},
  {"ST M25P10", SFLASH_DEVID_M25P10,
                17, 0x7, 0x80, 0x8000,0x20000},
  {"ST M25P20", SFLASH_DEVID_M25P20,
                18, 0x8, 0x100, 0x10000,0x40000},
  {"ST M25P40", SFLASH_DEVID_M25P40,
                19, 0x8, 0x100, 0x10000,0x80000},
  {"ST M25P80", SFLASH_DEVID_M25P80,
                20, 0x8, 0x100, 0x10000,0x100000},
  {"ST M45PE10", SFLASH_DEVID_M45P10,  
                 17, 0x8, 0x100, 0x10000,0x20000},
  {"ST M45PE20", SFLASH_DEVID_M45P20,
                 18, 0x8, 0x100, 0x10000,0x40000},
  {"ST M45PE40", SFLASH_DEVID_M45P40, 
                 19, 0x8, 0x100, 0x10000,0x80000},
  {"ST M45PE80", SFLASH_DEVID_M45P80,
                 20, 0x8, 0x100, 0x10000,0x100000},


  {"SPANSION S25FL004", SFLASH_DEVID_S25FL004, 
                        19, 0x8, 0x100, 0x10000,0x80000},
  {"SPANSION S25FL008", SFLASH_DEVID_S25FL008, 
                        20, 0x8, 0x100, 0x10000,0x100000},
  {"SPANSION S25FL016", SFLASH_DEVID_S25FL016, 
                        21, 0x8, 0x100, 0x10000,0x200000},
  {"SPANSION S25FL032", SFLASH_DEVID_S25FL032, 
                        22, 0x8, 0x100, 0x10000,0x400000},
  {"SPANSION S25FL064", SFLASH_DEVID_S25FL064,
                        23, 0x8, 0x100, 0x10000,0x800000},


  {"SST 25LF020", SFLASH_DEVID_SST25LF020,
                  18, -1, -1, 0x8000,0x40000},
  {"SST 25LF040", SFLASH_DEVID_SST25LF040,
                  19, -1, -1, 0x8000,0x80000},
  {"SST 25LF080", SFLASH_DEVID_SST25LF080,
                  20, -1, -1, 0x8000,0x100000},
  {"SST 25VF512", SFLASH_DEVID_SST25VF512, 
                  16, -1, -1, 0x8000,0x8000},
  {"SST 25VF010", SFLASH_DEVID_SST25VF010, 
                  17, -1, -1, 0x8000,0x10000},
  {"SST 25VF040", SFLASH_DEVID_SST25VF040, 
                  19, -1, -1, 0x8000,0x40000},
  {"SST 25VF080", SFLASH_DEVID_SST25VF080, 
                  20, -1, -1, 0x8000,0x100000},
  {"SST 25VF016", SFLASH_DEVID_SST25VF016,
                  21, -1, -1, 0x8000,0x200000},


  {"ATMEL 25F512",  SFLASH_DEVID_AT25F512,  
                    16, 0x7, 0x80, 0x8000,0x10000},
  {"ATMEL 25F1024", SFLASH_DEVID_AT25F1024, 
                    17, 0x8, 0x100, 0x8000,0x20000},
  {"ATMEL 25F2048", SFLASH_DEVID_AT25F2048, 
                    18, 0x8, 0x100, 0x10000,0x40000},
  {"ATMEL 25F4096", SFLASH_DEVID_AT25F4096,
                    19, 0x8, 0x100, 0x10000,0x80000},
  {"ATMEL 25FS040", SFLASH_DEVID_AT25FS040,
                    19, 0x8, 0x100, 0x10000,0x80000},


  {"MACRONIX 25L512",  SFLASH_DEVID_MX25L512,  
                       16, 0x4, 0x010, 0x10000,0x10000},
  {"MACRONIX 25L1005", SFLASH_DEVID_MX25L1005, 
                       17, 0x4, 0x010, 0x10000,0x20000},
  {"MACRONIX 25L2005", SFLASH_DEVID_MX25L2005, 
                       18, 0x4, 0x010, 0x10000,0x40000},
  {"MACRONIX 25L4005", SFLASH_DEVID_MX25L4005, 
                       19, 0x4, 0x010, 0x10000,0x80000},
  {"MACRONIX 25L4005A",SFLASH_DEVID_MX25L4005A,
                       19, 0x4, 0x010, 0x10000,0x80000},
  {"MACRONIX 25L8005", SFLASH_DEVID_MX25L8005,
                       20, 0x4, 0x010, 0x10000,0x100000},
  {"MACRONIX 25L1605", SFLASH_DEVID_MX25L1605,
                       21, 0x8, 0x100, 0x10000,0x200000},
  {"MACRONIX 25L1605A",SFLASH_DEVID_MX25L1605A,
                       21, 0x4, 0x010, 0x10000,0x200000},
  {"MACRONIX 25L3205", SFLASH_DEVID_MX25L3205,
                       22, 0x8, 0x100, 0x10000,0x400000},
  {"MACRONIX 25L3205A",SFLASH_DEVID_MX25L3205A,
                       22, 0x8, 0x100, 0x10000,0x400000},
  {"MACRONIX 25L6405", SFLASH_DEVID_MX25L6405, 
                       23, 0x8, 0x100, 0x10000,0x800000},
  {NULL,}

};

#endif

/* End of file - flash_chars.h */
