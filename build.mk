CFLAGS = -Wall -std=c11 -I "include/" -I "src/" -I "src/gui/" -L "lib/" -lopengl32 -lglfw3 -lgdi32

"build/main.exe": folder build/glad.o main.o build/stb.o
	gcc "build/glad.o" "build/main.o" "build/stb.o" -o "build/main.exe" $(CFLAGS)

folder:
	if not exist "build/" (mkdir build)

build/glad.o: include/glad/glad.c
	gcc -c "include/glad/glad.c" -o "build/glad.o" $(CFLAGS)

main.o: src/main.c
	gcc -c "src/main.c" -o "build/main.o" $(CFLAGS)

build/stb.o: include/stb/stb.c
	gcc -c "include/stb/stb.c" -o "build/stb.o" $(CFLAGS)
