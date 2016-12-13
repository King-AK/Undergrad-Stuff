#wordhistogram.py
#written by VJ Davey
from webreader import WebReader
from nltk import *
import urllib2
from collections import Counter


class WordHistogram():
   
  
   
   def __init__(self,URL):
      self.URL = (URL)
      self.WordDictionary={}
      WordList=(URL)
      
      
      self.Listerator= []
      self.tokenlist=[]
      
      
      for word in WordList.tokens_:
         if word not in self.tokenlist:
            self.tokenlist.append(word)
     
      for word in WordList.tokens_:
         self.Listerator.append(word)
      self.x = Counter(self.Listerator)
      for word in WordList.tokens_:
         self.WordDictionary[word] = self.x[word]
         
      self.index = -1
      self.end = len(self.tokenlist)-1
         
   def DictionaryReturn(self):
      for word in self.WordDictionary:
         print word, ":", self.WordDictionary[word]
  
   def __iter__(self):
      return self
   
   def next(self):
      if self.index == self.end:
            raise StopIteration  
      self.index += 1
      key = self.tokenlist[self.index]
      value = self.WordDictionary[key]
      string = (str(key)+ ":"+ str(value))

      return string
  

      
      
  

#URL = (input("enter URL..."))
#Function = WordHistogram(URL)


   
