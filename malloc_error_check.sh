make

i=0
num_timeouts=0
echo "A timeout could either mean you did not check your malloc, or never malloced this many times"
while [[ $i -lt 1000 ]]; do
	ret=$(ulimit -t 1; ~/inject ~/injections/malloc.so ./miniRT MALLOC_NULL=$i ./scenes/valid/everything.rt 2>&1)
	exit_code=$?

	if [[ $exit_code == 137 ]]; then # check for timeout by ulimit
		echo "Malloc fail at: $i gives a timeout"
		num_timeouts=$(( $num_timeouts + 1 ))
		if [[ $num_timeouts -gt 10 ]]; then
			break
		fi
	fi

	i=$(( $i + 1 ))
done