#Code Written By: VJ Davey
#CS241 Data Structures
#William and Mary

import requests
import json


#get an API Key...done
#API Key = 259f7fc6b26292afaf6ec1dabe6fdd6a
#Use HTTP requests to get all the movies an actor played in, all of the actors for a given movie, and a way to determine the ID of an actor or movie by name.
#Kevin Bacon tmdb ID no : 4724

#-------------------------------------------------------------------------
class FilmNFO():
    
    def __init__(self, apikey):
        
        self.API = apikey
        self.root = 'https://api.themoviedb.org'
    
    def findActor(self, actor_name):
        
        #returns a list of strings for the actor chain
        queryactor='/3/search/person?api_key='+self.API+'&query='+actor_name
        response = requests.get(self.root+queryactor)
        
        #print response.url #URL constructed with query and API, can be used in browser to check results
        #print
        
        data = response.json()
        results = data['results']
        '''total_results = data['total_results']
        total_pages = data['total_pages']'''
        #print 'there are', int(total_results), 'total results'#tells total number of results with search criteria
        #print
    
        
        return results[0]['id']#returns name of actor with ID next to it
    def IDtoName(self, actor_ID):
        queryID = '/3/person/'+str(actor_ID)+'?api_key='+self.API
        response = requests.get(self.root+queryID)
        data = response.json()
        name = data['name']
        return str(name)
    
    
    def findMovie(self,movie_name):
       
        querymovie='/3/search/movie?api_key='+self.API+'&query='+movie_name
        response = requests.get(self.root+querymovie)
        #print response.url#URL constructed with query and API, can be used in browser to check results
        
        
        data = response.json()
        results = data['results']
        total_results = data['total_results']
        total_pages = data['total_pages']        
        #print 'there are', int(total_results), 'total results'#gives toal number of results for search criteria
        
        i = 0
        return results[i]['id']#returns title of movie with ID next to it
                
    def moviesActedIn(self, actorID):
        
        querycredits = '/3/person/'+str(actorID)+'/movie_credits?api_key='+self.API
        response = requests.get(self.root+querycredits)
        #print response.url #url for response object
        #print
        
        
        data=response.json()
        casted = data['cast']
    
        i = 0
        movielist=[]
        movieIDlist =[]
        while i <len(casted):
            movielist.append( casted[i]['title'])
            movieIDlist.append( casted[i]['id'])
            i+=1
        return movieIDlist
    
    
    def actorsInMovie(self, movieID):
        queryMovieActrs = '/3/movie/'+str(movieID)+'/credits?api_key='+self.API
        response = requests.get(self.root+queryMovieActrs)
        #print response.url#url for response object
        #print
        data = response.json()
        casted = data['cast']
        
        i=0
        actorlist=[]
        while i <len(casted):
            actorlist.append(casted[i]['id'])
            i+=1
        return actorlist

#----------------------------------------------------------------------------


class BaconFinder():
    def __init__(self, API):
        self.API=API
        self.filmfunction = FilmNFO(API)
        self.origin = '4724'
        
    def find(self, actor_name):
        self.actor_name=actor_name
        
        MasterDictionary={}
        MasterDictionary[4724]=[4724]

        if self.actor_name == 'Kevin Bacon':
            print 'Kevin Bacon has a Bacon Number of Zero. He is Kevin Bacon.'
            BaconID = self.filmfunction.findActor(self.actor_name)
            print MasterDictionary[BaconID]
        else:
            ActorID = self.filmfunction.findActor(self.actor_name)
            ActedWith=[]
            ActorList=[]
            CheckedActors=[]
            CheckedMovies =[]
    
            BaconMovies = self.filmfunction.moviesActedIn(self.origin)
             
            #edit the program so that instead of just taking bacons movies, it extends to all the movies of the actors in the ActedWith List. Upon Extension, the degree of relation to kevin bacon should rise by 1.
            
            
            
            #while self.actor_name not in ActedWith:
            degree =1
            
            for Movies in BaconMovies:
                ActorList=self.filmfunction.actorsInMovie(Movies)
                CheckedMovies.append(Movies)
                for Actor in ActorList:
                    if Actor not in ActedWith:
                        ActedWith.append(Actor)
                        MasterDictionary[Actor]=[4724, Actor]
                        
            while ActorID not in ActedWith:
                degree+=1
                for Actor in ActedWith:
                    if Actor not in CheckedActors:
                        CheckedActors.append(Actor)
                        MoviesIn = self.filmfunction.moviesActedIn(Actor)
                        for Movie in MoviesIn:
                            if Movie not in CheckedMovies:
                                CheckedMovies.append(Movie)
                                ActorList = self.filmfunction.actorsInMovie(Movie)
                                for FoundActor in ActorList:
                                    if FoundActor not in ActedWith:
                                        ActedWith.append(FoundActor)
                                        MasterDictionary[FoundActor] = MasterDictionary[Actor].append(FoundActor)
                                
                
            if ActorID in ActedWith:
                print self.actor_name, 'has a bacon number of ', degree
                Connections= MasterDictionary[ActorID]
                NameList=[]
                for i in Connections:
                    NameList.append( self.filmfunction.IDtoName(i))
                print NameList
            


#-----------------------------------------------------------------------
    
MyAPI=('259f7fc6b26292afaf6ec1dabe6fdd6a')
print 'loading...'
#test functions for FilmNFO class
#Function = FilmNFO(MyAPI) 
#print Function.findActor('miley cyrus')
#print Function.findMovie('Prison Break')
#print Function.moviesActedIn(4724)
#print Function.actorsInMovie(176241)
#print Function.moviesActedIn(76594)
#print Function.IDtoName(4724)

Bacon=BaconFinder(MyAPI)

Bacon.find('Kevin Bacon')



    
