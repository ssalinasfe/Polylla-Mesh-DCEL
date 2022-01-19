# Polylla: Polygonal meshing algorithm based on terminal-edge regions
<p align="center">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/polyllalogo2.png" width="80%">
</p>
New algorithm to generate polygonal meshes of arbitrary shape, using any kind of triangulation as input, adaptable to any kind of complex geometry, no addition of extra points and uses the classic Doubly connected edge list (Half edge data struct) easy to implement wih another programming language.

<p align="center">
<img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachu_A500000_T531_V352.png" width="50%">
</p>

The algorithm needs a initial triangulation as input, any triangulations will work, in the following Figure the example of a Planar Straigh Line Graph (PSLG) with holes (left image), triangulizated (middle image) to generate a Polylla mesh (right image).

<p align="center">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/faceoriginalPSLG.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/facewithtrianglesblack.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/final.png" width="30%">
</p>

<p align="center">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachu PLSG.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachutriangulization.png" width="30%">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/pikachuPolylla.png" width="30%">
</p>


## IO formats

The algorithm supports two file formats as input, the output is an [.off file](https://en.wikipedia.org/wiki/OFF_(file_format)) and an .ale file use for the VEM.

### Input as  .node, .ele, .neigh files

Triangulation is represented as a [.node file](https://www.cs.cmu.edu/~quake/triangle.node.html) with the nodes of the triangulations and the [boundary marker](https://www.cs.cmu.edu/~quake/triangle.markers.html), [.ele file](https://www.cs.cmu.edu/~quake/triangle.ele.html) with the triangles of the triangulations and a [.neigh file ](https://www.cs.cmu.edu/~quake/triangle.neigh.html) with the adjacencies of each triangle. 


Input commands of polylla are:

```
./Polylla <input .node> <input .ele> <input .neigh> <output filename>
```

Example to generate pikachu

```
./Polylla ./Polylla pikachu.1.node pikachu.1.ele pikachu.1.neigh out
```


### Input as a .off file


```
./Polylla <input .off> <output filename>
```


## Shape of polygons

Note shape of the polygon depend on the initital triangulation, in the folowing Figure there is a example of a disk generate with a Delaunay Triangulation with random points (left image) vs a refined Delaunay triangulation with semi uniform points (right image).

<p align="center">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/2x2RPDisk_3000_poly_1000.png" width="40%" hspace="10px">
 <img src="https://github.com/ssalinasfe/Polylla-Mesh-DCEL/blob/main/images/disk2x2_1574_poly1012.png" width="40%">
</p>


## Scripts

Scripts made to facilizate the process of test the algorithm:

 - (in build folder) To generate random points, an initital triangulation and a poylla mesh

    ```
    ./generatemesh.sh <number of vertices of triangulation>
    ```


 - (in build folder) To generate mesh from files .node, .ele, .neigh with the same name 

    ```
    ./generatefromfile.sh <filename> <output name>
    ```


Triangulazitation are generated with [triangle](https://www.cs.cmu.edu/~quake/triangle.html) with the [command -zn](https://www.cs.cmu.edu/~quake/triangle.switch.html).


## TODO


### TODO scripts

- [ ] Line 45 of plotting depends on a transpose, store edges directly as the transpose of edge vectors and remove it.
- [ ] Define an input and output folder scripts
- [ ] Define -n in plot_triangulation.py to avoid label edges and vertices
- [ ] Change name plot_triangulation.py to plot_mesh.py

### TODO Poylla
- [ ] Travel phase does not work with over big meshes (10^7)
- [ ] Add high float point precision edge lenght comparision
- [ ] POSIBLE BUG: el algoritmo no viaja por todos los halfedges dentro de un poligono en la travel phase, por lo que pueden haber semillas que no se borren y tener poligonos repetidos de output
- [ ] Add arbitrary precision arithmetic in the label phase
- [ ] Add frontier-edge addition to constrained segmend and refinement (agregar método que dividida un polygono dado una arista especifica)
- [X] hacer la función distance parte de cada halfedge y cambiar el ciclo por 3 comparaciones.
- [X] Add way to store polygons.
- [ ] iterador de polygono
- [X] Vector con los poligonos de malla
- [ ] Método para imprimir SVG
- [ ] Copy constructor
- [ ] half-edge constructor
- [X] Change by triangle bitvector by triangle list
- [X] Remove distance edge


### TODO Halfedges 


- [ ] edge_iterator;
- [ ] face_iterator;
- [ ] vertex_iterator;
- [ ] copy constructor;
- [X] constructor indepent of triangle (any off file now works)
- [X] default constructor
- [ ] definir mejor cuáles variables son unsigned int y cuáles no
- [X] Change by triangle bitvector by triangle list
- [ ] Calculate distante edge
- [ ] Read node files with commentaries

### TODO C++

- [X] change to std::size_t to int
- [X] change operator [] by .at()
- [X] add #ifndef ALL_H_FILES #define ALL_H_FILES #endif to being and end header
- [ ] add google tests
- [ ] Add google benchmark


### TODO github

- [ ] Add how generate mesh from OFF file
- [X] Add images that show how the initial trangulization changes the output
- [X] Add the triangulation of the disks
- [X] Hacer el readme más explicativo
- [ ] Add example meshes
- [X] Add .gitignore
- [ ] Poner en inglés uwu