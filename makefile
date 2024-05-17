cpp_source_files := $(shell find -name *.cpp)
cpp_object_files := $(patsubst ./src/%.cpp, ./bin/obj/%.o,$(cpp_source_files))

GCC_FLAGS := -O2 -g1 -Wall -fexceptions -std=c++11 -g -Iinclude -c

all: canvas2d.exe
	mv canvas2d.exe ../
	cd .. && gdb canvas2d.exe

run: canvas2d.exe
	cd .. && ./canvas2d.exe

clear:
	rm ./bin -r
	rm canvas2d.exe

clean:
	rm ./bin -r
	rm canvas2d.exe

canvas2d.exe: $(cpp_object_files)
	g++ -O2 -g1 -o canvas2d.exe $(cpp_object_files) lib/libfreeglut32.a lib/libopengl32.a lib/libglu32.a

$(cpp_object_files): bin/obj/%.o : src/%.cpp
	mkdir -p bin
	mkdir -p $(dir $@)
	g++ $(GCC_FLAGS) $(patsubst bin/obj/%.o, src/%.cpp, $@) -o $@




