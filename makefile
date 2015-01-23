CXX = mpicxx
CPPFLAGS =
CXXFLAGS = -Wall -W -O2 -Wno-sign-compare
LDFLAGS = -L/usr/local/lib
LIBS = -lgdal

DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

TARGET = $(DIR_BIN)/psola

SRC_CC = $(shell find $(DIR_SRC) -name '*.cc')
SRC_CPP = $(shell find $(DIR_SRC) -name '*.cpp')
SRC = $(SRC_CC) $(SRC_CPP)
OBJ = $(addsuffix .o, $(basename $(SRC)))

$(TARGET) : $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

.PHONY : clean
clean :
	rm -rf $(OBJ) $(TARGET)
