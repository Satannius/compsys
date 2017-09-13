#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."

# ASCII text files
printf "Hello, World!" > test_files/ascii1.input    # Plain ASCII
printf "Hello, World!\n" > test_files/ascii2.input  # W/ linebreak
echo "\n" > test_files/ascii3.input               # Linebreak
printf "             \n" > test_files/ascii4.input  # Spaces w/ linebreak
printf "\x1b\n" > test_files/ascii5.input           # Hexadecimals
printf "^$?" > test_files/ascii6.input              # Special chars
printf "\110\151" > test_files/ascii7.input         # Octals
printf "\72\105" > test_files/ascii8.input          # Decimals
printf "\110\105\x0a" > test_files/ascii9.input     # All the -als!
printf "Hello, World..." > test_files/ascii10.input    # Using echo

# DATA files
printf "Hello,\x00World!\n" > test_files/data1.input    # NUL + nl w/ ASCII
printf "\x00\x0a" > test_files/data2.input              # Hexadecimals
printf "\x48\x69\x00\n" > test_files/data3.input        # Mix hexadecimals
printf "\000\012" > test_files/data4.input              # Octals
printf "\0\10" > test_files/data5.input                 # Decimals
printf "\0\65\x0a" > test_files/data6.input             # All the -als!
printf "\x01\x02\x03\x04\x05" > test_files/data7.input  # Various hex codes
printf "\x06\x07\x08\x09\x10" > test_files/data8.input  # Various hex codes

# Empty files
printf "" > test_files/empty1.input # Empty files cannot be created with echo.
touch test_files/empty2.input

echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    "${f}" | sed 's/ASCII text.*/ASCII text/' > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done
exit $exitcode
