CXX = mpicxx
CPPFLAGS =
CXXFLAGS = -Wall -W -O2 -Wno-sign-compare
LDFLAGS = -L/usr/local/lib
LIBS = -lgdal

TARGET = psola

SRC_CC = $(shell find . -name '*.cc')
SRC_CPP = $(shell find . -name '*.cpp')
SRC = $(SRC_CC) $(SRC_CPP)
OBJ = $(addsuffix .o, $(basename $(SRC)))

$(TARGET) : $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

.PHONY : clean
clean :
	rm -rf $(OBJ) $(TARGET)
