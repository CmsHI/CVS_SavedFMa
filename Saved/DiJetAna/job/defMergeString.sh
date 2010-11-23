infile=$1
mergeList="";for i in `cat $infile`; do mergeList=$mergeList" $i"; done
echo $mergeList
