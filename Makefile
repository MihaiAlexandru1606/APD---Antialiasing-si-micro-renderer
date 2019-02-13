all: homework homework1

homework: ./src/homework.c ./src/homework.h ./src/main.c
	gcc -o homework ./src/homework.c ./src/main.c -lpthread -Wall -lm
homework1: ./src/homework1.c ./src/homework1.h ./src/main1.c
	gcc -o homework1 ./src/homework1.c ./src/main1.c -lpthread -Wall -lm

.PHONY: clean
clean:
	rm -f homework
	rm -f homework1
	rm -rf out/* ./out

zip:
	zip -j 335CB_NiculescuMihaiAlexandru_Tema1.zip ./src/homework.c ./src/homework.h ./src/homework1.c ./src/homework1.h Readme.txt

fep: homework homework1
	rm -fr fep.zip
	zip fep.zip homework homework1 in/* ./in  ./out_ref out_ref/*
	scp fep.zip mihai.niculescu1606@fep.grid.pub.ro:.

