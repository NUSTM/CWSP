CXX ?= g++
CFLAGS = -std=c++11 -Wall -O3 -fPIC
OBJ = src/CharType.o src/StrFun.o src/SegDict.o src/SegFeat.o src/SegVocab.o src/SegProb.o src/Segmentor.o src/MultiPerceptron.o

all: cwsp_seg
	rm -rf *.o *.a
	cd src; rm -rf *.a *.o
.PHONY: all

convert: convert.o $(OBJ)
	$(CXX) $(CFLAGS) -o convert convert.o $(OBJ)

cwsp_seg: cwsp_seg.o $(OBJ)
	$(CXX) $(CFLAGS) -o cwsp_seg cwsp_seg.o $(OBJ)

cwsp_seg.o: cwsp_seg.cpp
	$(CXX) $(CFLAGS) -I./include -c cwsp_seg.cpp

src/CharType.o: src/CharType.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c CharType.cpp -o CharType.o

src/StrFun.o: src/StrFun.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c StrFun.cpp -o StrFun.o

src/SegDict.o: src/SegDict.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c SegDict.cpp -o SegDict.o 

src/SegFeat.o: src/SegFeat.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c SegFeat.cpp -o SegFeat.o

src/SegVocab.o: src/SegVocab.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c SegVocab.cpp -o SegVocab.o 

src/SegProb.o: src/SegProb.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c SegProb.cpp -o SegProb.o

src/MultiPerceptron.o: src/MultiPerceptron.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c MultiPerceptron.cpp -o MultiPerceptron.o

src/Segmentor.o: src/Segmentor.cpp
	cd src; $(CXX) $(CFLAGS) -I../include -c Segmentor.cpp -o Segmentor.o

clean:
	rm -rf *.o *.a
	cd src; rm -f *.a *.o
	cd data; rm -rf train

cleanall:
	rm -rf cwsp_seg
	rm -rf *.o *.a
	cd src; rm -f *.a *.o
	cd data; rm -rf train
