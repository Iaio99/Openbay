#!/bin/sh

mkdir ../consegna
mkdir /tmp/consegna
cp -r ../src/* ../CMakeLists.txt /tmp/consegna
tar -C /tmp/consegna -cvf ../consegna/pms.tar .
cp Aste\ Online.pdf ../consegna
cp Aste_Online.sql ../consegna
