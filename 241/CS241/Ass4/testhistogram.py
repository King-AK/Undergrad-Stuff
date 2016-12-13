#histogram tester
from webreader import WebReader
from wordhistogram import WordHistogram

page = WebReader("http://www.cs.wm.edu/~rfd/")

for x in page:
    print x
    
    # returns all the filtered words in the page.
print
print
h = WordHistogram(page)
for x in h:
    print x
# returns the word and frequency pair as a string
    
    