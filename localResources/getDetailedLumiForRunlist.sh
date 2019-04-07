
for i in `cat $1`
do
    brilcalc lumi -r $i --byls --output-style csv | grep -v \# | tr : " " | tr , " " | awk '{print $1" "$3" "$12}' >> $2
done