#!/bin/bash
#defaultRecord="CMS_COND_20X_GLOBALTAG"
#defaultRecord="CMS_COND_21X_GLOBALTAG"
defaultRecord="CMS_COND_31X_GLOBALTAG"

connection='frontier://(proxyurl=http://localhost:3128)(serverurl=http://frontier1.cms:8000/FrontierOnProd)(serverurl=http://frontier2.cms:8000/FrontierOnProd)(retrieve-ziplevel=0)'

echo "$connection/$defaultRecord" -T $1 
if [ $# -eq 0 ]; then
   cmscond_tagtree_list -c "$connection/$defaultRecord" 
elif [ $# -eq 1 ]; then
   cmscond_tagtree_list -c "$connection/$defaultRecord" -T $1 
elif [ $# -eq 2]; then
   cmscond_tagtree_list -c "$connection/$1" -T $2 
fi

