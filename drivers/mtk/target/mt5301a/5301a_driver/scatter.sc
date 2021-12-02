RAM 0x00010000 0x10000000
{
    BOOT 0x00010000
    {
      int.o
    }

    BSP +0
    {
      dram.o nucleus.o
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

    RW +0 NOCOMPRESS
    {
      * (+RW)
    }

    BSS +0
    {
      * (+ZI)
    }
}

