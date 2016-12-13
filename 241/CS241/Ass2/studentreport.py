# studentreport.py
#
# Created by:  R. Necaise
# Modified by: VJ Davey
#
#

from filereader import *

class StudentReport(object):
  
  def __init__(self):
    self.theList = list()
       
  def loadRecords( self, filename ):  
    reader = StudentFileReader( filename )
    reader.open()
    
    record = reader.fetchRecord()
    while record is not None :
      self.theList.append( record )
      record = reader.fetchRecord()
  
    reader.close()
    return self.theList
  

  def sortByName( self ):
    self.theList.sort( key = lambda rec: rec.lastName )
  
  def sortByid( self ):
    self.theList.sort( key = lambda rec: rec.idNum )
  
  
  def __str__( self ):
   
    # The class names associated with the class codes.
    classNames = [ "", "Freshman", "Sophomore", "Junior", "Senior" ]
     
     # Print the header.
    print( "LIST OF STUDENTS".center(50) )
    print( "" )
    print( "%-5s  %-25s  %-10s  %-4s" % ('ID', 'NAME', 'CLASS', 'GPA'))
    print( "%5s  %25s  %10s  %4s" % ('-' * 5, '-' * 25, '-' * 10, '-' * 4))
  
     # Print the body.
    for record in self.theList :
      print( "%5d  %-25s  %-10s  %4.2f" % \
                    (record.idNum, \
                     record.lastName + ', ' + record.firstName,
                     classNames[record.classCode], record.gpa) )
     # Add a footer.
    print( "-" * 50 )  
    totalGPA = 0
    for record in self.theList : 
      totalGPA += (record.gpa)
      averageGPA = totalGPA / len(self.theList)
    print( "Average GPA:", '%.2f' % averageGPA )  
    return ''
   
  # Code to Run Program:
if __name__ == "__main__":
  
  s = StudentReport()
  s.loadRecords('students2.txt')
  s.sortByName()
  print (str(s))