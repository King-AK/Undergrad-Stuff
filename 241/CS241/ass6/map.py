from node import Node
import math

class Map:

    def __init__(self, filename):
        f = open(filename, 'r')
        dimensions = f.readline()
        dimensions = dimensions.split()
        self.width = int(dimensions[0])
        self.height = int(dimensions[1])
        self.nodes = []
        for y in range(0, self.height):
            l = f.readline()
            l = l.split()
            for x in range(0, self.width):
                n = Node( x, y, int(l[x]))
                self.nodes.append(n)

    def getNode(self, coord):
        index = coord[0] + self.width * coord[1]
        return self.nodes[index]

    def printMap(self):
        ret = ""
        for j in range(0, self.height):
            for i in range(0, self.width):
                n = self.getNode((i, j))
                ret += str(round(n.getF(),2)) + ' '
            ret += '\n'
        return ret

    def getNearby(self, coord):
        x = coord[0]
        y = coord[1]
        nearbylist = []
        for j in range(-1,2):
            for i in range(-1, 2):
                if i == 0 and j == 0:
                    continue
                y2 = y + j
                x2 = x + i
                if y2>=0 and y2<self.height and x2>=0 and x2<self.width:
                    n = self.getNode((x2,y2))
                    if n.getT() == 0:
                        nearbylist.append(n)
        return nearbylist
                
                



