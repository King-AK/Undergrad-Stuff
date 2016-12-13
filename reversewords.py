#VJ Davey 
#reverse words
import sys
import csv

with open(sys.argv[1], 'r' ) as csvfile:
	test_cases = csv.reader(csvfile, delimiter = ' ') #space delimited
	for row in test_cases:
		string = ''
		m = 1
		for i in row:
			string += row[-1*m] + " "
			m+=1
		sys.stdout.write(string.rstrip() + '\n')
		