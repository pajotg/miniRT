make

i=0
num_timeouts=0
max_num_mallocs=0
echo "A timeout could either mean you did not check your malloc, or never malloced this many times"
while [[ $i -lt 1000 ]]; do
	#echo "------------"
	ret=$(ulimit -t 1; ~/custom_commands/inject ~/injections/malloc.so ./miniRT MALLOC_NULL=$i ./scenes/valid/everything.rt 2>/dev/null)
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
	if [[ $exit_code == 137 ]]; then # check for timeout by ulimit
		echo "The $i-th malloc returning null causes a timeout (max mallocs: $max_num_mallocs)"
		num_timeouts=$(( $num_timeouts + 1 ))
		if [[ $num_timeouts -gt 10 ]]; then # 10 timeouts probably mean that you never malloc more than this
			break
		fi
	fi

	i=$(( $i + 1 ))
done