#VJ Davey 
#print the nth fibonacci number
import sys
import csv

def fib(n):
	if n==0:
		return 0
	if n==1:
		return 1
	return fib(n-1)+fib(n-2)

if __name__ == "__main__":
	with open(sys.argv[1], 'r' ) as csvfile:
		test_cases = csv.reader(csvfile, delimiter = ' ') #space delimited
		for row in test_cases:
			sys.stdout.write(str(fib(int(row[0]))) + '\n')
		