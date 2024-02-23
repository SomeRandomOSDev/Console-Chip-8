build: src/main.c src/chip-8.h src/chip-8_instructions.h src/out.h
	x86_64-w64-mingw32-gcc "src/main.c" -o "bin/Console Chip-8.exe" -lwinmm

clean:
	rm ./bin/*