.PHONY: build

CC=gcc
APP=floyd

hash = $(shell git log --pretty=format:'%h' -n 1)

clean:
	rm -rf build

build_cli:
	mkdir -p ./build

	$(CC) -ggdb \
		./src/tui.c \
		./src/audio.c \
		./src/log.c \
		./src/keyboard.c \
		./src/main_cli.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP).debug -lm

build_gui:
	mkdir -p ./build

	$(CC) -ggdb \
		./src/audio.c \
		./src/log.c \
		./src/keyboard.c \
		./src/main_gui.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP).gui.debug -lm

release_cli:
	mkdir -p ./build

	$(CC) -O2 \
		./src/tui.c \
		./src/audio.c \
		./src/log.c \
		./src/keyboard.c \
		./src/main_cli.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP) -lm

package_debian: release_cli
	mkdir -p ./dist/debian/floyd/usr/bin
	cp ./build/floyd ./dist/debian/floyd/usr/bin/floyd
	cd ./dist/debian; dpkg-deb --build floyd

run:
	./build/floyd.debug ./test_data/The\ Llama\'s.mp3
