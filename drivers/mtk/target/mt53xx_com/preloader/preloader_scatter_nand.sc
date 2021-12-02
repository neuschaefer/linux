RAM 0x00000000 0x10000000
{
    BOOT 0x00000000
    {
      int.o
    }

    BSP +0
    {
      startup.o arm.o chip_boot.o nandboot.o chip_txt.o
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

