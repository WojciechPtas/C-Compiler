## Purpose
This directory contains code that you may use in your c4 compiler. It is
intended as a help to implement certain parts of the parser and to showcase
implementation techniques used in practical compiler development.
We do not guarantee the absence of errors in the code. Feel free to not use or
heavily modify the code to your liking.

## Overview

### Diagnostics
The `diagnostics.h` and `diagnostic.cpp` files provide means to manage source
code locations (via the `Locatable` base class) and to consistently handle
input errors via the `errorloc` and the  `error` functions.

### Tokens
The `token.h`, `token.cpp`, `tokenkinds.def`, and `keywords.def` files provide
a structure for representing tokens. The `.def` files are used in combination
with the C(++) preprocessor to avoid listing all token kinds at multiple
places. The files do not contain all token kinds and keywords, you will need to
extent them.

### Symbol Interning
In the `string_internalizer.h` and `string_internalizer.cpp` files, you can
find a simple implementation of [string
interning](https://en.wikipedia.org/wiki/String_interning).

This allows to store a de-duplicated set of strings centrally during the lexing
phase so that all subsequent compiler phases can use the internalized `Symbol`s
(which are just pointers to the corresponding strings) for cheap comparisons
and indexing in data structures.

The remaining provided code expects `Symbol`s to be internalized via a
`SymbolInternalizer`.

### Declaration Parser
The `parser.h` and `parser.cpp` files contain parts of the implementation of a
c4 parser. On the one hand, they contain the declarations of generally helpful
methods that you might want to implement to use them for parsing.
On the other hand, they contain the `parseSpecDecl()` method with helper
methods that parse a declaration specifier followed by a declarator or an
abstract declarator.

These methods also contain constructor calls for the abstract syntax tree,
which you need to implement or replace with your own AST construction. Do
observe the TODO inside the `parseSpecDecl()` method that you will need to
implement for your AST.

You should extend the `Parser` class with more methods that parse an entire
translation unit.


