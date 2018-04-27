CC = gcc
objects = server.o update.o date.o 
headers = update.h date.h 
files = server.c update.c date.c
name = p

p : $(objects)
	$(CC) -o $(name) $(objects) -pthread

server.o : server.c $(headers)
	$(CC) -c server.c

update.o : update.c
	$(CC) -c update.c

date.o : date.c
	$(CC) -c date.c



clean:
	rm $(name) $(objects)

