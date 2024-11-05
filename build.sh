#!/usr/bin/bash

cd src/server
bash build.sh
cd ../client
bash build.sh
cd ..
clear
echo "Build Server and Client Success!"