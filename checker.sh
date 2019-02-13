#!/bin/bash

declare -a resize=(2 3 8)
declare -a num_tread=(1 2 4 8)
image_input=$(ls ./in)
declare -a image_resolution=(1000 8000) 

make clean &> /dev/null
make all &> /dev/null
if [ $? -ne 0 ]; then
	echo "Makefile error!"
	exit 1
fi

mkdir ./out
tput setaf 4
echo -ne "\n\n\t\t\t\t\tTask 1\n"
for k in $image_input
do
	tput setaf 4
	echo -ne "\n\t\tImagine: ${k}\n"
	for i in {0..2}
	do
		for j in {0..3}
		do
			timp=$(./homework ./in/${k} ./out/"${num_tread[$j]}_${resize[$i]}${k}" ${resize[$i]} ${num_tread[$j]})
			
			diff ./out_ref/"${resize[$i]}${k}" ./out/"${num_tread[$j]}_${resize[$i]}${k}"
			if [ $? -eq 0 ]; then
				tput setaf 2
				echo -ne "Pentru : resize: ${resize[$i]} si thred: ${num_tread[$j]} : OK si timp : ${timp}\n"

			else
				tput setaf 1
				echo -ne "Pentru : resize: ${resize[$i]} si thred: ${num_tread[$j]} : Fail si timp : ${timp}\n"
			fi
		done
	done
done

tput setaf 4
echo -ne "\n\n\t\t\t\t\tTask 2\n"
for i in {0..1}
do
	tput setaf 4
	echo -ne "\n\t\tResolution : ${image_resolution[$i]}\n"
	for j in {0..3}
	do
		timp=$(./homework1 ./out/"${image_resolution[$i]}_${num_tread[$j]}.pgm" ${image_resolution[$i]} ${num_tread[$j]})
		
		diff ./out_ref/"${image_resolution[$i]}.pgm" ./out/"${image_resolution[$i]}_${num_tread[$j]}.pgm"
		if [ $? -eq 0 ]; then
			tput setaf 2
			echo -ne "Pentru : resolution: ${image_resolution[$i]} si thred: ${num_tread[$j]} : OK si timp : ${timp}\n"
		else
			tput setaf 1
			echo -ne "Pentru : resolution: ${image_resolution[$i]} si thred: ${num_tread[$j]} : Fail si timp : ${timp}\n"
		fi
	done
done
