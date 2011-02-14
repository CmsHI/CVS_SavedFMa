#! /usr/bin/python
import sys
from operator import itemgetter
from itertools import groupby

runlumi = {}
for line in sys.stdin:
  words = line.split()
  if words[0] in runlumi:
    runlumi[words[0]].append(int(words[1]))
  else:
    runlumi[words[0]] = [int(words[1])]

result = {}
for r,lumi in runlumi.iteritems():
  lumi.sort()
  print r
  #print lumi
  ranges = []
  for k,g in groupby(enumerate(lumi), lambda (i,x):i-x):
    #print map(itemgetter(1),g)
    group = map(itemgetter(1),g)
    ranges.append([group[0],group[-1]])
    #print ranges
  result[r] = ranges

print result

