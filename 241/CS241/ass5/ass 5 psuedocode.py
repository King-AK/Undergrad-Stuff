import json
moviename = 'fight+club'
api_key = '259f7fc6b26292afaf6ec1dabe6fdd6a'

import requests

root = 'https://api.themoviedb.org'
query = '/3/search/movie?api_key='+api_key+'&query='+moviename

link = root+query
print(link)


r = requests.get(root+query+'.json')
print(r)
r.json()

print(r)



#print data['name']
#print data['id']

#http://api.themoviedb.org/3/search/movie?api_key=###&query=fight+club