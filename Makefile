a.out: mainfile.o libmyDES.a
	gcc mainfile.o -L. -lmyDES 

mainfile.o:	mainfile.c myDES.h
	gcc -Wall -c mainfile.c

libmyDES.a:	myDES.o
		ar -rcs libmyDES.a myDES.o

myDES.o:	myDES.c myDES.h
	gcc -Wall -c myDES.c
clean:
	rm a.out mainfile.o libmyDES.a myDES.o
