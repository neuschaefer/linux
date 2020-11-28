#!/bin/bash
clear
echo -e "\033[37;41;1mStart : $(date)\033[0m"
color=44
config_file_prefix=.config_g6_*
sh genlink_mstar2.sh

for config_file in $config_file_prefix
do
  echo -e "\033[37;${color};1m +++++ building ${config_file} +++++ \033[0m"

  if [[ "${config_file}" == *nfs* ]]; then
    isNFS=1
  else
    isNFS=0
  fi
    
  if [ $isNFS == 1 ]; then
     target_folder="../kernel_nfs"
     target_file="uImage_nfs"
  else
     target_folder="../kernel"
     target_file="uImage"
  fi

  mkdir -p $target_folder
  mkdir -p $target_folder/modules

  rm .config
  ls -lah ${config_file}
  sleep 5
  cp ${config_file} .config
  make clean
  make silentoldconfig
  make -j6; make uImage -j6

  cp -f ./arch/arm/boot/uImage $target_folder/$target_file
  find ../ -type f -iname *.ko -print0 | grep -zf vestel_ko_list.txt | xargs -0 cp -t $target_folder/modules/

  sleep 3
  echo -e "\033[37;${color};1m ----- $target_file copied to $target_folder ----- \033[0m"
  echo "=== "
done
ls -lah ../kernel
ls -lah ../kernel/modules
echo -e "\033[37;41;1mEnd : $(date)\033[0m"
