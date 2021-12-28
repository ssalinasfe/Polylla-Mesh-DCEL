import numpy
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import re
import sys
import math

def plot_with_halfedges(node_file, hedge_file, output):

    print("[Plotting]  Reading files")
    vertices = []
    with open(node_file,'r') as file:
        line = file.readline()
        n_points = re.findall("(?<=[AZaz])?(?!\d*=)[0-9.+-]+",line)[0]
        for i in range(0, int(n_points)):
            line = file.readline()
            vertex = re.findall("(?<=[AZaz])?(?!\d*=)[0-9.+-]+",line)
            v1 = float(vertex[1])
            v2 = float(vertex[2])
            vertices.append([v1, v2])

    edges_triangles = []
    with open(hedge_file,'r') as file:
        line = file.readline()
        n_hedges = int(re.findall("(?<=[AZaz])?(?!\d*=)[0-9.+-]+",line)[0])
        for i in range(0, int(n_hedges)):
            line = file.readline()
            hedge = re.findall("(?<=[AZaz])?(?!\d*=)[0-9.+-]+",line)
            e1 = int(hedge[0])
            e2 = int(hedge[1])
            edges_triangles.append([e1, e2])


    points = numpy.array(vertices)
    edges = numpy.array(edges_triangles)

    #x = list( map(lambda pair: pair[0], vertices))
    #y = list( map(lambda pair: pair[1], vertices))

    x = points[:,0].flatten()
    y = points[:,1].flatten()

    print("[Plotting]  Plotting edges")
    #Plot edges
    plt.plot(x[edges.T], y[edges.T], linestyle='-', color='y',
            markerfacecolor='red', marker='o') 

    print("[Plotting]  Plotting vertices labels")
    #Label vertices
    tlab = range(0, len(vertices))  # label vector
    for x, y, lab in zip(x, y, tlab):
        label = '{:0}'.format(lab)
        plt.annotate(label,  # this is the text (put lab here to use tlab as string)
                    (x, y),  # this is the point to label
                    textcoords="offset points",  # how to position the text
                    xytext=(0, 10),  # distance from text to points (x,y)
                    ha='center', 
                    size = 3)

    print("[Plotting]  Calculating midpoint and angle of halfedges")
    #All the following is to label the index of the halfedge left to each halfedge
    #Calculate the label position of the halfedge, this position is a point on the left side of the edge
    midpoints = []           
    offsetlabel = []
    for i in range(0, len(edges)):
        x1 = vertices[edges[i][0]][0]
        y1 = vertices[edges[i][0]][1]
        x2 = vertices[edges[i][1]][0]
        y2 = vertices[edges[i][1]][1]
        x_mid = (x1 + x2)/2
        y_mid = (y1 + y2)/2
        midpoints.append([x_mid, y_mid])
        #tan = (x2 - x1)/(y2 - y1) if (y2 - y1) != 0 else 0

        offset = 3

        #Calculates te position vector
        vector_x = x2 - x1
        vector_y = y2 - y1
        
        tan = vector_y/vector_x if vector_x != 0 else 0

        #Calculates the angle of the vector
        taninv = math.degrees(math.atan(tan))

        #I taninv is zero, the vector is vertical/horizontal, so the offset is in the y direction/x direction
        if taninv == 0:
            if(vector_x == 0 and vector_y >= 0):
                offsetlabel.append([-1*offset, 0 ])
            elif(vector_x == 0 and vector_y < 0):
                offsetlabel.append([offset, 0 ])
            elif(vector_x >= 0 and vector_y == 0):
                offsetlabel.append([0, offset])
            elif(vector_x < 0 and vector_y == 0):
                offsetlabel.append([0, -1*offset])
            else:
                sys.exit("Error", vector_x, vector_y, edges[i][0], edges[i][1])
            continue
        #depending of the quadrant the atan has to be changed
        elif vector_x < 0 and vector_y > 0:
            taninv = taninv + 180
        elif vector_x < 0 and vector_y < 0:
            taninv = taninv + 180
        elif vector_x > 0 and vector_y < 0:
            taninv = taninv + 360

        #depending on the angle, the left position of the label is calculated
        if taninv > 0 and taninv < 90:
            offsetlabel.append([-1*offset, 1*offset])
        elif taninv > 90 and taninv < 180:
            offsetlabel.append([-1*offset, -1*offset])
        elif taninv > 180 and taninv < 270:
            offsetlabel.append([1*offset, -1*offset])
        elif taninv > 270 and taninv < 360:
            offsetlabel.append([1*offset, 1*offset])
        else:
            sys.exit("Error calculain offset", taninv, edges[i][0], edges[i][1])

            
    if len(offsetlabel) != len(midpoints):
        sys.exit("Error: offsetlabel and midpoints have different lengths")


    print("[Plotting]  Labeling halfedges")
    #Label halfedges
    tlab = range(0, len(midpoints))  # label vector
    for i in range(0, len(midpoints)):
        label = '{:0}'.format(i)
        plt.annotate(label,  # this is the text (put lab here to use tlab as string)
                    xy = (midpoints[i][0], midpoints[i][1]),  # this is the point to label
                    textcoords="offset points",  # how to position the text
                    xytext=(offsetlabel[i][0], offsetlabel[i][1]),  # distance from text to points (x,y)
                    ha='center', 
                    color='b',
                    size=3)

    plt.axis("off")   # turns off axes
    plt.axis("tight")  # gets rid of white border
    plt.axis("image")  # square up the image instead of filling the "figure" space

    ##Add legends
    #red_patch = mpatches.Patch(color='red', label='The red data')
    ## Shrink current axis's height by 10% on the bottom
    #box = plt.get_position()
    #plt.set_position([box.x0, box.y0 + box.height * 0.1,
    #                box.width, box.height * 0.9])
    #
    ## Put a legend below current axis
    #plt.legend(handles=[red_patch], loc='upper center', bbox_to_anchor=(0.5, -0.05),
    #        fancybox=True, shadow=True, ncol=5)

    print("[Plotting]  Saving figure in " + output)
    plt.savefig(output, dpi=1200)
    #plt.show()




def main():
    if len(sys.argv) == 4:
        node_file = sys.argv[1]
        hedge_file = sys.argv[2]
        output = sys.argv[3]
        plot_with_halfedges(node_file, hedge_file, output)
    else:
        print("Plot a triangulation based on a node file and an hedge_file")
        print("Halfedges are labeled with the index left to edge")
        print("Plot with halfedges: python3 plot_triangulation.py [node_file] [hedge_file] [output_file]")
    

if __name__ == "__main__":
    main()