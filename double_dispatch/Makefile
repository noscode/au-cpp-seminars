all: bin/self bin/rtti

bin/self: src/self.cpp src/shapes_self_dispatch.h
	g++ -Wall -Werror -std=c++11 \
		src/self.cpp src/shapes_self_dispatch.cpp -o $@

bin/rtti: src/shapes.h src/shapes_intersect.h src/double_dispatch.h \
	src/rtti.cpp
	g++ -Wall -Werror -std=c++11 src/rtti.cpp -o $@

clean:
	rm -rf bin/*
