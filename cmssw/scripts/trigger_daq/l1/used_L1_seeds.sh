#!/bin/bash
grep -o L1_[[:alnum:]]* $1 | grep -v L1Extra | sort | uniq

