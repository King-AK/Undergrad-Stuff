#VJ Davey 
#Space delimited fizzbuzz
import sys
import csv

with open(sys.argv[1], 'r' ) as csvfile:
	test_cases = csv.reader(csvfile, delimiter = ' ') #space delimited
	for row in test_cases:
		x = int(row[0])
		y = int(row[1])
		z = int(row[2])
		string = ''

		for i in range(1,z+1):
			if (i % (x*y)) == 0:
				string += "FB "
			elif (i % x) == 0:
				string += "F "
			elif (i % y) == 0:
				string += "B "
			else:
				string += ("%d "% (i))
		sys.stdout.write(string.rstrip() + '\n')
		