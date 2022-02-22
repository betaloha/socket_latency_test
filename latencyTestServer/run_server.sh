#!/bin/bash

sudo LD_PRELOAD=libvma.so taskset -c 0-2 ./latencyTestServer 51000
