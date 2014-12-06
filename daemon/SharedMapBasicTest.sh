#!/usr/bin/env bash


runTest() {
	make testClient

	input="ASDF"
	./testClient 1 $input 3&

	sleep 1
	./testClient 1
	result=$(./testClient 1)

	if [ "$result" = "$input" ]; then
		return 1
	else
		return 0
	fi

}

runTest
