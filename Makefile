# You may modify Makefile as your like
# but you should keep `make clean` and `make` works correct
SHELL = powershell.exe
PROGRAM = Basic

CXX = g++
CXXFLAGS = -IStanfordCPPLib -fvisibility-inlines-hidden -g -std=c++14

CPP_FILES = $(wildcard *.cpp)
H_FILES = $(wildcard *.h)

LDOPTIONS = -L.
LIB = -lStanfordCPPLib

all:clean $(PROGRAM) spl.jar score

$(PROGRAM): $(CPP_FILES) $(H_FILES) libStanfordCPPLib.a
	$(CXX) -o $(PROGRAM).exe $(CXXFLAGS) $(LDOPTIONS) $(CPP_FILES) $(LIB)

score: ../Test/score
	cmd /c "mklink /H score.exe ..\Test\score.exe"

../Test/score:
	powershell /c "cd ../Test; make"

libStanfordCPPLib.a:
	@if (Test-Path ../libStanfordCPPLib.a) {rm -Force ../libStanfordCPPLib.a}
	powershell /c "cd ../StanfordCPPLib; make all"
	cmd /c "mklink /H libStanfordCPPLib.a ..\StanfordCPPLib\libStanfordCPPLib.a"

spl.jar:
	cmd /c "mklink /H spl.jar ..\StanfordCPPLib\spl.jar"

tidy:
	rm -Force *~
	if (Test-Path core) {rm -Force core}
	if (Test-Path a.out) {rm -Force a.out}
	rm -Force *.err

clean scratch: tidy
	rm -Force *.o
	rm -Force *.a
	if (Test-Path $(PROGRAM).exe) {rm -Force $(PROGRAM).exe}
	if (Test-Path spl.jar) {rm -Force spl.jar}
	if (Test-Path score.exe) {rm -Force score.exe}
