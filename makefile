program = psola

CXX = mpicxx
CPPFLAGS = 
CXXFLAGS = -Wall -W -g -Wno-sign-compare
LDFLAGS = -L/usr/local/lib
LIBS = -lgdal

objs = main.o option.o raster.o \
		arg/arg_parser.o xml/tinyxml2.o \
		pso.o fitness.o rule.o \
		init.o tool.o output.o


$(program) : $(objs)
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $(objs)
# for cygwin
#	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $(objs) /usr/local/lib/libgdal.dll.a

.PHONY : clean
clean :
	-rm -rf $(objs) $(program)
	
