class Node (object) :
	
	def __init__(self, value):
		self.north = None
		self.east = None
		self.south = None
		self.west = None
		self.value = value
		
	def getNorth(self):
		return self.north
		
	def getSouth(self):
		return self.south
		
	def getEast(self):
		return self.east
		
	def getWest(self):
		return self.west
		
	def getValue(self):
		return self.value