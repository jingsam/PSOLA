program = psola

CXX = mpicxx
CPPFLAGS =
CXXFLAGS = -Wall -W -Wno-sign-compare
LDFLAGS = -L/usr/local/lib
LIBS = -lgdal

objs = main.o option.o raster.o \
	arg/arg_parser.o xml/tinyxml2.o \
	pso.o fitness.o rule.o \
	init.o tool.o output.o parameter.o


$(program) : $(objs)
	$(CXX) -o $@ $(objs) $(LDFLAGS) $(LIBS)

.PHONY : clean
clean :
	-rm -rf $(objs) $(program)
