echo Please key relase chip name:
read choice
echo $choice
cp -rf ./../mstar2/hal/$choice/ ./../mstar2/drv/$choice
rm -rf ./../mstar2/hal/*
cp -rf ./../mstar2/drv/$choice/ ./../mstar2/hal/
rm -rf ./../mstar2/drv/$choice
rm -rf arch/Kconfig.kdebug
rm -rf kernel/kdebugd/
cp arch/arm/Kconfig_no_kdebug arch/arm/Kconfig
cp kernel/Makefile_no_kdebug kernel/Makefile
