.PHONY: build

-include .env
export

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

release_mac_cli:
	mkdir -p ./build

	clang -target arm64-apple-macos11 -O2 -std=c99 \
		./src/tui.c \
		./src/audio.c \
		./src/log.c \
		./src/keyboard.c \
		./src/main.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP)-arm64 -lm

	clang -target x86_64-apple-macos10.15 -O2 -std=c99 \
		./src/tui.c \
		./src/audio.c \
		./src/log.c \
		./src/keyboard.c \
		./src/main.c \
	-I./vendor \
	-I./src \
	-o ./build/$(APP)-x86_64 -lm

	lipo -create ./build/$(APP)-arm64 ./build/$(APP)-x86_64 -output ./build/$(APP)

sign:
	codesign --sign "$(DEVELOPER_ID)" --force --options runtime ./build/$(APP)

notarize:
	ditto -c -k --keepParent ./build/$(APP) ./build/$(APP).zip
	xcrun notarytool submit ./build/$(APP).zip \
		--apple-id "$(APPLE_ID)" \
		--password "$(APPLE_APP_PASSWORD)" \
		--team-id "$(TEAM_ID)" \
		--wait
	rm ./build/$(APP).zip

package_macos: release_mac_cli sign notarize
	mkdir -p ./dist/macos
	cp ./build/$(APP) ./dist/macos/$(APP)

# windows cli doesn't work
# package_windows: release_windows_cli
# 	cp ./build/floyd.exe ./dist/windows/floyd.exe
