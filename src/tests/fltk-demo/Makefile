CXX = `fltk-config --cxx`
CXXFLAGS = -Wall `fltk-config --cxxflags`
LDFLAGS = `fltk-config --ldflags`

all: main

gui.o: gui.cxx
	$(CXX) $(CXXFLAGS) -c $<

main.o: main.cxx
	$(CXX) $(CXXFLAGS) -c $<

main: main.o gui.o
	$(CXX) $(LDFLAGS) $^ -o $@

.PHONY: clean all

clean:
	-rm -v gui.o main.o main
