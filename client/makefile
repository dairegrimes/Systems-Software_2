CC = gcc
objects = client.o
headers = client.c

name = p

p : $(objects)
	$(CC) -o $(name) $(objects) -lm

client.o : client.c $(headers)
	$(CC) -c client.c


clean:
	rm $(name) $(objects)