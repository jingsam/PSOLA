#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>

#include "arg/arg_parser.h"
#include "xml/tinyxml2.h"
#include "option.h"
#include "raster.h"

/*************** global variable initialization *************/
Random* RND;
int g_size                        = 50;
int g_max                         = 9;  
int g_seed                        = 0;
double g_momentum                 = 1.0;
double g_c1                       = 2.0;
double g_c2                       = 2.0;
int g_r1                          = 100;
int g_r2                          = 200;
int g_generation                  = 50;
std::string g_output              = "result";
int g_interval                    = 10;

int g_xsize                       = 0; 
int g_ysize                       = 0; 
int g_nodata                      = 255; 
Map<int> g_region_map;
Map<int> g_land_use_map;
Map<double> g_arable_suit_map;
Map<double> g_orchard_suit_map;
Map<double> g_forest_suit_map;
Map<double> g_grass_suit_map;
Map<double> g_urban_suit_map;
Map<double> g_rural_suit_map;
Map<double> g_slope_map;
Map<double> g_road_map;
std::vector<int> g_land_use_struct;
std::vector<double> g_obj_weights;

std::string g_region;
std::string land_use_map;
std::string arable_suit_map;
std::string orchard_suit_map;
std::string forest_suit_map;
std::string grass_suit_map;
std::string urban_suit_map;
std::string rural_suit_map;
std::string slope_map;
std::string road_map;


const char * const program_name = "PSOLA";
const char * const program_year = "2013";
const char * const program_version = "2.0";
const char * const program_author = "abc@whu.edu.cn";
const char *       invocation_name = 0;
bool               display_config = false;

const Arg_parser::Option options[] = {
    { 'h', "help",              Arg_parser::no  },
    { 'v', "version",           Arg_parser::no  },
    { 'd', "display",           Arg_parser::no  },

    { 'N', "size",              Arg_parser::yes },
    { 'M', "max",               Arg_parser::yes },	
    { 's', "seed",              Arg_parser::yes },
    { 'm', "momentum",          Arg_parser::yes },
    { 'c', "c1",                Arg_parser::yes },
    { 'C', "c2",                Arg_parser::yes },
    { 'r', "r1",                Arg_parser::yes },
    { 'R', "r2",                Arg_parser::yes },
    { 'g', "generation",        Arg_parser::yes },
    { 'o', "output",            Arg_parser::yes },
    { 'p', "prefix",            Arg_parser::yes },
    { 'i', "interval",          Arg_parser::yes },

    { 'z', "region-map",        Arg_parser::yes }, 
    { 'l', "land-use-map",      Arg_parser::yes },
    { 'A', "arable-suit-map",   Arg_parser::yes },
    { 'O', "orchard-suit-map",  Arg_parser::yes },
    { 'F', "forest-suit-map",   Arg_parser::yes },
    { 'G', "grass-suit-map",    Arg_parser::yes },
    { 'U', "urban-suit-map",    Arg_parser::yes },
    { 'V', "rural-suit-map",    Arg_parser::yes },
    { 256, "slope-map",         Arg_parser::yes },
    { 257, "road-map",          Arg_parser::yes },    
    { 'Q', "land-use-struct",   Arg_parser::yes },
    { 'w', "obj-weights",       Arg_parser::yes },

    { 299, "xml-file",          Arg_parser::yes },

    // end of options
    {   0, 0,                   Arg_parser::no  } };

/*************** end variable initialization *************/


