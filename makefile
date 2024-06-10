FLAGS :=-Wall -Werror -pedantic -std=c++20 -pthread
files := DynamicHTTP Socket Server Error HTTPResponse ThreadPool ReadAndWrite
objs := $(patsubst %,objs/%.o,${files})
.PHONY: all clean

all: wserver wclient fib.cgi

wserver: objs/wserver.o ${objs}
	g++ ${FLAGS} $^ -o $@

wclient: objs/wclient.o ${objs}
	g++ ${FLAGS} $^ -o $@

fib.cgi: objs/fib.o ${objs}
	g++ ${FLAGS} $^ -o $@

objs/%.o: %.cpp %.h
	mkdir -p objs
	g++ -c ${FLAGS} $< -o $@

# for standalone .cpp files:
objs/%.o: %.cpp
	mkdir -p objs
	g++ -c ${FLAGS} $< -o $@


clean:
	rm -f -r wserver wclient fib.cgi *.o objs

# all my code is on cs1
send: wserver index.html example.html example.js fib.cgi
	scp $^ tkobak@cs2.seattleu.edu:~/wserver