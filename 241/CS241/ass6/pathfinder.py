from priorityqueue import PriorityQueue
from node import Node
from map import Map
import math

class PathFinder():

    def __init__(self):
    #Im going to want to run a BFS on the items in the map
    #the priorityqeue is for efficiency, but I need a working search first and foremost...
    #work with map and node class, read both, see how they work in relation to a map txt file
	
	
        pass 

#--------------------------------------------------------------------    
    def findPath(self, theMap, start, end):
	
	# start and end nodes are tuples expressing their x and y coordinates.
	originNode = theMap.getNode(start)
	finishNode = theMap.getNode(end)
	SurroundingNodes = theMap.getNearby(start)#tells all the Nodes surrounding the start
	
	
	queue = []
	CheckedNodes=[]
	ParentDict = {}#node dictionary 
	queue.append(originNode)

	while queue:
	    
	    node = queue.pop(0)
	    
	    CheckedNodes.append(str(node))
	    xval = node.x
	    yval = node.y
	    NearNodes=theMap.getNearby((xval,yval))
	    #gets the nearby nodes of the node item in the queue
	#-------------------------
	#if finishNode is found...
	    if str(node)  == (str(finishNode)):
		
		path = [str(finishNode)]
		
		CurrentNode = finishNode
		while str(originNode) not in path:
		    
		    path.append (str(ParentDict[str(CurrentNode)]))
		    CurrentNode = ParentDict[str(CurrentNode)]
		    
		path.reverse()
		print 'To get to the end in the shortest amount of moves, follow this path of', len(path)-1, 'moves: ', path
		
	#--------------------------
	#if search for finishNode must continue...
	    for ChildNode in NearNodes:
		if str(ChildNode) not in ParentDict:#if the node is not already in the dicitonary, add it to the dictionary along with its parent. This MUST be done so that we dont have parents overwriting existing parents on the nodes
		    ParentDict[str(ChildNode)] = node # <<< adding the parent 
		    if str(ChildNode) not in CheckedNodes: 
			queue.append(ChildNode)
	#------------------------
	#BFS made... also contains node dicitionary built containing all possible nodes on map.txt, this is a dicitonary of all explored nodes
	#58 total travel spots: shown as 0s , 6 total dead zones : shown as 1s
	
#---------------------------------------------------------------------------
		
	
	#q is probably the dictionary, while its length is under some number, the priority queue is in effect
	
        '''while len(q):
           # remove an element from the priority queue
	   # add its neighbors
	   # compute and store the g and h values for each neighbor
	    
	# rebuild the path with your already explored node dictionary
	# return the path as a list of nodes
	    pass'''
#-----------------------------------------------------------------------------
if __name__ == "__main__":

    m = Map("map.txt")
    pf = PathFinder()
    print
    pf.findPath(m, (0,0), (7,7))





