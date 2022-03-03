import sys
from math import atan2, degrees


def read_OFF(filename):
    vertices = []
    regions = []
    boundary = []
    with open(filename) as fp:
        # Read number of vertices
        for line in fp:
            if line.startswith('#'):
                continue
            l = line.split()
            if l[0] == 'OFF':
                break
            else:
                sys.exit("Error: File is not an OFF file")
        for line in fp:
            if line.startswith('#'):
                continue
            l = line.split()
            if l[0].isdigit():
                n_vertices = int(l[0])
                n_regions = int(l[1])
                break
        # read vertices
        for i in range(0, n_vertices):
            line = fp.readline()
            l = line.split()
            vertices.append((float(l[0]), float(l[1])))
        # read regions
        for i in range(0, n_regions):
            line = fp.readline()
            l = line.split()
            l.pop(0)
            regions.append([int(x) for x in l])

    return vertices, regions

def calc_edges_per_polygon(regions):
    edges = 0
    for i in regions:
        edges += len(i)
    return edges / len(regions)

#https://stackoverflow.com/questions/58953047/issue-with-finding-angle-between-3-points-in-python
def angle_between(p1, p2, p3):
    x1, y1 = p1
    x2, y2 = p2
    x3, y3 = p3
    deg1 = (360 + degrees(atan2(x1 - x2, y1 - y2))) % 360
    deg2 = (360 + degrees(atan2(x3 - x2, y3 - y2))) % 360
    return deg2 - deg1 if deg1 <= deg2 else 360 - (deg1 - deg2)


def min_max_angle_polygon(poly, vertices):
    min_angle = 360
    max_angle = 0
    for i in range(0, len(poly)):   
        p1 = vertices[poly[i]]
        p2 = vertices[poly[(i + 1) % len(poly)]]
        p3 = vertices[poly[(i + 2) % len(poly)]]
        angle = angle_between(p1, p2, p3)
        if angle < min_angle:
            min_angle = angle
        if angle > max_angle:
            max_angle = angle
    return min_angle, max_angle

def min_max_angle_mesh(regions, vertices):
    min_angle = 360
    max_angle = 0
    for i in regions:
        min_angle_poly, max_angle_poly = min_max_angle_polygon(i, vertices)
        if min_angle_poly < min_angle:
            min_angle = min_angle_poly
        if max_angle_poly > max_angle:
            max_angle = max_angle_poly
    return min_angle, max_angle

if __name__ == "__main__":
    filename = sys.argv[1]
    vertices, regions = read_OFF(filename)
    print(vertices)
    print(regions)
    
    print("Edges per polygon:", calc_edges_per_polygon(regions))
    
    assert(angle_between((0,0), (0,1), (0,2)) == 180.0)
    assert(angle_between((0,0), (0,0), (0,0)) == 0.0)
    assert(angle_between((-1,0), (0,0), (0,1)) == 90.0)

    #square max min angle
    assert(min_max_angle_polygon([0, 1, 2, 3], [(0,0), (1,0), (1,1), (0,1)]) == (90.0, 90.0))

    #two triangle mesh in a square
    assert(min_max_angle_mesh([[0, 1, 3], [1, 2, 3]], [(0,0), (1,0), (1,1), (0,1)]) == (45.0, 90.0))

    min_angle, max_angle = min_max_angle_mesh(regions, vertices)
    print("Min angle:", min_angle)
    print("Max angle:", max_angle)    
