#!/bin/bash

while read LINE 
do
./RemoveNoisyStrips resources/runlist.txt $LINE results/$LINE resources/static_resources/area_noise2.txt

done < resources/rolllist
