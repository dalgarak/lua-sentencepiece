CFLAGS := $(CFLAGS) -std=c++11
CXXFLAGS := $(CFLAGS) 
LDFLAGS := $(LDFLAGS) -lsentencepiece

all: clean SentencePieceProcessor.so

SentencePieceProcessor.so: lsentencepiece.o
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

install: SentencePieceProcessor.so 
	cp $< $(INST_LIBDIR)

clean:
	rm -f lsentencepiece.o
	rm -f SentencePieceProcessor.so
	rm -f test

test: clean
	$(CXX) -Wall -D__TEST_LUA_SENTPIECE -std=c++11 -o $@ ./lsentencepiece.cpp -lsentencepiece -I/usr/local/include -I/home/jhshin/torch51/install/include -I/usr/include/c++/5 -L/home/jhshin/torch51/install/lib -lluajit $(LDFLAGS)
	
