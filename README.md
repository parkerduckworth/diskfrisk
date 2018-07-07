# DISKFRISK
A simple, thorough file-search solution that executes from the terminal.

## Motivation
As developers, we spend significant time at the command line. Born of the 
desire to search for files without constantly switching windows, **DISKFRISK** 
enables quick and easy file lookups.  With features such as auto-open, adjustable 
case sensitivity, and pattern matching, searches can be tailored for any need.

<!-- ## Build status
Build status of continus integration i.e. travis, appveyor etc. Ex. - 

[![Build Status](https://travis-ci.org/akashnimare/foco.svg?branch=master)](https://travis-ci.org/akashnimare/foco)
[![Windows Build Status](https://ci.appveyor.com/api/projects/status/github/akashnimare/foco?branch=master&svg=true)](https://ci.appveyor.com/project/akashnimare/foco/branch/master) -->

<!-- ## Code style
If you're using any code style like xo, standard etc. That will help others while contributing to your project. Ex. -

[![js-standard-style](https://img.shields.io/badge/code%20style-standard-brightgreen.svg?style=flat)](https://github.com/feross/standard)
 
## Screenshots
Include logo/demo screenshot etc.

## Tech/framework used
Ex. -

<b>Built with</b>
- [Electron](https://electron.atom.io) -->
## Features
- **Case Sensitivity:** Can be toggled on and off.
- **Target Directory Selection:** Set base directory as needed.
- **Auto-Open:** Automatically open first match upon discovery.
- **Permission Error Display:** Helpful if unsure of file permission settings.
- **Pattern Matching:** GREP it.

## Code Example
To initiate DISKFRISK, enter `dfstart`.
```
USER-MBP:~ user$ dfstart
DISKFRISK INITIATED
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ 
```

Type in your search.  
Begin each query with `find`,   
followed by whichever options are desired, then the filename.
```
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ find -h main.c


DISKFRISK -- VERSION 0.0.0


Home directory is being frisked...
Searching for: main.c

```
In the above example, DISKFRISK will search for all files matching `main.c` with the home directory as its base.    
To exit, enter `q`. 
```
USER-MBP:~ user$ dfstart
DISKFRISK INITIATED
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ q                  
exiting...
USER-MBP:~ user$
```

All options and funcitonality will be described in the [API Reference](#api-reference).

## Installation
1. Clone this repository.
2. While in the base project directory, enter `make find`.
3. Open your `.bash_profile` and add `source <path>/diskfrisk/bin/exec.sh` to an open line. 
4. Initiate DISKFRISK with `dfstart` from any working directory.

(New users, make sure to replace `<path>` with the absolute path to the cloned repository.)

## API Reference

DISKFRISK was specifically designed with simplicity as its core value.
Queries are comprised of three parts: (executable)(options)(query)

With this structure in mind, every search is began with the executable, `find`.

Options:

| Syntax  | Purpose                                                |
|---------|--------------------------------------------------------|
| `-C`    | Toggle on case sensitivity (uppercase 'C')             |
| `-h`    | Select home directory as base for search (user files)  |
| `-s`    | Select root directory as base for search (system files)|
| `-o`    | Auto-open first file that matches the query            |
| `-p`    | Display file permission errors                         |
| `grep:` | Search by pattern match (default is exact match)       |

Note: 
- the `grep:` option is the only option that will not be separated by whitespace from the query, as displayed in the example below.

Example: 
- search for any file matching the pattern "unix", with the home directory as base, 
auto-open the first match, and display all file permission errors.
```
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ find -hop grep:unix


DISKFRISK -- VERSION 0.0.0


Home directory is being frisked...
Searching for pattern: unix


```
> This shows how options can be combined into a single flag. Options can 
> appear in any order and be combined together.  If not combined, they must 
> be separated by whitespace.  All options must be entered between the 
> executable and the query.


## Tests
All tests are located in the `tests` folder.
1. cd into `tests`.
2. compile:
```
cc <test_filename>.c -o test_execs/<test_filename> -lcunit
```
3. run:
```
test_execs/<test_filename>
```

## Contribute

Contribution Guideline coming soon.

<!-- ## Credits
Give proper credits. This could be a link to any repo which inspired you to build this project, any blogposts or links to people who contrbuted in this project.  -->

## License
Please feel free to use, share, and extend this project. PR's welcome.

MIT © [Parker Duckworth](http://github.com/parkerduckworth)
