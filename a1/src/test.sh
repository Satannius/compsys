#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."


# ASCII text files
printf "Hello, World!" > test_files/ascii01.input        # Plain ASCII
printf "Hello, World!\n" > test_files/ascii02.input      # W/ linebreak
echo "\n" > test_files/ascii03.input                     # Linebreak
printf "             \n" > test_files/ascii04.input      # Spaces w/ linebreak
printf "\x1b\n" > test_files/ascii05.input               # Hexadecimals
printf "^$?" > test_files/ascii06.input                  # Special chars
printf "\110\151" > test_files/ascii07.input             # Octals
printf "\72\105" > test_files/ascii08.input              # Decimals
printf "\110\105\x0a" > test_files/ascii09.input         # All the -als!
echo "Hello, World..." > test_files/ascii10.input        # Using echo


# DATA files
printf "Hello,\x00World!\n" > test_files/data01.input    # NUL + nl w/ ASCII
printf "\x00\x0a" > test_files/data02.input              # Hexadecimals
printf "\x48\x69\x00\n" > test_files/data03.input        # Mix hexadecimals
printf "\000\012" > test_files/data04.input              # Octals
printf "\0\10" > test_files/data05.input                 # Decimals
printf "\0\65\x0a" > test_files/data06.input             # All the -als!
printf "\x01\x02\x03\x04\x05" > test_files/data07.input  # Various hex codes
printf "\x06\x07\x08\x09\x10\x11" > test_files/data08.input     # More hex
printf "\x12\x13\x14\x15\x16\x17\x18" > test_files/data09.input # More hex
printf "\x1a\x1b\x1c\x1d\x1e\x1f\x20" > test_files/data10.input # More hex


# Empty files
printf "" > test_files/empty1.input # Empty files cannot be created with echo.
touch test_files/empty2.input       # Empty file created via touch


# ISO-8859 files
echo "ÆØÅ" > test_files/iso-8859                      # Danish letters
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859a.input
echo "¡Hola mundo!" > test_files/iso-8859             # Spanish symbols.
iconv -f UTF-8 -t ISO-8859-1 test_files/iso-8859 > test_files/iso-8859b.input
echo "Héllů" > test_files/iso-8859                    # Various accents.
iconv -f UTF-8 -t ISO-8859-2 test_files/iso-8859 > test_files/iso-8859c.input


# UTF-8 files
echo "ÆØÅ" > test_files/utf-8a.input                  # Danish letters
echo "ƎƎƎ" > test_files/utf-8b.input                  # Math symbol
echo "†††" > test_files/utf-8c.input                  # Cross symbol
echo "!\"#€%&/()=?" > test_files/utf-8d.input         # Various symbols
echo "°∑é®†¥ü|œπªß∂ƒ©«‹∆¬ä…ç√∫ñµ" > test_files/utf-8e.input # More symbols


# Little endian UTF-16 Unicode text
echo "In hoc codex vinces" > test_files/utf-16        # Motto
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-a
  FILE_ENCODING="$( file --brief --mime-encoding test_files/utf-16-a )"
  iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-a >\
    test_files/utf-16le-a.input

echo "12345678910" > test_files/utf-16                # Numbers
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-b
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-b > test_files/utf-16le-b.input

echo "H@XX0rƵƵ!" > test_files/utf-16                  # Various symbols
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-c  
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-c >\
  test_files/utf-16le-c.input

echo "\x00\x0a!" > test_files/utf-16                  # Hexadecimals
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-d
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-d >\
  test_files/utf-16le-d.input

echo "°∑é®†¥ü|œπªß∂ƒ©«‹∆¬ä…ç√∫ñµ" > test_files/utf-16 # More symbols
iconv -f UTF-8 -t UTF-16 test_files/utf-16 > test_files/utf-16-e
iconv -f "$FILE_ENCODING" -t UTF-16LE test_files/utf-16-e >\
  test_files/utf-16le-e.input

# Big endian UTF-16 Unicode text
iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-a >\
  test_files/utf-16be-a.input                                 # Motto

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-b >\
  test_files/utf-16be-b.input                                 # Numbers

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-c >\
  test_files/utf-16be-c.input                                 # Various symbols

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-d >\
  test_files/utf-16be-d.input                                 # Hexadecimals

iconv -f "$FILE_ENCODING" -t UTF-16BE test_files/utf-16-e >\
  test_files/utf-16be-e.input                                 # Hexadecimals

echo "Running tests.."
exitcode=0
f=test_files/*.input
echo ">>> Testing ${f}.."
file    ${f} | sed \
  -e 's/ASCII text.*/ASCII text/' \
  -e 's/UTF-8 Unicode text.*/UTF-8 Unicode text/' > test_files/expected \
  -e 's/Big-endian.*/Big-endian UTF-16 Unicode text/' \
  -e 's/Little-endian.*/Little-endian UTF-16 Unicode text/'
./file  ${f} > test_files/actual

if ! diff -u test_files/expected test_files/actual
then
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Success :-)"
fi

exit $exitcode
