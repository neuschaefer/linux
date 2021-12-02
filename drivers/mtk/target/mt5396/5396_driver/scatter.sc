RAM 0x00010000 0x10000000
{
    BOOT 0x00010000
    {
      int.o
    }

    BSP +0
    {
      dram.o nucleus.o nucleus_ca9.o
    }

    RO +0
    {
      * (+RO)
    }

    MMU +0
    {
      pagetab.o (+RW)
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

