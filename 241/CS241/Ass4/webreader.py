#webreader.py
#written by VJ Davey

from nltk import *
import urllib2

class WebReader():
    
    
    def __init__(self, URL):
        
        self.URL = (URL)#Have the constructor take a URL for a website to scan.
        
        self.fetchedHTML = urllib2.urlopen(self.URL)
        
        self.HTML_Txt = self.fetchedHTML.read()
        
        self.cleanedTxt=clean_html(self.HTML_Txt)
        self.tokens = word_tokenize(self.cleanedTxt)
        self.tokens_ = filter(lambda c: c.isalpha(), self.tokens)
        self.text = Text(self.tokens)
        
        self.index = -1
        self.end = len(self.tokens_)-1
        
    
    def HTML_Script(self):
       
        print self.HTML_Txt
         
    def nltk_Cleaner(self):
        return self.cleanedTxt
    
    def tokenized_Txt(self):
        
        return self.tokens[0:]
       
        
    def __iter__(self):

        return self
       
    
    def next(self):
        
        if self.index == self.end:
            raise StopIteration
        self.index += 1
        word = self.tokens_[self.index]
        return word
        
    
    
#URL = input("enter the url...   ")
 
#URL must be entered with quotes ("") around it to work properly

#Reader = WebReader(URL)

'''for i in Reader.tokens_ :
    print i'''

   
