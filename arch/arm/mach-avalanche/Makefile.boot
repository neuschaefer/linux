# This is used for compressed kernel booting
# check kernel documentation
#
#
#/* Copyright 2008, Texas Instruments Incorporated
#*
#* This program has been modified from its original operation by Texas Instruments
#* to do the following:
#* Explanation of modification.
#*  relocation address changes
#*
#* THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
#* "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
#* OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
#* TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
#* PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
#* DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
#* COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
#* See The GNU General Public License for more details.
#*
#* These changes are covered under version 2 of the GNU General Public License,
#* dated June 1991.
#*/

zreladdr-y    = $(CONFIG_ARM_AVALANCHE_KERNEL_ZRELADDR_ADDRESS) 
params_phys-y = $(CONFIG_ARM_AVALANCHE_KERNEL_PARAMS_ADDRESS) 
initrd_phys-y = $(CONFIG_ARM_AVALANCHE_KERNEL_INITRD_ADDRESS) 
