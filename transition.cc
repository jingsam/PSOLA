#include "transition.h"
#include "option.h"

void normalize_p(std::vector<double>& p);
int roulette_wheel(std::vector<double>& p, Random* rnd);
std::vector<int> merge_rule(std::vector<int>& rule1, std::vector<int>& rule2);
std::vector<double> take_rule(std::vector<int>& rule, std::vector<double>& p);


void grain_for_green(Cell* mycell);
std::vector<int> quantity_constraint(Cell* mycell);



int transition(Cell* mycell)
{
    // rules
    std::vector<int> rule1 = quantity_constraint( mycell );

    // transition
    std::vector<double> p = take_rule( rule1, mycell->transP );
    // mycell->value = roulette_wheel(p, &RND);

    mycell->value = roulette_wheel(mycell->transP, RND);

    return mycell->value;
}


std::vector<int> merge_rule(std::vector<int>& rule1, std::vector<int>& rule2)
{
    std::vector<int> result = rule1;
    for (int i = 0; i < result.size(); ++i) {
        if (rule2.at(i) == 0) result.at(i) = 0;
    }

    return result;
}

std::vector<double> take_rule(std::vector<int>& rule, std::vector<double>& p) 
{
    std::vector<double> result = p;
    for (int i = 0; i < p.size(); ++i) {
        if (rule.at(i) == 0) result.at(i) = 0;
    }
    normalize_p(result);

    return result;
}

int classify_land_use(Cell* mycell)
{
    int land_use = g_land_use_map.atxy(mycell->x, mycell->y);
    switch (land_use) {
        case 11:  case 12:  case 13:    return 1;
        case 21:  case 22:  case 23:    return 2;
        case 31:  case 32:  case 33:    return 3;
        case 41:  case 42:  case 43:    return 4;
        case 201: case 202: case 204: 
        case 205:                       return 5;
        case 203:                       return 6;
        case 101: case 102: case 103:
        case 104: case 105: case 106:
        case 107:                       return 7;
        case 111: case 112: case 113:
        case 114: case 115: case 116:
        case 117: case 118: case 119:   return 8;
        case 121: case 122: case 123:
        case 124: case 125: case 126:
        case 127:                       return 9;
        default:                        return g_nodata;
    }  
}

void set_cell_type(Cell* mycell)
{
    int land_use = g_land_use_map.atxy(mycell->x, mycell->y);
	if (land_use == g_nodata) {
		mycell->value = g_nodata;
		mycell->type = kBackgroundCell;
		return;
	}
	
    switch (land_use) {
        case 11:  case 12:  case 13:
            grain_for_green(mycell);
            break;
        case 201: case 202: case 204:
        case 205:
            mycell->value = 5;
            mycell->type = kDeterminedCell;
            break;
        case 101: case 102: case 103:
        case 104: case 105: case 106:
        case 107:
            mycell->value = 7;
            mycell->type = kExcludedCell;
            break;    
        case 111: case 112: case 113:
        case 114: case 115: case 116:
        case 117: case 118: case 119:
            mycell->value = 8;
            mycell->type = kExcludedCell;
            break;
        case 121: case 122: case 123:
        case 124: case 125: case 126:
        case 127:
            mycell->value = 9;
            mycell->type = kExcludedCell;
            break;
        default:
            mycell->type = kNormalCell;
            break;
    }
    
}

void grain_for_green(Cell* mycell)
{
    double slope = g_slope_map.atxy(mycell->x, mycell->y);
    if (slope >= 25.0) {
        mycell->value = 3;
        mycell->type = kDeterminedCell;
    }

}

std::vector<int> quantity_constraint(Cell* mycell) {
    std::vector<int> counts = mycell->map->stats->counts;
    std::vector<int> flags( counts.size(), 0 );
    for (int i = 0; i < counts.size(); ++i) {
        if (counts.at(i) < g_land_use_struct.at(i)) {
            flags.at(i) = 1;
        }
    }

    return flags;
}

std::vector<int> farming_radius(Cell* mycell) {
    std::vector<int> flags( mycell->transP.size(), 1 );

    //std::vector<Cell*> neighbors = mycell->map->

    return flags;
}
