#!/bin/bash -
cvs co EventFilter/SiStripRawToDigi # head version of 2009.8.19
cd EventFilter/SiStripRawToDigi/plugins
mkdir org
mv SiStripDigiToRawModule.cc org/
cat org/SiStripDigiToRawModule.cc | \
   sed 's/<SiStripDigi>/<SiStripRawDigi>/g' | \
   sed 's;SiStripDigi\.h";SiStripDigi.h"\n#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h";' \
   > SiStripDigiToRawModule.cc
cd -

scramv1 b