void show_help()
{
    std::printf( "%s - Particle Swarm Optimization for Land-use Allocation.\n", program_name );
    std::printf( "Usage: %s [options]\n", invocation_name );
    std::printf( "  -h, --help                   display this help and exit\n"
                 "  -v, --version                display version information and exit\n"
                 "  -d, --display                display your configuration\n"

                 "\noptions for PSO:\n"
                 "  -N, --size=<arg>             number of particles in a deme\n"
		 "  -M, --max=<arg>              value range [1, max]\n"
                 "  -s, --seed=<arg>             general random seed\n"
                 "  -m, --momentum=<arg>         coefficient of keep current status\n"
                 "  -c, --c1=<arg>               coefficient of move to Pbest\n"
                 "  -C, --c2=<arg>               coefficient of move to Gbest\n"
                 "  -r, --r1=<arg>               random seed 1 of PSO\n"
                 "  -R, --r2=<arg>               random seed 2 of PSO\n"
                 "  -g, --generation=<arg>       total number of iterations\n"
                 "  -o, --output=<arg>           output directory\n"
                 "  -i, --interval=<arg>         frequency of output\n"

                 "\noptions for land-use allocation:\n"
                 "  -z, --region-map=<arg>       map of optimal region\n"
                 "  -l, --land-use-map=<arg>     land-use map\n"
                 "  -A, --arable-suit-map=<arg>  suitability map of arable land-use\n"
                 "  -O, --orchard-suit-map=<arg> suitability map of orchard land-use\n"
                 "  -F, --forest-suit-map=<arg>  suitability map of forest land-use\n"
                 "  -G, --grass-suit-map=<arg>   suitability map of grass land-use\n"
                 "  -U, --urban-suit-map=<arg>   suitability map of urban land-use\n"
                 "  -V, --rural-suit-map=<arg>   suitability map of rural land-use\n"
                 "      --slope-map=<arg>        slope map (unit: degree)\n"
                 "      --road-map=<arg>         road map (unit: meter)\n"
		 "  -Q, --land-use-struct=<arg>  land-use structure (splited by comma)\n"
                 "  -w, --obj-weights=<arg>      weights of objectives (splited by comma)\n"                 

                 "\nyou can use xml configuration :\n"
                 "      --xml-file=<arg>         xml file for configuration\n" );
    std::exit( 0 );
}

void show_version()
{
    std::printf( "%s %s (%s)\n", program_name, program_version, program_year);
    std::printf( "Developed by %s\n", program_author);
    std::exit( 0 );
}


void show_error(const char * const msg)
{
    if( msg && msg[0] ) {
      std::fprintf( stderr, "%s: %s\n", program_name, msg );
      std::fprintf( stderr, "Try '%s --help' for more information.\n", 
        invocation_name );
    }
}

void init_option()
{

}

int parse_option(const int argc, const char * const argv[])
{
    invocation_name = argv[0];

    if ( argc < 2) { show_help(); return 0; }

    const Arg_parser parser( argc, argv, options );

    if( parser.error().size() ) { 
        show_error( parser.error().c_str());
        std::exit(1);
    }

    init_option();

    for( int argind = 0; argind < parser.arguments(); ++argind )
    {
        const int code  = parser.code( argind );
        std::string arg = parser.argument( argind );
        if( !code ) break;
        switch( code ) {
            case 'h': show_help();                  return 0;    
            case 'v': show_version();               return 0;
            case 'd': display_config = true;        break;

            case 'N': set_size( arg );              break;
	    case 'M': set_max( arg );               break;
            case 's': set_seed( arg );              break;
            case 'm': set_momentum( arg );          break;
            case 'c': set_c1( arg );                break;
            case 'C': set_c2( arg );                break;
            case 'r': set_r1( arg );                break;
            case 'R': set_r2( arg );                break;
            case 'g': set_generation( arg );        break;
            case 'o': set_output( arg );            break;
            case 'i': set_interval( arg );          break;

            case 'z': set_region_map( arg );        break;
            case 'l': set_land_use_map( arg );      break;
            case 'A': set_arable_suit_map( arg );   break;
            case 'O': set_orchard_suit_map( arg );  break;
            case 'F': set_forest_suit_map( arg );   break;
            case 'G': set_grass_suit_map( arg );    break;
            case 'U': set_urban_suit_map( arg );    break;
            case 'V': set_rural_suit_map( arg );    break;
            case 256: set_slope_map( arg );         break;
            case 257: set_road_map( arg );          break;            
            case 'Q': set_land_use_struct( arg );   break;
	    case 'w': set_obj_weights( arg );       break;

            case 299: parse_xml_file( arg );        break;
            default : break;
        }
    }

    if (display_config) show_option(); 

    return 0;
}

void set_option(const std::string& opt, const std::string& arg)
{

    if      (opt == "size")                 set_size(arg);
    else if (opt == "max")                  set_max(arg);
    else if (opt == "seed")                 set_seed(arg);
    else if (opt == "momentum")             set_momentum(arg);
    else if (opt == "c1")                   set_c1(arg);
    else if (opt == "c2")                   set_c2(arg);
    else if (opt == "r1")                   set_r1(arg);
    else if (opt == "r2")                   set_r2(arg);
    else if (opt == "generation")           set_generation(arg);
    else if (opt == "output")               set_output(arg);
    else if (opt == "interval")             set_interval(arg);

    else if (opt == "region-map")           set_region_map(arg);
    else if (opt == "land-use-map")         set_land_use_map(arg);
    else if (opt == "arable-suit-map")      set_arable_suit_map(arg);
    else if (opt == "orchard-suit-map")     set_orchard_suit_map(arg);
    else if (opt == "forest-suit-map")      set_forest_suit_map(arg);
    else if (opt == "grass-suit-map")       set_grass_suit_map(arg);
    else if (opt == "urban-suit-map")       set_urban_suit_map(arg);
    else if (opt == "rural-suit-map")       set_rural_suit_map(arg);
    else if (opt == "slope-map")            set_slope_map(arg);
    else if (opt == "road-map")             set_road_map(arg);    
    else if (opt == "land-use-struct")      set_land_use_struct(arg);
    else if (opt == "obj-weights")          set_obj_weights(arg);

    else if (opt == "xml-file")             parse_xml_file(arg);
}


void show_option() {
    std::printf("\nConfiguration for PSO:\n");
    std::printf("size:                    %d\n", g_size);
    std::printf("max:                     %d\n", g_max);
    std::printf("seed:                    %d\n", g_seed);
    std::printf("momentum:                %f\n", g_momentum);
    std::printf("c1:                      %f\n", g_c1);
    std::printf("c2:                      %f\n", g_c2);
    std::printf("r1:                      %d\n", g_r1);
    std::printf("r2:                      %d\n", g_r2);
    std::printf("generation:              %d\n", g_generation);
    std::printf("output:                  %s\n", g_output.c_str());
    std::printf("interval:                %d\n", g_interval);

    std::printf("\nConfiguration for land-use allocation:\n");
    std::printf("region-map:              %s (%d, %d)\n", 
        g_region.c_str(),  		  g_region_map.xsize,       g_region_map.ysize);
    std::printf("land-use-map:            %s (%d, %d)\n", 
        land_use_map.c_str(),     g_land_use_map.xsize,     g_land_use_map.ysize);
    std::printf("arable-suit-map:         %s (%d, %d)\n", 
        arable_suit_map.c_str(),  g_arable_suit_map.xsize,  g_arable_suit_map.ysize);
    std::printf("orchard-suit-map:        %s (%d, %d)\n", 
        orchard_suit_map.c_str(), g_orchard_suit_map.xsize, g_orchard_suit_map.ysize);
    std::printf("forest-suit-map:         %s (%d, %d)\n", 
        forest_suit_map.c_str(),  g_forest_suit_map.xsize,  g_forest_suit_map.ysize);
    std::printf("grass-suit-map:          %s (%d, %d)\n", 
        grass_suit_map.c_str(),   g_grass_suit_map.xsize,   g_grass_suit_map.ysize);
    std::printf("urban-suit-map:          %s (%d, %d)\n", 
        urban_suit_map.c_str(),   g_urban_suit_map.xsize,   g_urban_suit_map.ysize);
    std::printf("rural-suit-map:          %s (%d, %d)\n", 
        rural_suit_map.c_str(),   g_rural_suit_map.xsize,   g_rural_suit_map.ysize);
    std::printf("slope-map:               %s (%d, %d)\n", 
        slope_map.c_str(),        g_road_map.xsize,         g_road_map.ysize);
    std::printf("road-map:                %s (%d, %d)\n", 
        road_map.c_str(),         g_slope_map.xsize,        g_slope_map.ysize);
	
    std::printf("land-use-struct:         ");
    for (int i = 0; i < g_land_use_struct.size(); ++i) {
        std::printf("%d ", g_land_use_struct.at(i));
    }
    std::printf("\n");
	
    std::printf("obj-weights:             ");
    for (int i = 0; i < g_obj_weights.size(); ++i) {
        std::printf("%.2f ", g_obj_weights.at(i));
    }
    std::printf("\n");
}

void clean_option() {
	delete RND;
}


