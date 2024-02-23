build: src/main.c
	x86_64-w64-mingw32-gcc "src/main.c" -o "bin/Console Chip-8.exe"

clean:
	# del "bin\*.*" /s /q
	rm ./bin/*