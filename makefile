cpp_source_files := $(shell find -name *.cpp)
cpp_object_files := $(patsubst ./src/%.cpp, ./bin/obj/%.o,$(cpp_source_files))

GCC_FLAGS := -Wall -fexceptions -std=c++11 -g -Iinclude -c

all: bin/canvas2d.exe
	./bin/canvas2d.exe

bin/canvas2d.exe: $(cpp_object_files)
	g++ -O2 -o bin/canvas2d.exe $(cpp_object_files) lib/libfreeglut32.a lib/libopengl32.a lib/libglu32.a

$(cpp_object_files): bin/obj/%.o : src/%.cpp
	mkdir -p bin
	mkdir -p $(dir $@)
	g++ $(GCC_FLAGS) $(patsubst bin/obj/%.o, src/%.cpp, $@) -o $@




