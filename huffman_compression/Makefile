CC = gcc
FLAGS = -O3 -Wall -Wextra -pedantic -std=c99


run: build/dictionary.o build/file_stream.o build/decode.o build/encode.o src/run.o
		$(CC) build/dictionary.o build/file_stream.o build/decode.o build/encode.o build/run.o -o bin/run $(FLAGS)

build/dictionary.o:
		$(CC) -c lib/dictionary.c -o build/dictionary.o $(FLAGS)

build/file_stream.o:
		$(CC) -c lib/file_stream.c -o build/file_stream.o $(FLAGS)

build/decode.o:
		$(CC) -c lib/decode.c -o build/decode.o $(FLAGS)

build/encode.o:
		$(CC) -c lib/encode.c -o build/encode.o $(FLAGS)

src/run.o:
		$(CC) -c src/run.c -o build/run.o $(FLAGS)

clean:
	rm build/* bin/*
