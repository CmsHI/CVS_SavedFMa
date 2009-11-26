getsc=$CMSSW_BASE/src/HLTrigger/Configuration/test/getHLT.py
menu=/cdaq/special/CirculatingBeam09/v2.0/HLT/V7
processName=HLTANA
cmd="$getsc --globaltag GR09_E_V6::All --data orcoff:$menu $processName"
echo $cmd
eval $cmd
