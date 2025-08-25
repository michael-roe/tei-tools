all: tei_betacode

tei_betacode: tei_betacode.o betacode.o
	gcc -o tei_betacode tei_betacode.o betacode.o -lxml2 -lunistring

tei_betacode.o: tei_betacode.c
	gcc -I /usr/include/libxml2 -c tei_betacode.c

betacode.o: betacode.c
	gcc -c betacode.c
