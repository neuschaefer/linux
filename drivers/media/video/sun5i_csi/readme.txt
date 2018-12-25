===========================================

Version: V1_41

Author:  raymonxiu

Date:     2012-9-18 18:34:34

Description:

newest module list:(X = 0 or 1)
insmod sun4i_csiX.ko ccm="ov7670" i2c_addr=0x42
insmod sun4i_csiX.ko ccm="gc0308" i2c_addr=0x42
insmod sun4i_csiX.ko ccm="gt2005" i2c_addr=0x78
insmod sun4i_csiX.ko ccm="hi704"  i2c_addr=0x60
insmod sun4i_csiX.ko ccm="sp0838" i2c_addr=0x30
insmod sun4i_csiX.ko ccm="mt9m112" i2c_addr=0xba
insmod sun4i_csiX.ko ccm="mt9m113" i2c_addr=0x78
insmod sun4i_csiX.ko ccm="ov2655" i2c_addr=0x60
insmod sun4i_csiX.ko ccm="hi253" i2c_addr=0x40
insmod sun4i_csiX.ko ccm="gc0307" i2c_addr=0x42
insmod sun4i_csiX.ko ccm="mt9d112" i2c_addr=0x78
insmod sun4i_csiX.ko ccm="ov5640" i2c_addr=0x78
insmod sun4i_csiX.ko ccm="gc2015" i2c_addr=0x60
insmod sun4i_csiX.ko ccm="ov2643" i2c_addr=0x60
insmod sun4i_csiX.ko ccm="gc0329" i2c_addr=0x62
insmod sun4i_csiX.ko ccm="gc0309" i2c_addr=0x42
insmod sun4i_csiX.ko ccm="tvp5150" i2c_addr=0xb8
insmod sun4i_csiX.ko ccm="s5k4ec" i2c_addr=0x5a
insmod sun4i_csiX.ko ccm="ov5650_mv9335" i2c_addr=0x50

V1_41
CSI: Fix gc0309/gt2005/ov5640 bug and add ov5650+mv9335.
1) Fix gc0309 red image
2) Fix ov5640 capturing bug at low fps
3) Add ov5650 + mv9335 isp support
4) Fix gt2005 I2C error at CTS test
5) Optimize gc0307 by galaxycore


V1_40
CSI: Optimizing GC and OV sensor/cached videobuf
1) Optimizing gc0307,gc0308,gc0309,gc0329,gt2005,gc2015
2) Fix ov5640 flicker when capturing image
3) Modify videobuf to be cached

V1_31
CSI: Optimizing ov5640/gc0308/sp0838/ov2655 and new module s5k4ec support
1) Optimizing ov5640/gc0308 for fast response
2) Fix sp0838 for CTS test
3) Add delay after ov2655 pll changed
4) New samsung 5M module s5k4ec support
5) Modify drivers/media/video/makefile to not compile sun4i_csi

V1_30
CSI: Support BT656 I/F, add tvp5150 device and add ov5640 g_af
1) Support 8bit BT656 interface
2) Support tvp5150 tv decoder
3) Add ov5640 g_autofocus 

V1_21
CSI: Adjust the ov5640 io drive cap to maximum
1) Adjust io drive
2) Add s_fps function

V1_20
CSI: Fine tune ov5640 and add autofocus function
1) Fine tune ov5640
2) Add autofocus function
3) Modify the buffer number to 1 when capturing still image
4) Return 0 when s_parm do nothing
5) Fix hi253 adn hi704 to 27MHz (settting from hynix)
6) Fix gc2015 crash bug on capture picture
7) Modify app_test.c to match the new driver

V1_13
CSI: Fix bugs and add new module gc0309 support 
1) Fix gc0308 AWB recovery
2) Fix ov2643 UXGA flicker
3) Add new module gc0309

V1_12
CSI: Optimizing for CTS test and fix bug
1) Optimizing gc0308 and gt2005 for CTS test
2) Fix clock alternating bug
3) Add gc0329 module
4) Modify all msleep to mdelay

V1_11
CSI: Modify clock gating and axp_gpio_get_io bug 
1) Insure the clk_enable() and clk_disable() are called in pair
2) Fix the axp_gpio_get_io() bug for pmu gpio2

V1_10
CSI: Merge modification from sun4i and fix gc0308 red color 
1) Judge if csi is generating before csi_read and csi_poll
2) Add i2c adapter lock when camera power on/off and standby on/off
3) Modify standby and reset io sequence when power on and standby off
4) Add standy and reset control before power off
5) Add new camera module gc2015,ov2643 and modify deconfig
6) Modfiy the device source code to keep identical between sun4i/5i
7) Fix gc0308 red color

V1_01
CSI: Fix bugs, add new modules support and modity power/standby interface
     Modify new camera modules default menuconfig 
1) Fix bugs for CTS test
2) Fix bugs for crash when insmod after rmmod
3) Add default format for csi driver
4) Modify the power on/off,stanby on/off interface
5) Fix bugs for multipex dual sensors using one csi 
6) Add gc0307, mt9d112 and ov5640 modules support
7) Fix gc0308 AWB alternation bug
8) Modify new camera modules default menuconfig 

V1_00
CSI:Initial version for linux 3.0.8
1) Ported from A10 V1_02


