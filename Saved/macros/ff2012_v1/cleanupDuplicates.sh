while read inputline
do
  for (( i=1;i<=2000;i++)) 
    do
    workDir="${inputline}"
    filename="output_${i}_*.root"
 #   echo "file name = $workDir/$filename"
    ls $workDir/$filename >& tmp_$i.txt
    nDup="$(wc tmp_$i.txt | awk '{print $1}')"
    echo " In" tmp_$i.txt "   nDup = " $nDup
    for (( j=2;j<=$nDup;j++))
      do
      tobeRem="$( cat tmp_$i.txt | head -$j | tail -1)"
      echo " I will remove " $tobeRem
      echo "rm" $tobeRem
      rm $tobeRem
    done
    
  done
done < listOfJobs.txt
mkdir tmpTxtDir
mv tmp_*.txt tmpTxtDir

#crab -status -c $filename
#  crab -getoutput -c $filename
#  crab -status -c $filename > ${filename}_output.txt
 # nline="$(cat -n ${filename}_output.txt  | grep "Jobs with Wrapper Exit Code" | wc -l)"
 # echo "$nline different exit codes!!!"
 #      cat -n ${filename}_output.txt  | grep "Jobs with Wrapper Exit Code"  > ${filename}_output_exit.txt
 #      cat ${filename}_output_exit.txt
 #      while read inline # loop over all the lines
	## do 
	#   begin="$(echo $inline | awk '{print $10}')"
	#   echo "$begin is the exit code!"
	#   if [ $begin -eq 0 ]
	#       then
	 #      echo "This one with a zero exit code!"
	 #  else
	 #      echo "Non zero exit code: $begin"
	 #      last="$(echo $inline | awk '{print $1}')"
	 ##      echo "last = $last"
	 #      newbegin=`expr $last + 1`
	  #     newlast=`expr $last + 1`
	 #      range="$newbegin, ${newlast}p"
	  #     echo "real range is $range"
	  #     #sed -n "$(echo $range)" ${filename}_output.txt
	  #     jobrange="$(sed -n "$(echo $range)" ${filename}_output.txt | awk '{print $4}')"
	  #     echo "jobs to be resumitted = $jobrange"
	  #     echo crab -resubmit $jobrange -c $filename
	  #     crab -resubmit $jobrange -c $filename
	 #  fi
	   #rm ${filename}_output.txt
	#   sleep 5
#       done < ${filename}_output_exit.txt
#done
