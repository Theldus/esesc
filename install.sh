#!/bin/bash
RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[0m'

if [ ! -f "CMakeLists.txt" ]; then
	echo -e "${RED}Please make sure to run inside the ESESC root folder!${NC}"
	exit 1
fi

if [ ! "$(grep -Ei 'debian|buntu|mint' /etc/*release)" ]; then
	echo -e "${RED}This script only supports Debian-based distros!${NC}"
	exit 1
fi

#Packages
sudo apt-get update
sudo apt-get -y install build-essential
sudo apt-get -y install cmake
sudo apt-get -y install libboost-dev
sudo apt-get -y install bison flex
sudo apt-get -y install g++
sudo apt-get -y install python
sudo apt-get -y install texinfo
sudo apt-get -y install libglib2.0-dev
sudo apt-get -y install libncurses5-dev

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
