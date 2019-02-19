#!/bin/bash
# Init kattis folder and download test files.

URL="https://kth.kattis.com/problems/$1/file/statement/samples.zip"
wget -q  "$URL" 
if [ $? -ne 0 ] ; then
  echo "Failed"
else
  mkdir $1
  ln -s ../Makefile ./$1/Makefile
  cp ./main.cpp $1/main.cpp
  mkdir $1/test
  unzip samples.zip -d $1/test/ > /dev/null
  rm samples.zip
fi

