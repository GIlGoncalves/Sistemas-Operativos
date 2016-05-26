default:all

clear:
	clear
clean:
	rm -f *.o cliente servidor toServidor

dire:
	rm -r data metadata

listaLigada.o: listaLigada.c listaLigada.h
	gcc -c listaLigada.c -Wall

servidor: servidor.c listaLigada.o
	gcc -o servidor servidor.c listaLigada.o -Wall

cliente: cliente.c
	gcc -o cliente cliente.c -Wall

all: servidor cliente
