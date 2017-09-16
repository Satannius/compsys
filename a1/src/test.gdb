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

# Defining macros, since gdb will not detect those defined in file
macro define UTF8_2B(value) (((value & 0xe0) == 0xc0) ? 1 : 0) // 2-byte
macro define UTF8_3B(value) (((value & 0xf0) == 0xe0) ? 1 : 0) // 3-byte
macro define UTF8_4B(value) (((value & 0xf8) == 0xf0) ? 1 : 0) // 4-byte
macro define UTF8_CONT(value) (((value & 0xc0) == 0x80) ? 1 : 0) // continuation byte

#
p "Some basic tests.."
p UTF8_CONT(128) != 0
p UTF8_2B(192) != 0
p UTF8_3B(224) != 0
p UTF8_4B(240) != 0
#
p "Testing UTF8_CONT.."
p UTF8_CONT(128 + 1) != 0
p UTF8_CONT(128 | 1) != 0
p UTF8_CONT(128 | 63) != 0
p UTF8_CONT(128 | 63) > 0
p UTF8_CONT(128 + 64) == 0
p UTF8_CONT(128 | 64) == 0
#
p "Testing UTF8_2B.."
p UTF8_2B(128 + 64) != 0
p UTF8_2B(128 | 64) != 0
p UTF8_2B(128 | 64 | 31) != 0
p UTF8_2B(128 | 64 | 31) > 0
p UTF8_2B(128 + 64 + 32) == 0
p UTF8_2B(128 | 64 | 32) == 0
#
p "Testing UTF8_3B.."
p UTF8_3B(128 + 64 + 32) != 0
p UTF8_3B(128 | 64 | 32) != 0
p UTF8_3B(128 | 64 | 32 | 15) != 0
p UTF8_3B(128 | 64 | 32 | 15) > 0
p UTF8_3B(128 + 64 + 32 + 16) == 0
p UTF8_3B(128 | 64 | 32 | 16) == 0
#
p "Testing UTF8_4B.."
p UTF8_4B(128 + 64 + 32 + 16) != 0
p UTF8_4B(128 | 64 | 32 | 16) != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7) != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7) > 0
p UTF8_4B(128 + 64 + 32 + 16 + 8) == 0
p UTF8_4B(128 | 64 | 32 | 16 | 8) == 0
#
p "Student tests.."
p UTF8_2B(384 >> 1) != 0
p UTF8_3B(56) == 0
p UTF8_4B(255) == 0
p UTF8_CONT(64 << 1) != 0

set pagination on

q
