# DISKFRISK -- VERSION 0.0.0

## Here is a short explanation of how to execute commands, with examples:

To use the following example commands verbatim, compile diskfrisk.c to an 
executble and set it to $PATH.  Otherwise, execute commands with `./a.out` 
or `./frisk`

If no flags present, your machine's entire file tree is searched, by
exact match.

Ex: `~ user$ frisk <filename>`

To search the `home` dir (user files) only, input the `-h` flag.

Ex: `~ user$ frisk -h <filename>`

If you want to search only system files, input `-s`.
Ex: `~ user$ frisk -s <filename>`

If both `-h` and `-s` are input, the result is the same as if no 
flags are input. They can also be input as `-sh` or `-hs`. Order
doesn't matter here.

Ex: `~ user$ frisk -h -s <filename>`

An `-o` flag is available to automatically open up the first matched 
result of search.  The search will continue, and all matches will be
listed.

Case sensitivity can be toggled on with a `-C` flag.  

Directories can be searched, and set to open upon discovery as well.

A `-p` flag is another option that sets all file permission errors to
display when thrown.

If you wish to search by pattern, rather than by exact match, preface
your `<filename>` with `grep:`, and all files containing the matching
pattern will be output. This can be used in conjuction with any (or all)
of the optional flags.

Ex: `~ user$ frisk grep:<filename>`
