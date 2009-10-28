#!/bin/bash -
if [ $# -lt 1 ]; then
  echo Usage:
  echo " run_respon.sh <input_jra_histos.root>"
  exit 1
fi

# set var's
input=$1

# 2. get responses
jet_response_and_resolution_x -input $input -output ${input%.root}_fullg.root

