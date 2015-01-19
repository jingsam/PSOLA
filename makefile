program = psola

CXX = mpicxx
CPPFLAGS =
CXXFLAGS = -Wall -W -O2 -Wno-sign-compare
LDFLAGS = -L/usr/local/lib
LIBS = -lgdal

DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

BIN_TARGET = ${DIR_BIN}/${program}

CC_SRC =  $(shell find $(DIR_SRC) -name '*.cc')
CPP_SRC = $(shell find $(DIR_SRC) -name '*.cpp')
SRC = $(CC_SRC)

CC_OBJ = $(patsubst %.cc, ${DIR_OBJ}/%.o, $(notdir ${CC_SRC}))
CPP_OBJ = $(patsubst %.cpp, ${DIR_OBJ}/%.o, $(notdir ${CPP_SRC}))
OBJ = $(CC_OBJ) $(CPP_OBJ)

#SRC = $(foreach d, $(DIR_SRC), $(wildcard $(addprefix $(d)/*, .cc .cpp)))
#OBJ = $(addsuffix .o, $(basename $(SRC)))

${DIR_OBJ}/%.o : ${SRC}
	$(CXX) -c $< -o $@

${BIN_TARGET} : ${OBJ}
	$(CXX) -o $@ ${OBJ} $(LDFLAGS) $(LIBS)

.PHONY : clean
clean :
	rm -rf ${DIR_OBJ}/*.o ${BIN_TARGET}
