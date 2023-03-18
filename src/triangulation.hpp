// https://jerryyin.info/geometry-processing-algorithms/half-edge/
// https://doc.cgal.org/latest/Arrangement_on_surface_2/classCGAL_1_1Arrangement__2_1_1Halfedge.html
// https://threejs.org/docs/#examples/en/math/convexhull/HalfEdge.vertex


// half-edge triangulation
/*
Basic operations
    incident_face(e): return the face incident to e
    twin(e): return the twin halfedge of e
    next(e): return the next halfedge of e
    prev(e): return the previous halfedge of e
    origin(e): return the first vertex of halfedge e
    target(e): return the second vertex of halfedge e
Others
    CCW_edge_to_vertex(e): return the next CCW edge incident to v after e
    edge_of_vertex(v): return A edge incident to v
    is_border_face(e): return true if the incent face of e is a border face
    is_interior(e): return true if the incent face of e is an interior face
    is_border_vertex(e): return true if the vertex v is part of the boundary
    faces(): return number of faces
    halfEdges(): Return number of halfedges
    vertices(): Return number of vertices
    get_Triangles(): bitvector of triangles where true if the halfege generate a unique face, false if the face is generated by another halfedge
    get_PointX(int i): return the i-th x coordinate of the triangulation
    get_PointY(int i): return the i-th y coordinate of the triangulation

TODO:
    edge_iterator;
    face_iterator;
    vertex_iterator;
    copy constructor;
    constructor indepent of triangle
*/

#ifndef TRIANGULATION_HPP
#define TRIANGULATION_HPP

#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include <map>
#include <chrono>

struct vertex{
    double x;
    double y;
    bool is_border = false; // if the vertex is on the boundary
    int incident_halfedge; // halfedge incident to the vertex, vertex is the origin of the halfedge
};



struct halfEdge {
    int origin; //tail of edge
    //int target; //head of edge
    int twin; //opposite halfedge
    int next; //next halfedge of the same face
    int prev; //previous halfedge of the same face
   // int face = -1; //face index incident to the halfedge
    int is_border; //1 if the halfedge is on the boundary, 0 otherwise
};

// Esta fue la unica función ql funciono, porque las weas nativas de c++ funcionan mal
//https://stackoverflow.com/a/22395635
// Returns false if the string contains any non-whitespace characters
// Returns false if the string contains any non-ASCII characters
bool isWhitespace(std::string s){
    for(int index = 0; index < s.length(); index++)
        if(!std::isspace(s[index]))
            return false;
    return true;
}

class Triangulation 
{

private:

    typedef std::array<int,3> _triangle; 
    typedef std::pair<int,int> _edge;

    //Statically data
    int n_halfedges = 0; //number of halfedges
    int n_faces = 0; //number of faces
    int n_vertices = 0; //number of vertices
    int n_border_edges = 0; //number of border edges
    double t_triangulation_generation = 0; //time to generate the triangulation


    std::vector<vertex> Vertices;
    std::vector<halfEdge> HalfEdges; //list of edges
    //std::vector<char> triangle_flags; //list of edges that generate a unique triangles, 
    std::vector<int> triangle_list; //list of edges that generate a unique triangles, 
    


    //Read node file in .node format and nodes in point vector
    void read_nodes_from_file(std::string name){
        std::string line;
        std::ifstream nodefile(name);
        double a1, a2, a3, a4;
        
        //std::cout<<"Node file"<<std::endl;
        if (nodefile.is_open())
        {
            nodefile >> n_vertices;
            //std::cout<<pnumber<<std::endl;
            Vertices.reserve(n_vertices);
            std::getline(nodefile, line); //skip the first line
            while (std::getline(nodefile, line))
            {
                std::istringstream(line) >> a1 >> a2 >> a3 >> a4;
                vertex ve;
                ve.x =  a2;
                ve.y =  a3;
                ve.is_border = (a4 == 1) ? true : false;
                Vertices.push_back(ve);
            }  
        }
        else 
            std::cout << "Unable to open node file"; 
        nodefile.close();
    }

