#!/bin/bash -
if [ $# -lt 1 ]; then
  echo Usage:
  echo "  plotRspRes.sh <input_jra_fullgraphs.root>"
  exit 1
fi

input=$1
cmsswbase=/home/frankma/work/HI/jet/sw/JES/ppjesCMSSW_3_3_1
config=$cmsswbase/src/JetMETAnalysis/JetAnalyzers/config/hijra_plot_rspres.config

for ivar in RelRspVsRefPt RelResVsRefPt; do
  jet_inspect_graphs_x $config -inputs $input -variables $ivar -batch true
  for ieta in seq 0 2; do
    jet_inspect_graphs_x $config -inputs $input -variables $ivar:JetEta@$ieta -batch true
  done
done

