#! /usr/bin/python

import sys
import re

def usage():
   print sys.argv[0]
   print "Usage:"
   print "  ", sys.argv[0], "<L1TrigResults1> <L1TrigResults2> [col # to compare]"

argc = len(sys.argv)
if argc == 1:
   usage()
   sys.exit(1)

# Set parameters
colMenu = 0
colName = 1
colComp = 2
if argc >= 3:
   file1 = sys.argv[1]
   file2 = sys.argv[2]
if argc >= 4:
   colComp = int(sys.argv[3])
if argc == 5:
   colName = int(sys.argv[4])

l1=[]
for line1 in open(file1).xreadlines():
   if re.findall('^[ ]*L1Menu',line1):
      p1 = line1.split()
      #print p1, len(p1)
      #print p1[0]
      l1.append(p1)

l2=[]
for line2 in open(file2).xreadlines():
   if re.findall('^[ ]*L1Menu',line2):
      p2 = line2.split()
      #print p2, len(p2)
      #print p2[0]
      l2.append(p2)

#print l1
#print l2
if len(l1) == len(l2):
   i=0
   for l in l1:
      #print "i:", i
      x1 = float(l1[i][colComp])
      x2 = float(l2[i][colComp])
      #print int(x1), "  ", int(x2)
      if x1 == 0:
	 print l1[i][colName].ljust(30), " denominator is 0"
      else:
	 print l1[i][colName].ljust(30), " ",str((x1-x2)/x1).rjust(20)
      i+=1
else:
   print "list lengths don't match: ", len(l1), " and ", len(l2)

