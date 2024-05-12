.PHONY: build

CC=gcc

build:
	mkdir -p ./build

	$(CC) -ggdb ./src/log.c ./src/keyboard.c ./src/main.c \
	-I./vendor \
	-I./src \
	-o ./build/floyd -lm

run:
	./build/floyd ./test_data/The\ Llama\'s.mp3