from tmdb3 import set_key
import json
import requests
import os


API = '259f7fc6b26292afaf6ec1dabe6fdd6a'
set_key(API)


from tmdb3 import set_cache
set_cache('null')
set_cache(filename='/full/path/to/cache') # the 'file' engine is assumed
set_cache(filename='tmdb3.cache')         # relative paths are put in /tmp
set_cache(engine='file', filename='~/.tmdb3cache')

from tmdb3 import get_locale, set_locale
set_locale()
print(get_locale())
#locale has bet set to english and United States...yes
print
print


'''import urllib2 #import Request#, urlopen headers = {"Accept": "application/json"}

request = urllib2.Request("https://api.themoviedb.org/3/search/movie?api_key="+API+"&query=Foxx")
responsebody = urllib2.urlopen(request).read()
print responsebody
print
print'''

import tmdb3
search = 'Hanks'
a=tmdb3.searchPerson(search)
print a[0:]



