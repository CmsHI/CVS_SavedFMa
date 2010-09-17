#! /usr/bin/python
import sys
ct = 0
sum = 0
for line in sys.stdin:
   words = line.split()
   for x in words:
      ct = ct + 1
      #print x
      sum = sum + float(x)
print "sum:", sum

# simple tests
#for x in [1, 2, 3, 4]:
#   sum = sum + x
