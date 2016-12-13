from point import Point


class LineSegment:
    
   def __init__(self, pointA, pointB):
      self.pointA = pointA
      self.pointB = pointB
   def endPointA(self):
      return self.pointA

   def endPointB(self):
      return self.pointB
   
   def length(self):
      return self.pointA.distance(self.pointB)
   
   def slope(self):
      x1 = self.pointA.getX()
      x2 = self.pointB.getX()
      y1 = self.pointA.getY()
      y2 = self.pointB.getY()
      
      rise = y2-y1
      run = x2-x1
      if run != 0:
         slope = rise/run
         return slope
      else:
         return False
   def isHorizontal(self):
      if self.pointA.getY() == self.pointB.getY():
         return True
      else:
         return False
   def isVertical(self):
      if self.pointA.getX() == self.pointB.getX():
         return True
      else:
         return False
   def isParallel(self, otherLine):
      x1 = self.pointA.getX()
      y1 = self.pointA.getY()
      x2 = self.pointB.getX()
      y2 = self.pointB.getY()
      
      x3 = otherLine.pointA.getX()
      y3 = otherLine.pointA.getY()
      x4 = otherLine.pointB.getX()
      y4 = otherLine.pointB.getY()
      
      rise = y2-y1
      run = x2-x1
      riseOthe= y4-y3
      runOthe= x4-x3
      
      if run ==0 or runOthe == 0:
         if run == 0 and runOthe ==0:
            return True
         else:
            return False      
      else:
         slope = rise/run
         slopeOthe = riseOthe/runOthe
         if slope == slopeOthe:
            return True
         else:
            return False
              
   def midPoint(self):
      x1 = self.pointA.getX()
      x2 = self.pointB.getX()
      y1 = self.pointA.getY()
      y2 = self.pointB.getY()
      
      xCoord = (x1 + x2)/2
      yCoord = (y1 +y2)/2
      
      midPoint = Point(xCoord, yCoord)
      return midPoint
   def __str__(self):
      x1 = self.pointA.getX()
      x2 = self.pointB.getX()
      y1 = self.pointA.getY()
      y2 = self.pointB.getY()
      x1 = str(x1)
      y1 = str(y1)
      y2 = str(y2)
      x2 = str(x2)
      
      string = ("(" + x1 +", " + y2 + ")#(" + x2 +", " + y2 + ")")
      return string