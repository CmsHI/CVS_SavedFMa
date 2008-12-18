{
   #print $0
   #print NF
   if (NF == 1) {
      print $1
   }
   if (NF >= 2) {
      OFS = "_"
      print $1,$2
   }
}
