#!/usr/bin/env bash

make testClient

(sleep 1; ./testClient 1)&

./testClient 1 asdf 3

