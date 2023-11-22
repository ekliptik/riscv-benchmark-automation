#!/bin/bash

set -e

BASEDIR=$PWD

TC_FOLDER=$TOOLS
FLAGS="-march=rv32imafdc -mabi=ilp32d -ffunction-sections -fdata-sections $CFLAGS"
LFLAGS_EXTRA="-march=rv32imafdc -mabi=ilp32d -Wl,--gc-sections $LDFLAGS"

CM_FOLDER=coremark
PORT_FOLDER=riscv32-spike

rm -f $BASEDIR/$CM_FOLDER/*.o
rm -f $BASEDIR/$PORT_FOLDER/*.o

cd $BASEDIR/$CM_FOLDER

make clean

# run the compile
echo "Start compilation"
make PORT_DIR="../"$PORT_FOLDER ITERATIONS=10 LFLAGS_EXTRA="$LFLAGS_EXTRA" USER_FLAGS="$FLAGS" TC=$TC_FOLDER link

#make PORT_DIR=../riscv64-baremetal compile
mv coremark.riscv ../
cd ..
echo "Start simulation (takes time)"
script '-c' 'spike --isa=rv32gc_Zicsr coremark.riscv' '-e'
