#!/bin/bash
rm build -rf
mkdir build
cd build
cmake -DMCU_TYPE=MM32F013x ..
make
rm -rf ~/shardir/mm32_firmware/mm32_irmp_irsnd.hex
cp mm32_irmp_irsnd.hex ~/shardir/mm32_firmware/