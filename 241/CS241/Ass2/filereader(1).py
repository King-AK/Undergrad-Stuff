# filereader.py
#
# Created by:  R. Necaise
# Modified by: 
#
#

class StudentFileReader :
  def __init__( self, inputSrc ):
    self._inputSrc = inputSrc
    self._inputFile = None

  def open( self ):
    self._inputFile = open(self._inputSrc, "r" )

  def close( self ):
    self._inputFile.close()
    self._inputFile = None

  def fetchRecord( self ):
    line = self._inputFile.readline()
    if line == "" :
      return None

    record = StudentRecord()
	# change below
    record.idNum = int(line)
    record.firstName = self._inputFile.readline().rstrip()
    record.lastName = self._inputFile.readline().rstrip()
    record.classCode = int(self._inputFile.readline())
    record.gpa = float(self._inputFile.readline())
	# change above
    return record


class StudentRecord :
  def __init__( self ):
    self.idNum = 0
    self.firstName = ""
    self.lastName = ""
    self.classCode = 0
    self.gpa = 0.0

