#! /bin/bash -
for i in $(find . -type f); do
   result=$(grep -i "$1" $i)
   if [ $? -eq 0 ]; then
      echo "$i:"
      echo "$result"
   fi
done

