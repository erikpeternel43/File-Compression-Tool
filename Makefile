CC = gcc
FLAGS = -O3 -Wall -Wextra -pedantic -std=c99
GUI_FLAGS = `pkg-config --cflags --libs gtk+-3.0` -export-dynamic


run: lzw_compression/build/dictionary.o lzw_compression/build/file_stream.o lzw_compression/build/decode.o lzw_compression/build/encode.o lzw_compression/build/run.o huffman_compression/build/dictionary.o huffman_compression/build/file_stream.o huffman_compression/build/decode.o huffman_compression/build/encode.o huffman_compression/build/run.o build/gui.o build/run.o 
		$(CC) build/gui.o build/run.o -o bin/run $(FLAGS) $(GUI_FLAGS)
		$(CC) lzw_compression/build/dictionary.o lzw_compression/build/file_stream.o lzw_compression/build/decode.o lzw_compression/build/encode.o lzw_compression/build/run.o -o lzw_compression/bin/run $(FLAGS)
		$(CC) huffman_compression/build/dictionary.o huffman_compression/build/file_stream.o huffman_compression/build/decode.o huffman_compression/build/encode.o huffman_compression/build/run.o -o huffman_compression/bin/run $(FLAGS)

# LZW build

lzw_compression/build/dictionary.o:
		$(CC) -c lzw_compression/lib/dictionary.c -o lzw_compression/build/dictionary.o $(FLAGS)

lzw_compression/build/file_stream.o:
		$(CC) -c lzw_compression/lib/file_stream.c -o lzw_compression/build/file_stream.o $(FLAGS)

lzw_compression/build/decode.o:
		$(CC) -c lzw_compression/lib/decode.c -o lzw_compression/build/decode.o $(FLAGS)

lzw_compression/build/encode.o:
		$(CC) -c lzw_compression/lib/encode.c -o lzw_compression/build/encode.o $(FLAGS)

lzw_compression/build/run.o:
		$(CC) -c lzw_compression/src/run.c -o lzw_compression/build/run.o $(FLAGS)

# Huffman build 

huffman_compression/build/dictionary.o:
		$(CC) -c huffman_compression/lib/dictionary.c -o huffman_compression/build/dictionary.o $(FLAGS)

huffman_compression/build/file_stream.o:
		$(CC) -c huffman_compression/lib/file_stream.c -o huffman_compression/build/file_stream.o $(FLAGS)

huffman_compression/build/decode.o:
		$(CC) -c huffman_compression/lib/decode.c -o huffman_compression/build/decode.o $(FLAGS)

huffman_compression/build/encode.o:
		$(CC) -c huffman_compression/lib/encode.c -o huffman_compression/build/encode.o $(FLAGS)

huffman_compression/build/run.o:
		$(CC) -c huffman_compression/src/run.c -o huffman_compression/build/run.o $(FLAGS)

# GUI build

build/gui.o:
		$(CC) -c lib/gui.c -o build/gui.o $(FLAGS) $(GUI_FLAGS)

build/run.o:
		$(CC) -c src/run.c -o build/run.o $(FLAGS) $(GUI_FLAGS)

clean:
	rm build/* bin/* huffman_compression/build/* huffman_compression/bin/* lzw_compression/build/* lzw_compression/bin/*
