compile: server client 

server: server.o networking.o execr.c parser.c shmc.c
	gcc -o server server.o networking.o execr.c  parser.c shmc.c

client: client.o networking.o execr.c parser.c
	gcc -o client client.o networking.o execr.c parser.c

server.o: server.c networking.h execr.h shmc.h
	gcc -c server.c

client.o: client.c networking.h parser.h
	gcc -c client.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
