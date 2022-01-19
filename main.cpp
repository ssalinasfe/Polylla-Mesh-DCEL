#include <algorithm>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <polylla.hpp>

#include <triangulation.hpp>

//#include <compresshalfedge.hpp>
//#include <io_void.hpp>
//#include <delfin.hpp>
//

int main(int argc, char **argv) {

    if(argc == 5)
    {
        std::string node_file = std::string(argv[1]);
        std::string ele_file = std::string(argv[2]);
        std::string neigh_file = std::string(argv[3]);
        std::string output = std::string(argv[4]);

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
        
        mesh.print_OFF(output+".off");
        std::cout<<"output off in "<<output<<".off"<<std::endl;
        mesh.print_ALE(output+".ale");
        std::cout<<"output ale in "<<output<<".ale"<<std::endl;
    }else if (argc == 3){
        std::string off_file = std::string(argv[1]);
        std::string output = std::string(argv[2]);
	    Polylla mesh(off_file);

        mesh.print_OFF(output+".off");
        std::cout<<"output off in "<<output<<".off"<<std::endl;
        mesh.print_ALE(output+".ale");
        std::cout<<"output ale in "<<output<<".ale"<<std::endl;
    }else{
        std::cout<<"Usage: "<<argv[0]<<" <off file .off> <output name>"<<std::endl;
        std::cout<<"Usage: "<<argv[0]<<" <node_file .node> <ele_file .ele> <neigh_file .neigh> <output name>"<<std::endl;
        return 0;
    }
    

    
    
	return 0;
}
