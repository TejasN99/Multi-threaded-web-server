#!/bin/bash

user_count=17750
max_users=19000

while [ $user_count -le $max_users ]
do
	taskset -c 1,2,3 ./load_gen localhost 8080 $user_count .4 60
	let user_count=user_count+250
done
