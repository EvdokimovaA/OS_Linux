#!/bin/bash
g++ -c pract9_2.cpp
g++ -o pract9_2 pract9_2.o -lcap

sudo setcap CAP_SETFCAP=+eip pract9_2