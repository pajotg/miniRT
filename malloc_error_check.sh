make

#malloc 108 causes a segfault, in mlx_init
#malloc 169 causes a segfault, in mlx_init
#malloc 169 causes a segfault, in mlx_new_window

i=0
max_num_mallocs=0
tests_since_last_print=0
echo "A timeout could either mean you did not check your malloc, or never malloced this many times"
while [[ $i -lt 1000 ]]; do
	if [[ $i -gt 132 && $i -lt 142 ]] || [[ $i -gt 277 && $i -lt 292 ]]; then # A couple mallocs in this range cause infinite loops
		i=$(( $i + 1 ))
		continue;
	fi
	#echo "------------"
	#echo "Running test $i..."
	ret=$(ulimit -t 1; ~/custom_commands/inject ~/injections/malloc.so ./miniRT MALLOC_NULL=$i ./scenes/valid/everything.rt 2>/dev/null)
	#echo "Test $i completed!"
	exit_code=$?

	current_num_mallocs=$(echo $ret | egrep -o "Num mallocs: [0-9]+")
	if [[ "$current_num_mallocs" ]]; then
		current_num_mallocs="${current_num_mallocs:13}"
		if [[ $current_num_mallocs -gt $max_num_mallocs ]]; then
			max_num_mallocs=$current_num_mallocs
		fi

		#expected=$(( $i + 2 ))	# + 2 for because yeah, malloc null: error, if error: ft_strjoin("Could not do X, reason: ", get_last_error())
		# but eh, dont really care, some system calls also still trigger this error
		#if [[ $current_num_mallocs -gt $expected ]]; then
		#	echo "The $i-th malloc returning null causes more mallocs than expected: $current_num_mallocs"
		#fi
	fi

	#echo $i = $current_num_mallocs
	#echo $i = $exit_code
	if [[ $exit_code == 139 ]]; then # check for seg fault
		echo "The $i-th malloc returning null causes a segfault (max mallocs: $max_num_mallocs)"
	fi
	if [[ $exit_code == 137 ]]; then # check for timeout by ulimit
		echo "The $i-th malloc returning null causes a timeout (max mallocs: $max_num_mallocs)"
	fi

	tests_since_last_print=$(( $tests_since_last_print + 1 ))
	if [[ $tests_since_last_print -gt 100 ]]; then
		tests_since_last_print=0
		echo "Test $i completed!"
	fi
	i=$(( $i + 1 ))
done