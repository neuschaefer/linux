RAM 0x00000000 0x10000000
{
    BOOT 0x00000000
    {
      int.o
    }

    ROLIB +0
    {
      * (+RO)
    }

    ROCODE0 +0
    {
      arm.o (+RO)
      bootp.o (+RO)
      chip_boot.o (+RO)
      chip_txt.o (+RO)
    }

    ROCODE1 +0
    {
      ddr.o (+RO)
      ddr2.o (+RO)
      ddr3.o (+RO)
    }

    ROCODE2 +0
    {
      ddr_cal0.o (+RO)
      ddr_cal1.o (+RO)
      ddr_cal2.o (+RO)
      ddr_cal3.o (+RO)
    }

    ROCODE3 +0
    {
      lzhs.o (+RO)
      bsp_start.o (+RO)
    }

    BSS +0
    {
      * (+ZI)
    }
}

