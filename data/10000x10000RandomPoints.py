# Generate random points inside a square 2x2 
# Input: nPoints: number of points inside the square and in the borders
# Output: A .node file with the random points

import random
import sys
import math

def write_points(nPoints, xPoints, yPoints):
    largo =len(xPoints)
    print("{} 2 0 0".format(largo))
    for i in range(0,largo):
        print('{0} {1} {2}'.format(i, xPoints[i], yPoints[i]))
    print('\n')
    

def generate_random_points(nPoints, xPoints, yPoints, tolerance):
    uwu = 10000.0
    s = set([(0, 0), (uwu,uwu), (0, uwu), (uwu,0)])

    while len(s) != nPoints:
        x = random.uniform(0.0, uwu )
        y = random.uniform(0.0, uwu )
        #print("premove ",x,y)
        s.add(move_point(uwu, x, y, tolerance))
        #print("postmove ",x,y)
        #print(len(s))

    for _ in range(0,len(s)):
        l = s.pop()
        if l[0] == 0 and l[1] == 0: 
            xPoints.insert(0,0)
            yPoints.insert(0,0)
        else:
            xPoints.append(l[0])
            yPoints.append(l[1])


def move_point(nPoints, xPoints, yPoints, tolerance):
    #tolerance =  0.0001  
    #tolerance =  5/(nPoints/10)
    n = nPoints
    r = random.uniform(0, 1)
    if r > 0.5:
        if xPoints >= nPoints*(1.0-tolerance): 
            xPoints = n
        if yPoints >= nPoints*(1.0-tolerance): 
            yPoints = n
        if xPoints <= nPoints*tolerance: 
            xPoints = 0
        if yPoints <= nPoints*tolerance: 
            yPoints = 0
    else:
        if xPoints <= nPoints*tolerance: 
            xPoints = 0            
        if yPoints <= nPoints*tolerance: 
            yPoints = 0
        if xPoints >= nPoints*(1.0-tolerance): 
            xPoints = n
        if yPoints >= nPoints*(1.0-tolerance): 
            yPoints = n
    #print("returning", xPoints, yPoints)
    return (xPoints, yPoints)

#Input Largo inicial, incremento

def main():
    full_cmd_arguments = sys.argv
    argument_list = full_cmd_arguments[1:]

    nPoints = int(argument_list[0])
    #tolerance = float(argument_list[1])
    tolerance = 1/10**(0.5*math.log10(nPoints) + 0.5)
    tolerance *= 2
    random.seed(138)
    xPoints = []
    yPoints = []
    
    generate_random_points(nPoints, xPoints, yPoints, tolerance)
    write_points(nPoints, xPoints, yPoints)
    

if __name__ == "__main__":
    main()
