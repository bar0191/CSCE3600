#! /bin/bash

# Author      : Brandon Reid
# Date        : 9/22/2016
# Assignment  : minor3.sh
# Course      : CSCE3600 - Thompson
# Description : simple script to run ls, ps U on user/users, and factor

# quit is used as a bool type value to stop menu loop when 
quit=false

# if multiple arguements found, print greeting
if [ $# -eq 1 -o $# -eq 2 ]; then
	echo Good day, $1! Nice to meet you!
fi

# do-while style loop
# will stop once quit = true
while [ $quit != true ]
do
	# if there are no args, echo usage and exit with error
	if [ $# -eq 0 ]; then
		echo "usage: minor3 name [euid]"
		exit 1
	# else if there are 1 or 2 args, then echo menu
	elif [ $# -eq 1 -o $# -eq 2 ]; then
		# menu prompt
		echo +*******************************************************************+
		echo "Enter one of the following options:                                 |"
		echo "1) List and count all non-hidden files in the current directory.    |"
		echo "2) Check if given user (default = current user) is logged in, then  |"
    		echo "   ... list all active processes for that user.                     |"
		echo "3) Display prime factors of user-entered positive integer.          |"
		echo "4) Exit this shell script.                                          |"
		echo +*******************************************************************+
	
		# prompt user for menu choice
		read -p ">" input

		# case statement for menu options
		case $input in
		1) 	# echo total files and list directory files 
			# ls -1 will print plain list of files piped into a wordcount
			# wc -l will count each line and output total lines
			# this should end up being the amount of files in the directory
			echo "==> Total number of files: " `ls -1 | wc -l`
			ls ;; 
		2) 	# if there are 2 args then take in euid from args
			# check if user is online through who | grep (-q makes it silent)
			if [ $# -eq 2 ]; then
				euid=$2
				echo -n "Active processes for $euid "
				who | grep -q $euid
				# if most recent command (who) is true(zero)
				if [ $? -eq 0 ]; then
					echo "(logged in):"
				else 
					echo "(NOT logged in):"
				fi
				ps U $euid	# run processes
			# else only one arg, take euid from whoami
			else
				euid=`whoami`	
				echo -n "Active processes for $euid "
				who | grep -q $euid
				# if most recent command (who) is true(zero)
				if [ $? -eq 0 ]; then
					echo "(logged in):"
				else
					echo "(NOT logged in):"
				fi
				ps U $euid	# run processes
			fi ;;
		3)	# echo factor of postive value prompted by the user
			echo "==> Display prime factors of user-entered integer:"
			read -p "Enter a positive integer to find its prime factors: " intValue
			factor $intValue ;;
		4)	# echo friendly exit, and make quit = true 
			echo "==> Exit this shell script:"
			echo Thanks, $1! Have a great day!
			quit=true ;;
		*)	# default
			echo Invalid option. Please select again!
		esac
	# else if arguements are over 2, output exit error
	else 
		echo Maximum number of arguements exeeded...
		echo "usage: minor3 name [euid]"
		exit 1   
	fi
done
