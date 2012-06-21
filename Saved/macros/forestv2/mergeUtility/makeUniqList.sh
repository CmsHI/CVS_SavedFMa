indir=$1
basename=${indir##*/}
listname=${basename}.txt
ls -ltr $indir | grep root > ${basename}_raw.txt
ls -ltr $indir | grep root | awk '{if ($5>500) print $NF, int($5/100) }' | awk -F_ '{print $1 "_" $2, $NF}' | awk '{print $1, $NF}' | sort > $listname
cat $listname | uniq > ${basename}_uniq.txt

numUniq=`cat ${basename}_uniq.txt | wc -l`
echo $numUniq

echo "" > ${basename}_final.txt
for il in `seq 1 ${numUniq}`; do
   line=`sed -n "${il}p" ${basename}_uniq.txt`
   name=`echo $line | awk '{print $1}'`
   fsize=`echo $line | awk '{print $2}'`
   echo $name $fsize
   ls -ltr /mnt/hadoop/cms/store/user/frankmalocal/forest/promptskim-hihighpt-hltjet80-pt90-v3/*.root | grep $name | grep $fsize | tail -n 1 | awk '{print $NF}' >> ${basename}_final.txt
done

for i in `ls $indir | grep root`; do
   echo $i
done