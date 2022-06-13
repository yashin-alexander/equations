all:
	mkdir -p build
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=OFF
	cmake --build build 

clean:
	rm -rf build
