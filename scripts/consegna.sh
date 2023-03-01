#!/bin/sh

mkdir ../consegna
mkdir /tmp/consegna
cp -r ../src/* ../CMakeLists.txt /tmp/consegna
tar -C /tmp/consegna -cvf ../consegna/openbay.tar .
cp ../doc/Openbay.pdf ../consegna
cp Openbay.sql ../consegna
