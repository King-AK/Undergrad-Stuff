#modifying linked list to implement a two-dimensional linked-list (or matrix)

#VJ Davey
from node import Node


class LinkedMatrix(object):
    
    def __init__(self, x, y, defaultValue):
        
        ''' creates a new matrix with x columns and y rows 
        with default value default value'''
        
        self._xRows = x
        # x amt of rows
        self._yCols = y
        # y amt of columns
        self.defaultValue=defaultValue
        self.matrix = []
        #empty matrix list. will contain list of lists
        
        for number in range(self._xRows) :
            self.mlist = [] #empty list meant to contain future nodes
            for number in range (self._yCols) :
                base_node=Node(defaultValue)
                self.mlist = self.mlist + [base_node]
            self.matrix = self.matrix +[self.mlist]
               
        #connected nodes
        self.head=None
        self.tail=None
        for number in range(self._xRows):
            for number in range (self._yCols):
                base_node=Node(defaultValue)
                if self.head == None:
                    self.head=base_node
                    self.head.east=self.tail
                if self.tail != None:
                    self.tail.east=base_node
                self.tail=base_node
                    
                
    
        
    def __str__ (self):
        
        '''returns the string representation of the matrix'''

        stringmatrix=[]
        for item in self.matrix:
            stringmlist=[]
            for node in item:
                a=str(node.getValue())
                stringmlist=stringmlist+[a]
            stringmatrix = stringmatrix + [stringmlist]
        for item in stringmatrix:
            print (item)
        
        return ''
    
    def __getitem__(self, index):
        '''return the element at the index expressed as a tuple'''
        x,y = index
        stringmatrix=[]
        for item in self.matrix:
            stringmlist=[]
            for node in item:
                a= str(node.getValue())
                stringmlist=stringmlist+[a]
            stringmatrix = stringmatrix + [stringmlist]
        get = stringmatrix[x][y]
        return get

    def __setitem__(self, index, a):

        '''set value at index to a'''
        x,y = index
        newnode = Node(a)
        self.matrix[x][y] = newnode
        
        

    def __iter__(self):

        '''returns a list of all the items in the matrix first across down
        then across repeat'''
        #this will become some kind of yield function
        #set each node so that they are connected NSEW. Print From West to East
        #then go South a row, and print from West to East again until all nodes 
        #are hit
        node = self.head
        while node != None:
            yield (node.getValue())
            node = node.east         
        
        
        

    def insertRowAfter(self, rowIndex):

        newRow = [Node(self.defaultValue)]*self._yCols
        
        self.matrix[rowIndex:rowIndex]=[newRow]

    def insertColumnAfter(self, colIndex):

        for item in self.matrix:
            newNode=Node(self.defaultValue)
            item[colIndex:colIndex]=[newNode]
        

    def removeRowAfter(self, rowIndex):

        self.matrix.pop(rowIndex)

    def removeColumnAfter(self, colIndex):
        for item in self.matrix:
            item.pop(colIndex)
    def PrintList(self):
        node = self.head
        while node != None:
            print (node.getValue())
            node = node.east        