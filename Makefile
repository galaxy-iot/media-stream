cmake:
	mkdir -p build && cd build && cmake ..

build: cmake
	make -C build

clean:
	rm -rf build

.PHONY: clean build