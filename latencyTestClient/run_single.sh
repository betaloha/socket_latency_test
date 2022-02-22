#!/bin/bash

for ((i=8;i<=8192;i=i*4)) ; do sudo LD_PRELOAD=libvma.so VMA_TRACELEVEL=0 ./latencyTestClient 1000000 $i 51000 0 2>> $1 ; done
