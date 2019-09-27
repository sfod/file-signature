# file-signature

Test utility for creating signature file of the specified input file, i.e. for each block from input file calculate hash and write it to the signature file.

You could get the help message by executing binary with a cmd option `--help`.


### Design

Application maps input file into memory and process it block by block in parallel in two stages:
1. Calculate hash.
2. Write hash into the output file.
