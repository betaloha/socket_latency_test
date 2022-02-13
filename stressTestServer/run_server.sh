#!/bin/bash

sudo LD_PRELOAD=libvma.so taskset -c 0-2 ./stressTestServer 51000
