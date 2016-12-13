#VJ Davey 
#print all permutations of a string
import sys
from itertools import permutations
import csv

if __name__ == "__main__":
	with open(sys.argv[1], 'r' ) as csvfile:
		test_cases = csv.reader(csvfile, delimiter = ' ') #space delimited
		for row in test_cases:
			string = row[0]
			perms = [''.join(p) for p in permutations(string)]
			perms.sort()
			output = ''
			for s in perms:
				output += "%s," % (s)
			sys.stdout.write(output[:-1] + '\n') #-1 here used to chop trailing comma after removing trailing whitespace

		