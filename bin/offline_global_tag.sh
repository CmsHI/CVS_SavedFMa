#!/bin/bash
#-- For available tags: --
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
#defaultRecord="CMS_COND_20X_GLOBALTAG"
defaultRecord="CMS_COND_21X_GLOBALTAG"

#-- For availabe frontier or local DB Connections --
# https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideORACLEProductionAndDevelopmentDatabaseSetup#Production

#-- Interation --
#connection='frontier://(proxyurl=http://localhost:3128)(serverurl=http://cmsfrontier.cern.ch:8000/FrontierInt)(serverurl=http://cmsfrontier1.cern.ch:8000/FrontierInt)(serverurl=http://cmsfrontier2.cern.ch:8000/FrontierInt)(serverurl=http://cmsfrontier3.cern.ch:8000/FrontierInt)(retrieve-ziplevel=0)'
#-- Producation --
connection='frontier://(proxyurl=http://localhost:3128)(serverurl=http://cmsfrontier.cern.ch:8000/FrontierProd)(serverurl=http://cmsfrontier1.cern.ch:8000/FrontierProd)(serverurl=http://cmsfrontier2.cern.ch:8000/FrontierProd)(serverurl=http://cmsfrontier3.cern.ch:8000/FrontierProd)(retrieve-ziplevel=0)'

echo "$connection/$defaultRecord" -T $1 
if [ $# -eq 0 ]; then
   cmscond_tagtree_list -c "$connection/$defaultRecord" 
elif [ $# -eq 1 ]; then
   cmscond_tagtree_list -c "$connection/$defaultRecord" -T $1 
elif [ $# -eq 2]; then
   cmscond_tagtree_list -c "$connection/$1" -T $2 
fi

