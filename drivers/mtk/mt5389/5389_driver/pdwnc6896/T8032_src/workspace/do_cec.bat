cls
call link clean
call link %1
call b2h %1.bin mtk_t8032fw.h
copy mtk_t8032fw.h ..\..\..\drv_cust\mtk
