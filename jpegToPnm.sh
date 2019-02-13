#!/bin/bash

file_name=$(ls *.jpeg  )

for i in $file_name
do
	#convert $i "$(basename "$file" .html).pnm"
	jpegtopnm $i >"$(basename "$i" .jpeg).pnm"
done
#resize mogrify -resize 2338x8438! *.pnm
