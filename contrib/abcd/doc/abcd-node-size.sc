# This data file was generated by the Spreadsheet Calculator.
# You almost certainly shouldn't edit it.

set numeric
format A 12 0 0
format C 4 2 0
let A1 = 29
leftstring B1 = " # bits for node idx"
let A2 = 13
leftstring B2 = " # bits for a variable"
leftstring A3 = "----------------------------------"
let A4 = 2^A1
leftstring B4 = " maximal number of nodes"
let A5 = 2^A2
leftstring B5 = " maximal number of variables"
let A7 = 2*A1+A2+2
leftstring B7 = " original ABCD (2idx + var + mark + sign)"
leftstring B8 = "      ="
let C8 = A7/32
leftstring D8 = " words (32-bit)"
let A10 = A1*3+A2+2
leftstring B10 = " ABCD with efficient gc (3idx + var + sign + mark)"
leftstring B11 = "      ="
let C11 = A10/32
leftstring D11 = " words (32-bit)"
let A13 = A1*5+A2+2
leftstring B13 = " ABCD with parent pointers (5idx + var + sign + mark)"
leftstring B14 = "      ="
let C14 = A13/32
leftstring D14 = " words (32-bit)"
goto A4