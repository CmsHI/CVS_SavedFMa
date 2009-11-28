getsc=$CMSSW_BASE/src/HLTrigger/Configuration/test/getHLT.py
menu=/online/collisions/week49/HLT/V2
processName=HLTANA
cmd="$getsc --globaltag STARTUP3X_V8D::All --mc $menu $processName"
echo $cmd
eval $cmd
