all:
	cmake . -B build -DBUILD_TESTS=ON -DDEBUG=ON && cmake --build build	

test: all
	./build/SlugVk-lib-common-TestAll
