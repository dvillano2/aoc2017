AOC 2017 in C.
A learning exercise.
Prior to doing this, I had written no C.
I found this pretty fun, and once December rolled around I did 2025 in C also. You can find that in another repo on this github.

This was written at the recurse center in October and November 2025.

If you're interested in compiling or running any of this, here are instructions

To compile everything run

```bash
make
```

or

```bash
make all
```

To compile a specific day run

```bash
make day day=03
```
change 03 appropriately, days should be between 01 and 25.

To compile a specific part of a specific day, run

```bash
make part day=04 part=01
```
Again, change the day as necessary. part should be 01 or 02. Note there is no part 2 for day 25

If you'd like to run the code, for almost all problems you'll need to put your input files in the appropriate directory with the name "input.txt". So your input file for the fourth day should be 04_passphrase/input.txt

To run everything:
```bash
make run
```
a specific day:
```bash
make run-day day=04
```
a specfic part:
```bash
make run-part day=04 part=01
```
Change days and parts as you need.