    //Read triangle file in .ele format and stores it in faces vector
    std::vector<int> read_triangles_from_file(std::string name){
        std::vector<int> faces;
        std::string line;
        std::ifstream elefile(name);
        int a1, a2, a3, a4;
        
        //std::cout<<"Node file"<<std::endl;
        if (elefile.is_open())
        {
            elefile >> n_faces;
            //std::cout<<pnumber<<std::endl;
            faces.reserve(3*n_faces);
            std::getline(elefile, line); //skip the first line
            while (std::getline(elefile, line))
            {
                std::istringstream(line) >> a1 >> a2 >> a3 >> a4;
                faces.push_back(a2);
                faces.push_back(a3);
                faces.push_back(a4);
                //std::cout<<"Vertex "<<a1<<" "<<v.x<<" "<<v.y<<" "<<v.is_border<<std::endl;
            }
        }
        else 
            std::cout << "Unable to open node file"; 
        elefile.close();

        return faces;
    }

    //Read node file in .node format and nodes in point vector
    std::vector<int>  read_neigh_from_file(std::string name){
        std::vector<int> neighs;
        std::string line;
        std::ifstream neighfile(name);
        int a1, a2, a3, a4;
        
        //std::cout<<"Node file"<<std::endl;
        if (neighfile.is_open())
        {
            neighfile >> n_faces;
            //std::cout<<pnumber<<std::endl;
            neighs.reserve(3*n_faces);
            std::getline(neighfile, line); //skip the first line
            while (std::getline(neighfile, line))
            {
                if(line[0] == '#'){
                    std::cout<<line<<std::endl;
                    continue;
                }
                std::istringstream(line) >> a1 >> a2 >> a3 >> a4;
                
                neighs.push_back(a2);
                neighs.push_back(a3);
                neighs.push_back(a4);

                // count all number minior than 0 as border edges
                if(a2 < 0){
                    n_border_edges++;
                    //std::cout<<"Border edge 0 "<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<std::endl;
                }
                if(a3 < 0){
                    n_border_edges++;
                    //std::cout<<"Border edge 1 "<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<std::endl;
                }
                if(a4 < 0){
                    n_border_edges++;
                    //std::cout<<"Border edge 2 "<<a1<<" "<<a2<<" "<<a3<<" "<<a4<<std::endl;
                }
            }
        }
        else 
            std::cout << "Unable to open node file"; 
        neighfile.close();
        return neighs;
    }

