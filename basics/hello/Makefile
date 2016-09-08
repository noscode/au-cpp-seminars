./bin/hello: ./src/main.cpp ./src/hello.h
	mkdir -p bin
	g++ -Wall -Werror ./src/main.cpp -o ./bin/hello

clean:
	rm -rf bin/*

.PHONY: clean
