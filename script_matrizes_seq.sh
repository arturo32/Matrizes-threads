#!/bin/bash -x
for i in 4 8 16 32 64 128 256 512 1024 2048
do
for j in {1..20} ; do ./main $i S ; done
done
