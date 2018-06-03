#!/bin/bash

if [ ! -f "CMakeLists.txt" ]; then
	echo "Please make sure to run inside the ESESC root folder"
	exit 1
fi

RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[0m'

#Build
rm -rf ./build &> /dev/null
mkdir -p build/run
cd build/
time cmake ../
time make

#Copying files
cp ../conf/* run/
cp ../conf/scripts/myrun.sh run/
cp ../bins/kernels/lu/lu.armel run/
cp main/esesc run/

#Permissions
chmod +x run/esesc
chmod +x run/myrun.sh

cd ../

echo ""
echo ""
echo    "Please make sure to run as root the following command:"
echo -e "   ${GREEN}echo 4096 > /proc/sys/vm/mmap_min_addr${NC}"
echo    "everytime you use ESESC"
