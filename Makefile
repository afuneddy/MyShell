
Shell35: main.o hash.o cmd.o parser.o execute.o   
	gcc -o Shell35 main.o hash.o cmd.o parser.o execute.o -lm
main.o: main.c header.h
	gcc -c main.c 
hash.o: hash.c  header.h
	gcc -c hash.c 
cmd.o: cmd.c  header.h
	gcc -c cmd.c 
parser.o: parser.c  header.h
	gcc -c parser.c 
execute.o: execute.c  header.h
	gcc -c execute.c 
clean:
	rm -f *.c * *.o Shell35 