        //Generate interior halfedges using a a vector with the faces of the triangulation
    //if an interior half-edge is border, it is mark as border-edge
    //mark border-edges
    /*
    void construct_interior_halfEdges_from_faces(std::vector<int> &faces){
        auto hash_for_pair = [](const std::pair<int, int>& p) {
            return std::hash<int>{}(p.first) ^ std::hash<int>{}(p.second);
        };
        std::unordered_map<_edge, int, decltype(hash_for_pair)> map_edges(3*this->n_faces, hash_for_pair); //set of edges to calculate the boundary and twin edges
        for(std::size_t i = 0; i < n_faces; i++){
            halfEdge he0, he1, he2;
            int index_he0 = i*3+0;
            int index_he1 = i*3+1;
            int index_he2 = i*3+2;
            int v0 = faces.at(3*i+0);
            int v1 = faces.at(3*i+1);
            int v2 = faces.at(3*i+2);
            
            he0.origin = v0;
            he0.next = index_he1;
            he0.prev = index_he2;
            //he0.face = i;
            he0.is_border = false;
            he0.twin = -1;
            //falta twin
            Vertices.at(v0).incident_halfedge = index_he0;
            
            map_edges[std::make_pair(v0, v1)] = index_he0;
            HalfEdges.push_back(he0);
            
            he1.origin = v1;
            //he1.target = v2;
            he1.next = index_he2;
            he1.prev = index_he0;
            //he1.face = i;
            he1.is_border = false;
            he1.twin = -1;
            Vertices.at(v1).incident_halfedge = index_he1;
            
            map_edges[std::make_pair(v1, v2)] = index_he1;
            HalfEdges.push_back(he1);

            he2.origin = v2;
            //he2.target = v0;
            he2.next = index_he0;
            he2.prev = index_he1;
            //he2.face = i;
            he2.is_border = false;
            he2.twin = -1;
            Vertices.at(v2).incident_halfedge = index_he2;

            map_edges[std::make_pair(v2, v0)] = index_he2;            
            HalfEdges.push_back(he2);
        }
        this->n_halfedges = HalfEdges.size();

        //Calculate twin halfedge and boundary halfedges from set_edges
        std::unordered_map<_edge,int, decltype(hash_for_pair)>::iterator it;
        for(std::size_t i = 0; i < HalfEdges.size(); i++){
            //if halfedge has no twin
            if(HalfEdges.at(i).twin == -1){
                int tgt = origin(next(i));
                int org = origin(i);
                _edge twin = std::make_pair(tgt, org);
                it=map_edges.find(twin);
                //if twin is found
                if(it!=map_edges.end()){
                    int index_twin = it->second;
                    HalfEdges.at(i).twin = index_twin;
                    HalfEdges.at(index_twin).twin = i;
                }else{ //if twin is not found and halfedge is on the boundary
                    HalfEdges.at(i).is_border = true;
                    Vertices.at(org).is_border = true;
                    Vertices.at(tgt).is_border = true;
                }
            }
        }
    }
*/
    void construct_interior_halfEdges_from_faces(std::vector<int> &faces){
        auto hash_for_pair = [n = faces.size()](const std::pair<int, int>& p) {
            return std::hash<int>{}(p.first)*3*n + std::hash<int>{}(p.second);
        };
        std::unordered_map<_edge, int, decltype(hash_for_pair)> map_edges(3*this->n_faces, hash_for_pair); //set of edges to calculate the boundary and twin edges
        for(std::size_t i = 0; i < n_faces; i++){
            for(std::size_t j = 0; j < 3; j++){
                halfEdge he;
                int v_origin = faces.at(3*i+j);
                int v_target = faces.at(3*i+(j+1)%3);
                he.origin = v_origin;
                he.next = i*3+(j+1)%3;
                he.prev = i*3+(j+2)%3;
                he.is_border = false;
                he.twin = -1;
                Vertices.at(v_origin).incident_halfedge = i*3+j;
                map_edges[std::make_pair(v_origin, v_target)] = i*3+j;
                HalfEdges.push_back(he);
            }
        }
        
        //Calculate twin halfedge and boundary halfedges from set_edges
        std::unordered_map<_edge,int, decltype(hash_for_pair)>::iterator it;
        for(std::size_t i = 0; i < HalfEdges.size(); i++){
            //if halfedge has no twin
            if(HalfEdges.at(i).twin == -1){
                int tgt = origin(next(i));
                int org = origin(i);
                _edge twin = std::make_pair(tgt, org);
                it=map_edges.find(twin);
                //if twin is found
                if(it!=map_edges.end()){
                    int index_twin = it->second;
                    HalfEdges.at(i).twin = index_twin;
                    HalfEdges.at(index_twin).twin = i;
                }else{ //if twin is not found and halfedge is on the boundary
                    HalfEdges.at(i).is_border = true;
                    Vertices.at(org).is_border = true;
                    Vertices.at(tgt).is_border = true;
                }
            }
        }
    }
    //Generate interior halfedges using faces and neigh vectors
    //also associate each vertex with an incident halfedge
    void construct_interior_halfEdges_from_faces_and_neighs(std::vector<int> &faces, std::vector<int> &neighs){
        int neigh, origin, target;
        for(std::size_t i = 0; i < n_faces; i++){
            for(std::size_t j = 0; j < 3; j++){
                halfEdge he;
                neigh = neighs.at(3*i + ((j+2)%3));
                origin = faces[3*i+j];
                target = faces[3*i+((j+1)%3)];

                he.origin = origin;
               // he.target = target;
                he.next = 3*i + ((j+1)%3);
                he.prev = 3*i + ((j+2)%3);
                //he.face = i;
                he.is_border = (neigh == -1);
                if(neigh != -1){
                    for (std::size_t j = 0; j < 3; j++){
                        if(faces.at(3*neigh + j) == target && faces.at(3*neigh + (j + 1)%3) == origin){
                            he.twin = 3*neigh + j;
                            break;
                        }
                    }
                }else
                    he.twin = -1;
                HalfEdges.push_back(he);
                Vertices[he.origin].incident_halfedge = i*3 + j;
            }
        }
    }

    
    //Generate exterior halfedges
    //This takes  n + k time where n is the number of vertices and k is the number of border edges
    void construct_exterior_halfEdges(){

        //search interior edges labed as border, generates exterior edges
        //with the origin and target inverted and add at the of HalfEdges vector
        //std::cout<<"Size vector: "<<HalfEdges.size()<<std::endl;
        this->n_halfedges = HalfEdges.size();
        for(std::size_t i = 0; i < this->n_halfedges; i++){
            if(HalfEdges.at(i).is_border){
                halfEdge he_aux;
                //he_aux.face = -1;
                he_aux.twin = i;
                he_aux.origin = origin(next(i));
                //he_aux.target = HalfEdges.at(i).origin;
                he_aux.is_border = true;
                HalfEdges.at(i).is_border = false;
                
                HalfEdges.push_back(he_aux);
                HalfEdges.at(i).twin = HalfEdges.size() - 1 ;
            }    
        }
        //traverse the exterior edges and search their next prev halfedge
        int nxtCCW, prvCCW;
        for(std::size_t i = n_halfedges; i < HalfEdges.size(); i++){
            if(HalfEdges.at(i).is_border){
                nxtCCW = CCW_edge_to_vertex(HalfEdges.at(i).twin);
                while (HalfEdges.at(nxtCCW).is_border != true)
                    nxtCCW = this->CCW_edge_to_vertex(nxtCCW);
                HalfEdges.at(i).next = nxtCCW;

                prvCCW = this->next(twin(i));
                while (HalfEdges.at(HalfEdges.at(prvCCW).twin).is_border != true)
                    prvCCW = this->CW_edge_to_vertex(prvCCW);
                HalfEdges.at(i).prev = HalfEdges.at(prvCCW).twin;
            }
        }
        this->n_halfedges = HalfEdges.size();
    }


