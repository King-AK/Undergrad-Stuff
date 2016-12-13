# VJ Davey 
# Fitness Python Program
# 
# Used to log and keep track of calorie and protein intake over time
# User Data is held in a local CSV file
#
import webbrowser as web
import datetime
import os
import csv 
clear = lambda: os.system('cls')

def new_profile():
	#prompt user for information
	usr = str(raw_input("Hello! What is your name?\n"))
	#TODO introduce a check to see if a user with this name already exists, so as not to 
	#overwrite them and their vital statistics

	print("Hello %s!\n" %(usr))
	age = int(raw_input("Please enter your age in years... \n"))
	height = int(raw_input("Please enter your height in inches... \n"))
	weight = int(raw_input("Please enter your weight in lbs... \n"))
	gender = str(raw_input("Please enter your gender: (Press M or F)\n"))
	#calculate bmr based on input
	bmr = bmr_calc(age, height, weight, gender)
	#prompt for activity level and calculate tdee
	print('''
Please select one of the following that best describes your activity level:
===========================================================================
	[0] Little to No Exercise 
	[1] Light exercise (1 to 3 days per week) 
	[2] Moderate exercise (3 to 5 days per week) 
	[3] Heavy exercise (6 to 7 days per week)
	[4] Very heavy exercise (twice per day, extra heavy workouts)
	\n''')
	choice = int(raw_input())
	activity_level_multiplier =[1.2,1.375,1.55,1.725,1.9]
	tdee = bmr * activity_level_multiplier[choice]
	#export all information to a CSV file titled "usr.csv"
	#first row is user information
	with open("%s.csv" % (usr), 'wb') as csvfile:
		writeCSV = csv.writer(csvfile, delimiter = ',')
		writeCSV.writerow([age,height,weight,gender,bmr,tdee])


def bmr_calc(age, height, weight, gender):
	s=0
	if gender == ('M' or 'm'):
		s = 5
	elif gender == ('F' or 'f'):
		s = -161
	weight /= 2.2046
	height *= 2.54
	return ((10*weight)+(6.25*height)-(5.0*age)+s)


def addFood(new_kcal, new_protein, usr):
	#append the new information given to the csvfile based on todays date 
	with open("%s.csv" %(usr), 'a') as csvfile:
		writeCSV = csv.writer(csvfile, delimiter = ',')
		writeCSV.writerow([str(datetime.date.today()),new_kcal,new_protein])


def log_in(usr):
	#use given name to search local folder for "name".xml. If no such file exists, exit.
	#if the file does exist, open the file and load up a new set of options to run
	#import statistics and give statistical analysis of foods eaten
	with open("%s.csv" %(usr)) as csvfile: 
		readCSV = csv.reader(csvfile, delimiter = ',') #reads all the csv data in 
		dates = []
		calories = []
		protein = []
		kcal_consumed_today = 0
		protein_consumed_today = 0
		i = 0
		for row in readCSV:
			if i != (0):
				date = row[0]
				today = str(datetime.date.today())
				if(date == today ):
					kcal_consumed_today += int(row[1])
					protein_consumed_today += int(row[2])
			elif i == (0):
				kcal_goal = int(float(row[5]))
				protein_goal = int(row[2])
				i+=1
		clear()
		while 1:
			print('''
Hello %s! Welcome back!

You have %d calories and %d grams of protein left to eat today.

What would you like to do?
==========================
	[0] Log new foods eaten
	[1] Log exercise
	[2] View statistics
	[3] Find new workouts
	[Q] Log out''' % (usr, (kcal_goal - kcal_consumed_today) , (protein_goal - protein_consumed_today)))
			choice = str(raw_input())
			

			if choice == '0':
				clear()
				#convert while loop w/ fail safe for incorrect, non numerical input
				new_kcal = int(raw_input("Tell me the total calories in the food you just ate: "))
				new_protein = int(raw_input("Tell me the total protein in the food you just ate: "))
				addFood(new_kcal, new_protein,usr)
			elif choice == '1':
				#figure out a way to input exercises
				pass
			elif choice == '2':
				#give the option to look at various statistics
				#avg kcal burned, 
				pass
			elif choice == '3':
				#open web browser, link new workout suggestions
				pass
			elif choice == ('q' or 'Q'):
				clear()
				break
			else:
				clear()
				print("Invalid option selected... \n")

if __name__ == '__main__':
	clear()
	while 1:
		print("Todays date is %s" % (str(datetime.date.today())))
		print('''
HELLO. WELCOME TO THE FITNESS PROGRAM!

Please select one of the following options:
===========================================
	[0] New Profile
	[1] Log In
	[2] Go to sticky
	[Q] Exit Program\n'''
			)
		choice = str(raw_input())

		if choice == '0':
			new_profile()
			clear()
		elif choice == '1':
			usr = str(raw_input("Please enter your name: \n"))
			log_in(usr)
		elif choice == '2':
			web.open("http://boards.4chan.org/fit/thread/\
				17018018/this-link-covers-the-following-topics")
			clear()

		elif choice == ('q' or 'Q'):
			print("\nExiting program...\n")
			break
		else:
			print("Invalid option\n")



	


