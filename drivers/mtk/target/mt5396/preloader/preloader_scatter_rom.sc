RAM 0xFB000000 0x10000000
{
    BOOT 0xFB000000
    {
      int.o
    }
    NANDTAG 0xFB000100 FIXED PADVALUE 0xFFFFFFFF
    {
      *(ROMNANDTAG)
    }
    BSP +0
    {
      startup.o startup_ca9.o arm.o chip_boot.o chip_txt.o
    }

    RO +0
    {
      * (+RO)
    }

    RW +0
    {
      * (+RW)
    }

    BSS +0
    {
      * (+ZI)
    }
}

