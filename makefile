program = psola

CXX = mpicxx
CPPFLAGS =
CXXFLAGS = -std=c++11 -Wall -W -O2 -Wno-sign-compare
LDFLAGS = -L/usr/local/lib
LIBS = -lgdal

objs = main.o option.o raster.o \
	arg/arg_parser.o xml/tinyxml2.o \
	pso.o stats.o rule.o \
	init.o tool.o output.o


$(program) : $(objs)
	$(CXX) -o $@ $(objs) $(LDFLAGS) $(LIBS)

.PHONY : clean
clean :
	-rm -rf $(objs) $(program)
