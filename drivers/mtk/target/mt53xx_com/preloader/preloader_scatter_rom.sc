RAM 0x2AFFE000 0x10000000
{
    BOOT 0x2AFFE000
    {
      int.o
    }

    BSP +0
    {
      startup.o arm.o chip_boot.o chip_txt.o
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

