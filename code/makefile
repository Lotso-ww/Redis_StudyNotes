.PHONY:all
all:hello genertic

hello:hello.cc
	g++ -o $@ $^ /usr/local/lib/libredis++.a /usr/lib/x86_64-linux-gnu/libhiredis.a -pthread -std=c++17
genertic:genertic.cc
	g++ -o $@ $^ /usr/local/lib/libredis++.a /usr/lib/x86_64-linux-gnu/libhiredis.a -pthread -std=c++17
.PHONY:clean
clean:
	rm -rf hello