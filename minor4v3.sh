#! /bin/bash

# Author      : Brandon Reid
# Course      : CSCE3600
# Date        : 10/4/16
# Assignment  : minor4
# Description : script to monitor when users log in and out of a system
 
#global variables
trap 'threetimes' 2
trapCount=3		
declare -a euids	# array to store users
i=0			# index for array

# function for trap on ^C
threetimes()
{
	# while trap count is greater then 0
	while [ $trapCount -gt 0 ]
	do	
		# output (SIGINT) ignored
		echo "(SIGINT) ignored. Enter ^C $trapCount more times to terminate program."
		let trapCount=$trapCount-1 #increment trap count 3 times
		break
	done
	# once trap count equals 0
	if [ $trapCount -eq 0 ]; then
		trap 2	# reset the trap
	fi
}

# function to initialize users to the array
initUsers()
{
	# initialize # of users
	initNumOfUsers=`users | wc -w`
	
	# loop through users, initialize array
	for users in `users`
	do
		euids[i]=$users
		let i=$i+1 # increment index
	done
	
}

#function to initialize users and print out users
printUsers()
{
	# initialize # of users
	initNumOfUsers=`users | wc -w`
	
	# loop through users, print logged in
	for users in `users`
	do
		echo "> $users logged in"
		euids[i]=$users
		let i=$i+1 # increment index
	done
}

# print initial users logged in
echo "`date` ) initial users logged in"
printUsers
echo "`date` ) # of users: $initNumOfUsers"
sleep 10

# after initial print
# loop continuously until trap is reset
while true
do
	# initialize some bool values to check for duplicates
	logoutDupCheck=true;
	loginDupCheck=false;
	
	# re-initialize # of users	
	numOfUsers=`users | wc -w`

	# the # of users allows us to know whether someone has logged in or out
	# this will check numof users from prev 10 seconds to next 10 seconds

	# if # of users has changed... then check if less or more
	if [ $numOfUsers -ne $initNumOfUsers ]; then
		# if # of users is less then before..
		if [ $numOfUsers -lt $initNumOfUsers ]; then
			# then increment through array of prev users
			for i in "${euids[@]}"
			do
				
				# checks through who to see who is logged out (1)
				who | grep -q $i

				# check who to see if value comes back false
				if [ $? -eq 1 ]; then
					#then echo user logged out
					echo "> $i logged out"
					logoutDupCheck=false; # indicate user wasn't a dup
					continue
				fi	
			done
			initUsers # re-init users
			# when a duplicate user is logged in, the who into grep will not catch a dup logout
			# so the bool value will indicate if the user is still logged in
			# this indicates the user has duplicate users, and will echo results
			if [ $logoutDupCheck = true ]; then
				echo "`date` ) duplicate user(s) logged out"
				printUsers # re-print list
			fi
			initUsers # re-initialize array with updated users
		# if # of users is greater then before...
		elif [ $numOfUsers -gt $initNumOfUsers ]; then
			# to see who the last person logged in was...
			# you could simply use the last command with a little manip
			# this will indicate the most recent person who logged in
			dif=0
			declare -a newEuids
			let dif=$numOfUsers-$initNumOfUsers
			j=0
			f=11
			lastEuid=`last | head -$dif`
			newestEuid=$(echo $lastEuid | cut -d" " -f1)
			if [ $dif -gt 1 ]; then
				until [ $j = $dif ]
				do
					newEuids[j]=$(echo $lastEuid | cut -d" " -f$f)
					echo $newEuids[$j]
					let f=$f+10
					let j=$j+1
				done
			fi
			

			# loop through array of euids
			for i in "${euids[@]}"
			do
				#if an element in the array equals newest loggin
				#it was a duplicate user
				if [ $i == $newestEuid ]; then
					# output duplicate user, and set bool value
					echo "`date` ) duplicate user(s) logged in"
					printUsers # re-print users
					loginDupCheck=true;
					break
				fi
			done
			# if dup isn't found, simply print newest user logged in
			if [ $loginDupCheck = false ]; then
				echo "> $newestEuid logged in"
			fi
			initUsers # re-initialize the array with updated users
		fi	
	fi
	# print # of users after changes, or every 10 seconds with no change made
	echo "`date` ) # of users: $numOfUsers"
	sleep 10
	continue	
done