    //Read the mesh from a file in OFF format
    std::vector<int> read_OFFfile(std::string name){
        //Read the OFF file
        std::vector<int> faces;
		std::string line;
		std::ifstream offfile(name);
		double a1, a2, a3;
		std::string tmp;
		if (offfile.is_open())
		{
            //Check first line is a OFF file
			while (std::getline(offfile, line)){ //add check boundary vertices flag
                std::istringstream(line) >> tmp;
                //std::cout<<"tmp: "<<tmp<<std::endl;
				if (tmp[0] != '#' && !isWhitespace(line))
				{
					if(tmp[0] == 'O' && tmp[1] == 'F' && tmp[2] == 'F') //Check if the format is OFF
                        break;
                    else{
                        std::cout<<"The file is not an OFF file"<<std::endl;
                        exit(0);
                    }
				}
			}

            //Read the number of vertices and faces
            while (std::getline(offfile, line)){ //add check boundary vertices flag
                std::istringstream(line) >> tmp;
               // std::cout<<"tmp: "<<tmp<<std::endl;
				if (tmp[0] != '#' && !isWhitespace(line))
                { 
                            std::istringstream(line) >> this->n_vertices >> this->n_faces;
                            this->Vertices.reserve(this->n_vertices);
                            faces.reserve(3*this->n_faces);
                            break;
                            
                }
			}

            //Read vertices
            int index = 0;
			while (index < n_vertices && std::getline(offfile, line) )
			{
				std::istringstream(line) >> tmp;
                // std::cout<<"tmp: "<<tmp<<std::endl;
				if (tmp[0] != '#' && !isWhitespace(line))
				{
					std::istringstream(line) >> a1 >> a2 >> a3;
					vertex ve;
                    ve.x =  a1;
                    ve.y =  a2;
                    this->Vertices.push_back(ve);
                    index++;
				}
			}
            //Read faces
            
            int lenght, t1, t2, t3;
            index = 0;
			while (index < n_faces && std::getline(offfile, line) )
			{
				std::istringstream(line) >> tmp;
                // std::cout<<"tmp: "<<tmp<<std::endl;
				if (tmp[0] != '#' && !isWhitespace(line))
				{
                    std::istringstream(line) >> lenght >> t1 >> t2 >> t3;
                    faces.push_back(t1);
                    faces.push_back(t2);
                    faces.push_back(t3);
          //          std::cout<<"face "<<index<<": "<<t1<<" "<<t2<<" "<<t3<<std::endl;
                    index++;
				}
			}

		}
		else 
				std::cout << "Unable to open node file"; 
		offfile.close();
        return faces;
    }


public:

    //default constructor
    Triangulation() {}

    //Constructor from file
    Triangulation(std::string node_file, std::string ele_file, std::string neigh_file) {
        std::vector<int> faces;
        std::vector<int> neighs;
        std::cout<<"Reading node file"<<std::endl;
        read_nodes_from_file(node_file);
        //fusionar estos dos métodos
        std::cout<<"Reading ele file"<<std::endl;
        faces = read_triangles_from_file(ele_file);
        std::cout<<"Reading neigh file"<<std::endl;
        neighs = read_neigh_from_file(neigh_file);

        //calculation of the time to build the data structure
        auto t_start = std::chrono::high_resolution_clock::now();
        HalfEdges.reserve(3*n_vertices - 3 - n_border_edges);
        //std::cout<<"Constructing interior halfedges"<<std::endl;
        construct_interior_halfEdges_from_faces_and_neighs(faces, neighs);
        //std::cout<<"Constructing exterior halfedges"<<std::endl;
        construct_exterior_halfEdges();

        //std::cout<<"Constructing triangles"<<std::endl;
        auto t_end = std::chrono::high_resolution_clock::now();
        t_triangulation_generation = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    }

