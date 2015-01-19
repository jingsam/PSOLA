program = psola

CXX = mpicxx
CPPFLAGS =
CXXFLAGS = -Wall -W -O2 -Wno-sign-compare
LDFLAGS = -L/usr/local/lib
LIBS = -lgdal

objs = arg/arg_parser.o xml/tinyxml2.o \
	main.o pso.o stats.o \
	transition.o init.o raster.o \
	option.o parameter.o tool.o


$(program) : $(objs)
	$(CXX) -o $@ $(objs) $(LDFLAGS) $(LIBS)

.PHONY : clean
clean :
	-rm -rf $(objs) $(program)
