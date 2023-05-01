#!/bin/bash
g++ -c pract9_1.cpp
g++ -o pract9_1 pract9_1.o -lcap

touch file
sudo chown root file
sudo chgrp root file