    Triangulation(std::string OFF_file){
        std::cout<<"Reading OFF file "<<OFF_file<<std::endl;
        std::vector<int> faces = read_OFFfile(OFF_file);

        auto t_start = std::chrono::high_resolution_clock::now();
        HalfEdges.reserve(3*n_vertices);
        construct_interior_halfEdges_from_faces(faces);
        construct_exterior_halfEdges();

        auto t_end = std::chrono::high_resolution_clock::now();
        t_triangulation_generation = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    }

    // Copy constructor
    Triangulation(const Triangulation &t) {
        this->n_vertices = t.n_vertices;
        this->n_faces = t.n_faces;
        this->n_halfedges = t.n_halfedges;
        this->Vertices = t.Vertices;
        this->HalfEdges = t.HalfEdges;
        this-> t_triangulation_generation = t.t_triangulation_generation;
    }

    Triangulation(int size){
        int n = size;
        int sqrt_n = (int)sqrt(size);

        n_vertices = size;
        std::vector<int> faces;

        this->Vertices.reserve(this->n_vertices);
        faces.reserve(2*(n-sqrt_n));

        std::cout<<"Generating points  "<<std::endl;
        for (int i = 0; i < sqrt_n; i++)
            for (int j = 0; j < sqrt_n; j++)
            {
                vertex ve;
                ve.x =  (float)i;
                ve.y =  (float)j;
                this->Vertices.push_back(ve);
            }
        
        std::cout<<"Generating triangles "<<std::endl;
        for (int i = 0; i < n-sqrt_n; i++)
        {
            if (i % sqrt_n != sqrt_n-1){
                faces.push_back(i);
                faces.push_back(i+1);
                faces.push_back(i+sqrt_n+1);

                faces.push_back(i);
                faces.push_back(i+sqrt_n+1);
                faces.push_back(i+sqrt_n);
            }
        }

        n_faces = faces.size()/3;

//        std::cout<<"Constructing halfedges "<<std::endl;
        auto t_start = std::chrono::high_resolution_clock::now();      
        HalfEdges.reserve(3*n_vertices);
        construct_interior_halfEdges_from_faces(faces);
        construct_exterior_halfEdges();

        auto t_end = std::chrono::high_resolution_clock::now();
        t_triangulation_generation = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        std::cout<<"Triangulation generation time: "<<t_triangulation_generation<<std::endl;
    }


    // destructor
    ~Triangulation() {
        Vertices.clear();
        HalfEdges.clear();
    }

    double get_triangulation_generation_time() {
        return t_triangulation_generation;
    }

    long long get_size_vertex_struct() {
        return sizeof(decltype(Vertices.back())) * Vertices.capacity();
    }

    long long get_size_vertex_half_edge() {
        return sizeof(decltype(HalfEdges.back())) * HalfEdges.capacity();
    }

    // Calculates the distante of edge e
    double distance(int e){
        double x1 = Vertices.at(origin(e)).x;
        double y1 = Vertices.at(origin(e)).y;
        double x2 = Vertices.at(target(e)).x;
        double y2 = Vertices.at(target(e)).y;
        return pow(x1-x2,2) + pow(y1-y2,2); //no sqrt for performance
    }


    //int face_index(int i){
    //    return HalfEdges.at(i).face;
    //}

    //Return triangle of the face incident to edge e
    //Input: e is the edge
    //output: array with the vertices of the triangle
    _triangle incident_face(int e)
    {   
        _triangle face;  
        int nxt = e;
        int init_vertex = origin(nxt);
        int curr_vertex = -1;
        int i = 0;
        while ( curr_vertex != init_vertex )
        {
            nxt = next(nxt);            
            curr_vertex = origin(nxt);
            face.at(i) = curr_vertex;
            i++;
        }
        return face;
    }
    
