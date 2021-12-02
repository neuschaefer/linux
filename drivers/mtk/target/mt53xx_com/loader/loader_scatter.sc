RAM 0x00D00000 0x10000000
{
    BOOT 0x00D00000
    {
      int.o
    }

    BSP +0
    {
      startup.o arm.o chip_boot.o
    }

    RO +0
    {
      * (+RO)
    }

    DATACLI +0
    {
      * (data.cli)
    }

    ENDCLI +0
    {
      * (end.cli)
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

