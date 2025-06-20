#!/bin/bash -e 

out=""
if [ "$#" -lt 1 ];
then
    echo "Usage: ./test.sh <name>"
    echo "Usage:        E.g., ./test.sh read"
    exit 0
fi

out=$out"$1"
if [[ $1 == *"-variable" ]]; then
    if [ "$#" -lt 4 ];
    then
        echo "Usage: ./test.sh $1 <blocksize> <iterations> <offset>"
        echo "Usage:        E.g., ./test.sh test-read-variable 512 1000 0"
        exit 0
    fi
    out=$out"-$2-$3-$4"
fi

# Check which /dev entry contains the USB device
dpath=/dev/`lsblk | grep " 1G " | cut -d ' ' -f 1`

# Cleaning the device 
echo "Cleaning Device"
SIZE=$(blockdev --getsize64 "$dpath")
dd if=/dev/zero of="$dpath" bs=1M count=$(( SIZE / 1024 / 1024 ))

# Remove the module if it is present
sudo rmmod kmod || true

# Make the module
pushd kmodule
    make
    sudo insmod kmod.ko device=$dpath  
popd

# Run a testcase
pushd testcases
    rm -rf *.txt
    make clean
    make test-$1
    echo ""
    echo ""
    echo "----------------------------TESTCASE---------------------------"
    sudo ./test-$1 $dpath $2 $3 $4 > ../outputs/$out 2<&1
popd


echo "----------------------------RESULT---------------------------"
cat outputs/$out

#Remove the module after use
pushd kmodule
    make remove
popd
