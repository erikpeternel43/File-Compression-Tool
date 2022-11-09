CC = gcc
FLAGS = -O3 -Wall -Wextra -pedantic -std=c99


run: build/dictionary.o build/file_stream.o build/lzw_decode.o build/lzw_encode.o src/run.o
		$(CC) build/dictionary.o build/file_stream.o build/lzw_decode.o build/lzw_encode.o build/run.o -o bin/run $(FLAGS)

build/dictionary.o:
		$(CC) -c lib/dictionary.c -o build/dictionary.o $(FLAGS)

build/file_stream.o:
		$(CC) -c lib/file_stream.c -o build/file_stream.o $(FLAGS)

build/lzw_decode.o:
		$(CC) -c lib/lzw_decode.c -o build/lzw_decode.o $(FLAGS)

build/lzw_encode.o:
		$(CC) -c lib/lzw_encode.c -o build/lzw_encode.o $(FLAGS)

src/run.o:
		$(CC) -c src/run.c -o build/run.o $(FLAGS)

clean:
	rm build/* bin/*
