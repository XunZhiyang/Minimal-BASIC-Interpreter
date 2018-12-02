PROGRAM = Basic

CXX = g++
CXXFLAGS = -std=c++17 -IStanfordCPPLib -fvisibility-inlines-hidden -g

CPP_FILES = $(wildcard *.cpp)
H_FILES = $(wildcard *.h)

LDOPTIONS = -L.
LIB = -lStanfordCPPLib

all: clean code spl.jar

code: $(CPP_FILES) $(H_FILES) libStanfordCPPLib.a
	$(CXX) -o code $(CXXFLAGS) $(LDOPTIONS) $(CPP_FILES) $(LIB)

libStanfordCPPLib.a:
	@rm -f ./libStanfordCPPLib.a
	(cd ./StanfordCPPLib; make all)
	ln -s ./StanfordCPPLib/libStanfordCPPLib.a .

spl.jar:
	ln -s ./StanfordCPPLib/spl.jar .

tidy:
	rm -f ,* .,* *~ core a.out *.err

clean scratch: tidy
	rm -f *.o *.a $(PROGRAM) spl.jar score
