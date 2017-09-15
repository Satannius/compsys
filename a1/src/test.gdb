# Start with:
# $ gdb file -x test.gdb
# The -x flag tells gdb to execute the commands in this file.

# Exercise 3.1-6

# Set pagination off: gdb will not pause to wait for user confirmation in between many print statements.
set pagination off

# p = print, x: print in hex, t: print in bin
p "Hex representation of 192_10:"
p/x 192
p "Bin representation of 192_10:"
p/t 192
p "Bin representation of 80_16:"
p/t 0x80
p "Dec representation of 80_16:"
p 0x80
p "Hex representation of 110_2:"
p/x 0b110
p "Dec representation of 110_2:"
p/d 0b110

set pagination on

run



q
