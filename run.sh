#!/bin/bash

make && make install && make test

clear

sudo dmesg | tail -20
sudo dmesg --clear

sudo ./config 4
sudo ./push Mihir
sudo ./push Hellow
sudo ./push Worldwa
sudo ./push FULL
sudo ./push OVERFLOW
sudo ./pop
sudo ./push WhyMeGod?
sudo ./config 5
sudo ./push Extra
sudo ./pop
sudo ./pop

sudo dmesg | tail -20
make remove >> temp.txt

rm -rf temp.txt