    //function to check if a triangle is counterclockwise
    //Input: array with the vertices of the triangle
    //Output: true if the triangle is counterclockwise, false otherwise
    bool is_counterclockwise(_triangle tr)
    {
        int v0 = tr.at(0);
        int v1 = tr.at(1);
        int v2 = tr.at(2);
        double area = 0.0;
            //int val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
        area = (Vertices.at(v2).x - Vertices.at(v1).x) * (Vertices.at(v1).y - Vertices.at(v0).y) - (Vertices.at(v2).y - Vertices.at(v1).y) * (Vertices.at(v1).x - Vertices.at(v0).x);
        if(area < 0)
            return true;
        return false;
    }

//Given a edge with vertex origin v, return the next coutnerclockwise edge of v with v as origin
//Input: e is the edge
//Output: the next counterclockwise edge of v
int CCW_edge_to_vertex(int e)
{
    int twn, nxt;
    nxt = HalfEdges.at(e).prev;
    twn = HalfEdges.at(nxt).twin;
    return twn;
}    

//Given a edge with vertex origin v, return the prev clockwise edge of v with v as origin
//Input: e is the edge
//Output: the prev clockwise edge of v
int CW_edge_to_vertex(int e)
{
    int twn, nxt;
    twn = HalfEdges.at(e).twin;
    nxt = HalfEdges.at(twn).next;
    return nxt;
}    

    //return number of faces
    int faces(){
        return n_faces;
    }

    //Return number of halfedges
    int halfEdges(){
        return n_halfedges;
    }

    //Return number of vertices
    int vertices(){
        return n_vertices;
    }

    //list of triangles where true if the halfege generate a unique face, false if the face is generated by another halfedge
    //Replace by a triangle iterator
    std::vector<int> get_Triangles(){
        triangle_list.reserve(n_faces);
        for(std::size_t i = 0; i < n_faces; i++)
            triangle_list.push_back(3*i);
        return triangle_list;
    }

    double get_PointX(int i){
        return Vertices.at(i).x;
    }

    double get_PointY(int i){
        return Vertices.at(i).y;
    }

    //Calculates the next edge of the face incident to edge e
    //Input: e is the edge
    //Output: the next edge of the face incident to e
    int next(int e){
        return HalfEdges.at(e).next;
    }

    //Calculates the tail vertex of the edge e
    //Input: e is the edge
    //Output: the tail vertex v of the edge e
    int origin(int e){
        return HalfEdges.at(e).origin;
    }


    //Calculates the head vertex of the edge e
    //Input: e is the edge
    //Output: the head vertex v of the edge e
    int target(int e){
        //return HalfEdges.at(e).target;
        return this->origin(HalfEdges.at(e).twin);
    }

    //Return the twin edge of the edge e
    //Input: e is the edge
    //Output: the twin edge of e
    int twin(int e){
        return HalfEdges.at(e).twin;
    }

    //Return the twin edge of the edge e
    //Input: e is the edge
    //Output: the twin edge of e
    int prev(int e)
    {
        return HalfEdges.at(e).prev;
    }



    //return a edge associate to the node v
    //Input: v is the node
    //Output: the edge associate to the node v
    int edge_of_vertex(int v)
    {
        return Vertices.at(v).incident_halfedge;
    }

    //Input: edge e
    //Output: true if is the face of e is border face
    //        false otherwise
    bool is_border_face(int e)
    {
        return HalfEdges.at(e).is_border;
    }

    // Input: edge e of compressTriangulation
    // Output: true if the edge is an interior face a
    //         false otherwise
    bool is_interior_face(int e)
    {
       return !this->is_border_face(e);
    }

    //Input:vertex v
    //Output: the edge incident to v, wiht v as origin
    bool is_border_vertex(int v)
    {
        return Vertices.at(v).is_border;
    }

    //Halfedge update operations
    void set_next(int e, int nxt)
    {
        HalfEdges.at(e).next = nxt;
    }

    void set_prev(int e, int prv)
    {
        HalfEdges.at(e).prev = prv;
    }

    //void set_face(int e, int f)
    //{
    //    HalfEdges.at(e).face = f;
    //}

int degree(int v)
{
    int e_curr = edge_of_vertex(v);
    int e_next = CCW_edge_to_vertex(e_curr);
    int adv = 1;
    while (e_next != e_curr)
    {
        e_next = CCW_edge_to_vertex(e_next);
        adv++;
    }
    return adv;
}

int incident_halfedge(int f)
{
    return 3*f;
}
};

#endif