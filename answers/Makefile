CXXFLAGS=-I./provided-all/ -std=c++17 -Wall -Wextra -O3 -pthread

all: monte-carlo-pi-async.exe

clean:
	rm -f *.exe *.o

%.exe: %.cxx
	$(CXX) $(CXXFLAGS) -o $@ $<

monte-carlo-pi-parstl.exe: monte-carlo-pi-parstl.cxx
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ $< -ltbb

monte-carlo-pi-parstl-timed.exe: monte-carlo-pi-parstl-timed.cxx
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ $< -ltbb

