CXX = g++
RM = rm -f
CP = cp -f
CXXFLAGS += -Wall -D_GNU_SOURCE  -g -DLINUX -O0 -fPIC
INCS = -I. 
LIBS =  

TARGETS = test
all:$(TARGETS)

test: dict.o test.o
	$(CXX) $^ $(LIBS) -o $@

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCS) $< -o $@

clean:
	$(RM) $(TARGETS) *.o *.exe *.pb.h *.pb.cc

install:
	$(CP) $(TARGETS) /tmp/$(TARGETS)

total:
	make clean&&make&&make install
