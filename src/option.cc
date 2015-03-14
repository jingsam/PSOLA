#include "option.h"
#include <cstdlib>  // exit()
#include <cstdio>   // printf()
#include <string>
#include "arg/arg_parser.h"
#include "xml/tinyxml2.h"
#include "raster.h" // readRaster()
#include "tool.h"   // stoi()


std::map<std::string, std::string> g_option;


const char * const program_name = "PSOLA";
const char * const program_year = "2015";
const char * const program_version = "3.0";
const char * const program_author = "abc@whu.edu.cn";
const char *       invocation_name = "";

const Arg_parser::Option options[] = {
    { 'h', "help",              Arg_parser::no  },
    { 'v', "version",           Arg_parser::no  },

    { 'N', "population",        Arg_parser::yes },
    { 'M', "max",               Arg_parser::yes },
    { 'w', "momentum",          Arg_parser::yes },
    { 'c', "c1",                Arg_parser::yes },
    { 'C', "c2",                Arg_parser::yes },
    { 'r', "r1",                Arg_parser::yes },
    { 'R', "r2",                Arg_parser::yes },
    { 'g', "generation",        Arg_parser::yes },
    { 'i', "interval",          Arg_parser::yes },
    { 'o', "output",            Arg_parser::yes },
    { 'm', "mode",              Arg_parser::yes },
    { 'x', "xml",               Arg_parser::yes },

    // end of options
    {   0, 0,                   Arg_parser::no  } };

void set_option_default();
void show_help();
void show_option();
void show_error(const char * const msg);
void show_version();
void parse_xml(const std::string& arg);

void parse_option(const int argc, const char * const argv[])
{
    invocation_name = argv[0];

    if (argc < 2) {
        show_help();
        return;
    }

    const Arg_parser parser(argc, argv, options);

    if(parser.error().size()) {
        show_error(parser.error().c_str());
        std::exit(1);
    }

    set_option_default();

    for(int i = 0; i < parser.arguments(); ++i)
    {
        const int code  = parser.code(i);
        std::string arg = parser.argument(i);
        switch(code) {
            case 'h': show_help();                      return;
            case 'v': show_version();                   return;

            case 'N': g_option["population"] = arg;     break;
	        case 'M': g_option["max"] = arg;            break;
            case 'w': g_option["momentum"] = arg;       break;
            case 'c': g_option["c1"] = arg;             break;
            case 'C': g_option["c2"] = arg;             break;
            case 'r': g_option["r1"] = arg;             break;
            case 'R': g_option["r2"] = arg;             break;
            case 'g': g_option["generation"] = arg;     break;
            case 'i': g_option["interval"] = arg;       break;
            case 'o': g_option["output"] = arg;         break;
            case 'm': g_option["mode"] = arg;           break;
            case 'x': parse_xml(arg);                   break;
        }
    }
}

void set_option_default()
{
    g_option["population"] = "32";
    g_option["max"] = "10";
    g_option["momentum"] = "1.0";
    g_option["c1"] = "2.0";
    g_option["c2"] = "2.0";
    g_option["r1"] = "100";
    g_option["r2"] = "200";
    g_option["generation"] = "50";
    g_option["interval"] = "5";
    g_option["output"] = "result";
    g_option["mode"] = "async";
}

void show_help()
{
    std::printf("%s - Particle Swarm Optimization for Land-use Allocation.\n", program_name);
    std::printf("Usage: %s [options]\n", invocation_name);
    std::printf("  -h, --help                   display this help and exit\n"
                "  -v, --version                display version information and exit\n\n"

                "  -N, --population=<arg>       number of particles in a deme\n"
                "  -M, --max=<arg>              value range [0, max)\n"
                "  -w, --momentum=<arg>         coefficient of keep current status\n"
                "  -c, --c1=<arg>               coefficient of move to Pbest\n"
                "  -C, --c2=<arg>               coefficient of move to Gbest\n"
                "  -r, --r1=<arg>               random seed 1 of PSO\n"
                "  -R, --r2=<arg>               random seed 2 of PSO\n"
                "  -g, --generation=<arg>       total number of iterations\n"
                "  -i, --interval=<arg>         frequency of output\n"
                "  -o, --output=<arg>           output directory\n"
                "  -m, --mode=<sync|async>      synchronous or asynchronous land-use change\n"
                "  -x, --xml=<arg>              xml file for configuration\n");
    std::exit(0);
}

void show_option()
{
    std::printf("population:    %s\n", g_option["population"].c_str());
    std::printf("max:           %s\n", g_option["max"].c_str());
    std::printf("momentum:      %s\n", g_option["momentum"].c_str());
    std::printf("c1:            %s\n", g_option["c1"].c_str());
    std::printf("c2:            %s\n", g_option["c2"].c_str());
    std::printf("r1:            %s\n", g_option["r1"].c_str());
    std::printf("r2:            %s\n", g_option["r2"].c_str());
    std::printf("generation:    %s\n", g_option["generation"].c_str());
    std::printf("interval:      %s\n", g_option["interval"].c_str());
    std::printf("output:        %s\n", g_option["output"].c_str());
    std::printf("mode:          %s\n", g_option["mode"].c_str());
}

void show_error(const char * const msg)
{
    if(msg && msg[0]) {
      std::fprintf(stderr, "%s: %s\n", program_name, msg);
      std::fprintf(stderr, "Try '%s --help' for more information.\n",
        invocation_name);
    }
}

void show_version()
{
    std::printf("%s %s (%s)\n", program_name, program_version, program_year);
    std::printf("Developed by %s\n", program_author);
    std::exit(0);
}

void parse_xml(const std::string& arg)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(arg.c_str());

    tinyxml2::XMLElement *root = doc.RootElement();
    tinyxml2::XMLElement *entry = root->FirstChildElement();
    while (entry) {
        const char *opt = entry->Attribute("key");
        const char *arg = entry->GetText();
        if (opt != NULL && arg != NULL) g_option[opt] = arg;

        entry = entry->NextSiblingElement();
    }
}
