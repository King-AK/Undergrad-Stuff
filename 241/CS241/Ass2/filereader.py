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
    RecordLine = line.split(":")
	# change below
    record.idNum = int(RecordLine[0])
    record.firstName = RecordLine[1]
    record.lastName = RecordLine[2]
    record.classCode = int(RecordLine[3])
    record.gpa = float(RecordLine[4])
	# change above
    return record


class StudentRecord :
  def __init__( self ):
    self.idNum = 0
    self.firstName = ""
    self.lastName = ""
    self.classCode = 0
    self.gpa = 0.0

