The GameCube/Wii/WiiU External Interface (EXI)
==============================================

::

	+---------------+                         +----------------+
	| EXI channel 0 |---------- MISO -+-------| channel 0,     |
	|               |---------- MOSI -|-+-----| device 0       |
	|               |---------- SCK --|-|-+---|                |
	|               |---------- CS0 --|-|-|---| Memory Card    |
	|               |---------- EXT --|-|-|---| slot A         |
	|               |---------- CS1 . | | |   +----------------+
	+---------------+               | | | |
	| EXI channel 1 |-- MISO/MOSI   | | | |
	|               |-- SCK/CSx/EXT | | | |   +----------------+
	+---------------+               | `-|-|---| channel 0,     |
	| EXI channel 2 |-- ...         |   `-|---| device 1       |
	|               |               |     `---|                |
	+---------------+               `---------| ROM/RTC/SRAM   |
	                                          +----------------+

EXI is an enumerable, SPI-like bus. The normal EXI host controller as used in
the Nintendo GameCube and Wii has three *channels*, which are essentially
independent controllers with their own register blocks, each of which supports
three *devices* through the use of three chip select lines. In addition, the
host controller can detect whether a card was inserted in the slot associated
with a channel's slot (through the ``EXT`` line in the above picture).

Enumeration works in the following way: The OS writes the 16-bit number
``0x0000``, and then reads a 32-bit number back. This number identifies the
connected device, for example ``0x00000010`` for the 251-block Memory Card.


Device drivers
--------------

Outlined below is the recommended practice when writing EXI device drivers.

The bus driver already handles quite an amount of stuff, although some of
it might have to be implemented by individual device drivers. If in doubt,
see include/linux/exi.h.


Registration
------------

Declare a :c:type:`struct exi_driver <exi_driver>`. Initialize at least the
``name``, ``eid_table``, ``frequency``, ``probe`` and ``remove`` fields:

.. code-block:: c

	static struct exi_device_id frob_eid_tbl[] = {
		{ .id = EXI_ID_FROB0, },
		{ .id = EXI_ID_FROB1, },
		{ .id = EXI_ID_FROB2, },
	};

	static struct exi_driver frob_driver = {
		.name		= "frob",
		.eid_table	= frob_eid_tbl,
		.frequency      = EXI_CLK_8MHZ,
		.probe		= frob_probe,
		.remove		= frob_remove,
	};


``name`` distinguishes the driver from others registered with the bus.
It should be short, unique, yet remain informative.

``eid_table`` is a pointer to a table of device IDs the driver claims to
support. These should be taken directly from include/linux/exi_ids.h.

``frequency`` specifies the frequency at which the EXI clock shall be run. It
must be between EXI_CLK_1MHZ and EXI_CLK_32MHZ.

``probe`` is a pointer to a function that's called once the driver is bound
to a device it claims to support.

``remove`` is a pointer to a function that's called when either the driver
unregisters with the bus, or a device bound to that specific driver is
physically unplugged from the bus.

From within the driver's initialization function, register the driver with the
bus by calling :c:func:`exi_driver_register` with the driver structure declared
previously:

.. code-block:: c

	static int __init frob_init(void)
	{
		return exi_driver_register(&frob_driver);
	}


Deregistration
--------------

If the driver may be compiled as a loadable kernel module, then all you have to
do is call :c:func:`exi_driver_unregister` in the driver's exit function:

.. code-block:: c

	static void __exit frob_exit(void)
	{
		exi_driver_unregister(&frob_driver);
	}

Device Private Data
-------------------

The functions :c:func:`exi_set_drvdata`/:c:func:`exi_get_drvdata` are available
for the sole purpose of setting and getting driver private data for an EXI
device.  These are simply helpers around the driver-model functions that do the
actual work. Use them. That way, you don't have to worry (or worry less) about
changes to the driver-model API.


API Reference
-------------

.. kernel-doc:: include/linux/exi.h

.. kernel-doc:: drivers/exi/exi-driver.c
   :export:

.. kernel-doc:: drivers/exi/exi-hw.c
   :export:

