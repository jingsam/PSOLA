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
Random* g_RND;
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

double g_core                     = 1.0;
double g_edge                     = 1.0;
int g_edge_depth                  = 1;
int g_num_arable;
int g_num_urban;
double g_weight_suit;
double g_weight_prox;
double g_weight_unchange;


int g_xsize                       = 0;
int g_ysize                       = 0;
int g_nodata                      = 255;
std::string land_use_map;
Map<int> g_land_use_map;
Map<double> g_arable_suit_map;
Map<double> g_orchard_suit_map;
Map<double> g_forest_suit_map;
Map<double> g_grass_suit_map;
Map<double> g_build_suit_map;
Map<double> g_slope_map;
Map<double> g_road_map;
Map<double> g_soil_depth_map;


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
    { 'i', "interval",          Arg_parser::yes },
    { 'x', "xml",               Arg_parser::yes },

    // end of options
    {   0, 0,                   Arg_parser::no  } };

/*************** end variable initialization *************/


void show_help()
{
    std::printf( "%s - Particle Swarm Optimization for Land-use Allocation.\n", program_name );
    std::printf( "Usage: %s [options]\n", invocation_name );
    std::printf( "  -h, --help                   display this help and exit\n"
                 "  -v, --version                display version information and exit\n"
                 "  -d, --display                display your configuration\n\n"

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
                 "  -x, --xml-file=<arg>         xml file for configuration\n" );
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

int parse_option(const int argc, const char * const argv[])
{
    invocation_name = argv[0];

    if ( argc < 2) { show_help(); return 0; }

    const Arg_parser parser( argc, argv, options );

    if( parser.error().size() ) {
        show_error( parser.error().c_str());
        std::exit(1);
    }

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
            case 'x': parse_xml( arg );             break;
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
    else if (opt == "xml")                  parse_xml(arg);
}


void show_option() {
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
    std::printf("\n");
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

void set_land_use_map(const std::string& arg) {
    set_map( arg, g_land_use_map );

    g_xsize = getRasterXSize( arg.c_str() );
    g_ysize = getRasterYSize( arg.c_str() );
    g_nodata = (int)getRasterNoDataValue( arg.c_str() );
    land_use_map = arg;
}

void set_arable_suit_map(const std::string& arg) {
    set_map( arg, g_arable_suit_map );
}

void set_orchard_suit_map(const std::string& arg) {
    set_map( arg, g_orchard_suit_map );
}

void set_forest_suit_map(const std::string& arg) {
    set_map( arg, g_forest_suit_map );
}

void set_grass_suit_map(const std::string& arg) {
    set_map( arg, g_grass_suit_map );
}

void set_build_suit_map(const std::string& arg) {
    set_map( arg, g_build_suit_map );
}

void set_slope_map(const std::string& arg) {
    set_map( arg, g_slope_map );
}

void set_road_map(const std::string& arg) {
    set_map( arg, g_road_map );
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

void parse_xml(const std::string& arg) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile( arg.c_str() );

    parse_xml(&doc);
}