void set_size(const std::string& arg) {
    g_size = std::atoi( arg.c_str() );
}

void set_max(const std::string& arg) {
    g_max = std::atoi( arg.c_str() );
}

void set_seed(const std::string& arg) {
    g_seed = std::atoi( arg.c_str() );    
} 

void set_momentum(const std::string& arg) {
    g_momentum = std::atof( arg.c_str() );
}  

void set_c1(const std::string& arg) {
    g_c1 = std::atof( arg.c_str() );
}  

void set_c2(const std::string& arg) {
    g_c2 = std::atof( arg.c_str() );
}

void set_r1(const std::string& arg) {
    g_r1 = std::atof( arg.c_str() );
}  

void set_r2(const std::string& arg) {
    g_r2 = std::atof( arg.c_str() );
}  

void set_generation(const std::string& arg) {
    g_generation = std::atoi( arg.c_str() );
} 

void set_output(const std::string& arg) {
    g_output = arg;
}

void set_interval(const std::string& arg) {
    g_interval = std::atoi( arg.c_str() );
}

template <typename T>
void set_map(const std::string& filename, Map<T>& map) {
    int xsize = getRasterXSize( filename.c_str() );
    int ysize = getRasterYSize( filename.c_str() );
    readRaster( map, filename.c_str() );
    map.xsize = xsize;
    map.ysize = ysize;

}

void set_region_map(const std::string& arg) {
    set_map( arg, g_region_map );

    g_xsize = getRasterXSize( arg.c_str() );
    g_ysize = getRasterYSize( arg.c_str() );
    g_nodata = (int)getRasterNoDataValue( arg.c_str() );
	
    g_region = arg;
}

void set_land_use_map(const std::string& arg) {
    set_map( arg, g_land_use_map );

    land_use_map = arg;
} 

void set_arable_suit_map(const std::string& arg) {
    set_map( arg, g_arable_suit_map );

    arable_suit_map = arg;
}   

void set_orchard_suit_map(const std::string& arg) {
    set_map( arg, g_orchard_suit_map );

    orchard_suit_map = arg;
}  

void set_forest_suit_map(const std::string& arg) {
    set_map( arg, g_forest_suit_map );

    forest_suit_map = arg;
}  

void set_grass_suit_map(const std::string& arg) {
    set_map( arg, g_grass_suit_map );

    grass_suit_map = arg;
}   

void set_urban_suit_map(const std::string& arg) {
    set_map( arg, g_urban_suit_map );

    urban_suit_map = arg;
}   

void set_rural_suit_map(const std::string& arg) {
    set_map( arg, g_rural_suit_map );   

    rural_suit_map = arg;
}   

void set_slope_map(const std::string& arg) {
    set_map( arg, g_slope_map );

    slope_map = arg;
}        

void set_road_map(const std::string& arg) {
    set_map( arg, g_road_map );    

    road_map = arg;
}

void set_values(std::vector<int>& values, const std::string& arg) {
    values.clear();
	char *str = const_cast<char *> (arg.c_str());
    const char *delimiters = " ,/";
    char *token = strtok(str, delimiters);
    while (token) {
        values.push_back(std::atoi(token));
        token = strtok(NULL, delimiters);
    }
}

void set_values(std::vector<double>& values, const std::string& arg) {
    values.clear();
	char *str = const_cast<char *> (arg.c_str());
    const char *delimiters = " ,/";
    char *token = strtok(str, delimiters);
    while (token) {
        values.push_back(std::atof(token));
        token = strtok(NULL, delimiters);
    }
}

void set_land_use_struct(const std::string& arg) {
    set_values( g_land_use_struct, arg );
}

void set_obj_weights(const std::string& arg) {
    set_values( g_obj_weights, arg );
}

void parse_xml(tinyxml2::XMLDocument *doc) {
    tinyxml2::XMLElement *root = doc->RootElement();
    tinyxml2::XMLElement *entry = root->FirstChildElement();
    while (entry) {
        const char *opt = entry->Attribute("key");
        const char *arg = entry->GetText();
        if (opt != NULL && arg != NULL) set_option(opt, arg);        

        entry = entry->NextSiblingElement();
    }
}

void parse_xml_file(const std::string& arg) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile( arg.c_str() );

    parse_xml(&doc);
}
