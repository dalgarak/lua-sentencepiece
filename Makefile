CXXFLAGS := $(CFLAGS)

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
	$(CXX) -Wall -D__TEST_LUA_SENTPIECE -std=c++11 -o $@ ./lsentencepiece.cpp -lsentencepiece -I/home/jhshin/Works/sentencepiece/src -I/home/jhshin/torch/install/include -I/usr/include/c++/5 -L/home/jhshin/torch/install/lib -lluajit 
	
