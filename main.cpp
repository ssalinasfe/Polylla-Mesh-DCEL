#include <algorithm>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <polylla.hpp>

//#include <triangulation.hpp>

//#include <compresshalfedge.hpp>
//#include <io_void.hpp>
//#include <delfin.hpp>
//

int main(int argc, char **argv) {


    if(argc < 5)
    {
        std::cout<<"Usage: "<<argv[0]<<" <node_file .node> <ele_file .ele> <neigh_file .neigh> <output .off>"<<std::endl;
		std::cout<<"Usage: "<<argv[0]<<" <node_file .node> <ele_file .ele> <neigh_file .neigh> <output .off> <output .hedge>"<<std::endl;
        return 0;
    }

	std::string node_file = std::string(argv[1]);
	std::string ele_file = std::string(argv[2]);
	std::string neigh_file = std::string(argv[3]);
	std::string output_off = std::string(argv[4]);
	

    if(node_file.substr(node_file.find_last_of(".") + 1) != "node"){
        std::cout<<"Error: node file must be .node"<<std::endl;
        return 0;
    }
    if(ele_file.substr(ele_file.find_last_of(".") + 1) != "ele"){
        std::cout<<"Error: ele file must be .ele"<<std::endl;
        return 0;
    }
    if(neigh_file.substr(neigh_file.find_last_of(".") + 1) != "neigh"){
        std::cout<<"Error: neigh file must be .neigh"<<std::endl;
        return 0;
    }
	
	Polylla mesh(node_file, ele_file, neigh_file);
	std::cout<<"output off in "<<output_off<<std::endl;
	mesh.print_OFF(output_off);
    if(argc == 6){
        std::string output_hedge = std::string(argv[5]);
        std::cout<<"output hedge in "<<output_hedge<<std::endl;
        mesh.print_hedge(output_hedge);
    }

	return 0;
}
