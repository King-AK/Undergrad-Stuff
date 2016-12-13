from node import Node
class PriorityQueue ():

    def __init__(self):
        self.A = []
	

    def insert(self, value):
	self.A.append(value)
    	

    def remove(self):
	#the Node class you gave us doesnt give us any H G or F values. Im not entirely sure about how I should go about implementing this but I think I would have to edit the node class to be able to get the F score ...
	
       # write the code to remove the node with the lowest f score from the priority queue
	# you may want to call bubble down afterwards 
	'''
	if len(self.A) == 0:
	    return None	
	
	elif len(self.A)>0:
	    fdict = {}
	    lowestF = 99999999
	    for node in self.A:
		#fscore = node.getF()
		fdict[node] = fscore
		if fscore < lowestF:
		    lowestF = fscore
	    '''
	   
	#iterates thru all nodes, gets all their fscores, lowest fscore will be given value lowestF
	
		
		
	
	
		
	    
	
    
    def bubbledown(self, pos):

       # write the code to bubble the values down the heap
	pass
    def __len__(self):
        return len(self.A)
                

    def __str__(self):
        ret = ""
        for x in self.A:
            ret += str(x) + ","
        return ret


#tests
f=PriorityQueue()
f.insert(Node(3,4,5))
f.remove()
