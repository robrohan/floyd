.PHONY: build

CC=clang
APP=floyd

C_ERRS += -Wall -Wextra -Wpedantic \
		-Wformat=2 -Wno-unused-parameter -Wshadow \
		-Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
		-Wredundant-decls -Wnested-externs -Wmissing-include-dirs \
		-Wno-unused

hash = $(shell git log --pretty=format:'%h' -n 1)

clean:
	rm -rf build
	rm -f dist/debian/floyd_cli.deb
	rm -f dist/debian/floyd_cli/usr/bin/floyd
	rm -f dist/macos/FloydCli.app/Contents/MacOS/FloydCli

build:
	mkdir -p ./build

	$(CC) $(C_ERRS) -ggdb -O2 -std=c99 \
		./src/tui.c \
		./src/audio.c \
		./src/log.c \
		./src/keyboard.c \
		./src/main.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP).debug -lm

release_cli:
	mkdir -p ./build

	$(CC) -O2 -std=c99 \
		./src/tui.c \
		./src/audio.c \
		./src/log.c \
		./src/keyboard.c \
		./src/main.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP) -lm

# windows cli doesn't work
# release_windows_cli:
# 	mkdir -p ./build
# 	$(CC) -O2 \
# 	-Wl,--allow-multiple-definition \
# 	-pthread -lopengl32 -lgdi32 -lwinmm \
# 	-mwindows -static \
# 		./src/tui.c \
# 		./src/audio.c \
# 		./src/log.c \
# 		./src/keyboard.c \
# 		./src/main_cli.c \
# 	-I./vendor \
# 	-I./src \
# 	-o ./build/$(APP).exe -lm

package_debian: release_cli
	mkdir -p ./dist/debian/floyd_cli/usr/bin
	cp ./build/floyd ./dist/debian/floyd_cli/usr/bin/floyd
	cd ./dist/debian; dpkg-deb --build floyd_cli floyd_cli.deb

package_macos: release_cli
	cp ./build/floyd ./dist/macos/FloydCli.app/Contents/MacOS/FloydCli

# windows cli doesn't work
# package_windows: release_windows_cli
# 	cp ./build/floyd.exe ./dist/windows/floyd.exe
