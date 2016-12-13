import math

class Point(object):
    
    def __init__(self, x, y):
        self.xCoord = x
        self.yCoord = y
        
    def __eq__(self, other):
        if self.xCoord == other.xCoord:
            if self.yCoord == other.yCoord:
                return True
            else: return False
        
    
    def getX(self):
        return self.xCoord
    
    def getY(self):
        return self.yCoord
    
    def distance(self, other):
        xDist = self.xCoord - other.xCoord
        yDist = self.yCoord - other.yCoord
        return math.sqrt(xDist**2 + yDist**2)
    
        