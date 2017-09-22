# Griddlers-Puzzles Solver

This C++ Program is a solver for Griddler problems, based on an open-source SAT solver named [Minisat](http://http://minisat.se/).

## Usage

	Griddler [OPTION...]
	To understand the file format, see the examples.

	-i, --input-file arg    The file with the Griddler problem. Defaults to
	                        stdin.
	-o, --output-file arg   The file to the print the solution. Defaults to
	                        stdout.
	    --output-style arg  The style of the output - "O" (default), "bricks",
	                        or "bits"
	-h, --help              Display this help

<sup> Built using command-line parser [cxxopts](https://github.com/jarro2783/cxxopts). </sup>

## The Griddler Puzzle

The Griddler puzzle is a nice game, where clues are given about a certain binary drawing, and the solver needs to gradually figure out what the drawing it - which cells are black and which are white.

The clues are of the following format: for each row and for each column, the lengths of all the black-cell-streaks are given.

For example, a problem could look like this:

||4|3|3|5|2|
|-----|------|-----|-----|------|-----|
|2||||||
|2, 2||||||
|4||||||
|4||||||
|1||||||

and the solution would be a simple sketch of a dog.

## The SAT Solver

The SAT (boolean satisfiability) problem is the following: given a boolean formula, does it have an interpretation (variable values) that satisfy it. It is an NP-complete problem, but many good implementations exist for it so that the runtime is reasonable. 
Usually, SAT solvers work on CNF formulas, which means the formula is an AND of clauses, each clause is an OR of literals (variable or its negation). SAT solvers also usually give the actual variables values, not only if the formula is satisfiable or not.

Minisat is an award-winning open-source SAT solver, so why not?

## Representing the Griddlers problem as CNF

To explain the representation of the Griddlers problem as CNF, one has to explain what the semantic meaning of the variables is, and what types of clauses are there in the formula.

### Variable types

There are two types of variables: cell-variables are block-start variables. 

The cell variables (named "r3c3" for example) represent whether a specific cell is black or not.

The block-start variables (named "r2b0s2") represent whether a specific block (referenced by its row/column, and by its index in the row/column) starts in a specific index.


### Block-starts-somewhere clauses

These clauses come to make sure that each block has at least one position. For example, for block 2 of row 3, the clause could look like this:

r3b2s0 or r3b2s1 or r3b2s2 or r3b2s3

### Block-doesn't-duplicate clauses

These clauses come to make sure that for every block, no more than one block-position variable is set. For block 2 of row 3, the clauses could look like this:

!r3b2s0 or !r3b2s1 <br>
!r3b2s0 or !r3b2s2 <br>
!r3b2s0 or !r3b2s3 <br>
!r3b2s1 or !r3b2s2 <br>
!r3b2s1 or !r3b2s3 <br>
!r3b2s2 or !r3b2s3

### Block-to-cell clauses

These clauses come to represent the bond between cells and blocks - if a block of length 2 starts at index 3, then there are two cells that must be black. The clauses could look like this:

!r0b0s3 or r0s3
!r0b0s3 or r0s4

### Cell-to-block clauses

These clauses make sure that cells are black only if there is a block that causes them to be black. The clause could look like this:

!r0c1 or r0b0c0 or r0b0c1

### Block-order clauses

These clauses come to enforce the order of the blocks as appears in the puzzle. They are only relevant on rows/columns with more than one block. If block 0 of row 0 is of length 2, the clauses could look like this:

!r0b0c1 or !r0b1c0
!r0b0c1 or !r0b1c1
!r0b0c1 or !r0b1c2
!r0b0c1 or !r0b1c3
