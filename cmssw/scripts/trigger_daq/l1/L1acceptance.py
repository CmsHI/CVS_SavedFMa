#! /usr/bin/python

import sys
#from decimal import *
import decimal

def usage():
   print sys.argv[0]
   print "Usage:"
   print "  ", sys.argv[0], "<L1TrigResults1> [col # of passed evts] [col # of rej evts]"

argc = len(sys.argv)
if argc == 1:
   usage()
   sys.exit(1)

# assign parameters
colMenu = 0
colName = 1
colPas = 2
colRej = 3
if argc >= 2:
   file1 = sys.argv[1]
if argc >= 3:
   colPas = int(sys.argv[2])
if argc == 4:
   colRej = int(sys.argv[3])

# Load data into lists
l1=[]
for line1 in open(file1).xreadlines():
   p1 = line1.split()
   #print p1, len(p1)
   #print p1[0]
   l1.append(p1)

# settings
decimal.getcontext().prec = 2

#print l1
nTotalPrev = 0
nTotalCur = 0
totalPassRate = 0
i=0
for l in l1:
   nPas = int(l1[i][colPas])
   nRej = int(l1[i][colRej])
   nTotalCur = nPas + nRej
   # fist bit: print some general information
   if i == 0:
      nTotalPrev = nTotalCur
      print "L1 menu: ", l1[i][colMenu]
      print "  total L1 accepted event: ", nTotalCur
      print
   # test nPas + nRej is consistent with the total (calculated from previous line)
   else:
      if nTotalCur != nTotalPrev:
	 print "Problem: ", l1[i][colName], " has inconsistent total L1 input number"
	 sys.exit(2)

   # Calculate pass rate
   passRate = decimal.Decimal(nPas)/decimal.Decimal(nTotalCur)
   totalPassRate += passRate

   # Output results
   print l1[i][colMenu].ljust(20), l1[i][colName].ljust(35), str(passRate).ljust(10)
   

   # increment counter
   i+=1

# Other checks
#print "Total accept rate: ", totalPassRate

