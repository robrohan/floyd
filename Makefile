.PHONY: build

CC=gcc
APP=floyd

clean:
	rm -rf build

build:
	mkdir -p ./build

	$(CC) -ggdb ./src/audio.c ./src/log.c ./src/keyboard.c ./src/main.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP).debug -lm

release:
	mkdir -p ./build

	$(CC) -O2 ./src/audio.c ./src/log.c ./src/keyboard.c ./src/main.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP) -lm

run:
	./build/floyd.debug ./test_data/The\ Llama\'s.mp3
