# CD Scrambler / Descrambler

Unconditionally applies ECMA-130 scrambling to an entire CD dump file (2352 bytes per sector).

Run the function again to descramble, it is a reversible function.

Usage:
`./scramble.exe dump.bin`

**Note**: This will overwrite dump.bin

---

If your file does not begin at the start of a sector, you can offset the scrambler by the number of bytes until the start of the first full sector, e.g.
`./scramble.exe dump.bin 16`
will descramble a file where the first full sector begins after 16 bytes.
