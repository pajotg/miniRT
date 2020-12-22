#!/bin/bash
if [ $# != 1 ]; then
	echo -e "\e[1;91mUsage: run_test.sh {what test}"
	return 1 2>/dev/null
	exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

test="$1"

# make sure that what we want to test is up to date
make_result=$(make -C $DIR/..)

# test!
~/Desktop/JTester/run_test.sh "$DIR/../TestBinder.a" "-I $DIR/../include" "$DIR"