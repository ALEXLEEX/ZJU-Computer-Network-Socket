#!/usr/bin/fish

g++ packet_test.cpp ../src/packet.cpp -o packet_test -I../include -std=c++11 -lssl -lcrypto -w
./packet_test
rm packet_test