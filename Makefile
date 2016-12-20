clv = `clang++ --version`
gpp = `g++ --version`
cpp = `c++ --version`
ifneq ("",$(clv))
	CC = clang++
else ifneq ("",$(gpp))
	CC = g++
else ifneq ("",$(cpp))
	CC = c++
endif

ALLDEP = tmp/RNNet.o tmp/debugRNN.o tmp/DictCreate.o

ALL:$(ALLDEP)
	@echo $(CC)
	$(CC) -o bin/test $(ALLDEP) -lpthread -std=c++11
	-rm $(ALLDEP)

tmp/RNNet.o:
	$(CC) -c src/RNNet.cpp -o tmp/RNNet.o -std=c++11
tmp/debugRNN.o:
	$(CC) -c src/debugRNNet.cpp -o tmp/debugRNN.o -std=c++11
tmp/DictCreate.o:
	$(CC) -c src/DictCreate.cpp -o tmp/DictCreate.o -std=c++11

.PHONY:clean
clean:
	-rm $(ALLDEP)
