class Node():

    def __init__(self, x, y, t):
        self.x = x
        self.y = y
        self.t = t
        self.g = 0
        self.h = 0

    def getG(self):
        return self.g

    def getH(self):
        return self.h

    def getF(self):
        return self.g + self.h

    def getT(self):
        return self.t

    def __eq__(self, other):
        if not other and not self:
            return True
        return other.x == self.x and other.y == self.y

    def __ne__(self, other):
        if  other or self:
            return True
        return self.x != other.x and self.y == other.y

    def __lt__(self, other):
        return self.getF() < other.getF()

    def __gt__(self, other):
        return self.getF() > other.getF()

    def __str__(self):
        return "(x:" + str(self.x) + ",y:" + str(self.y) + ",t:" + str(self.getT()) + ")"

    
