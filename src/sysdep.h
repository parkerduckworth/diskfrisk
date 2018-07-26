#ifndef SYSDEP_H
#define SYSDEP_H
 
/* Bash syntax */
#define RDFROMS   "-c"      // Read-from-string option
#define SHELL     "sh"      // Initiate new environment context

/* OSX nomenclature */
#ifdef __APPLE__
#define HNAME     "Users"   // Home directory name
#define HOME      "/Users"  // Home directory path
#define ROOT      "/"       // Root directory path
#define SHOPEN    "open "   // Open command (space necessary for valid bash commands)
#endif

//Defining for compilation on Linux
#ifdef __linux__

#define HNAME     "home"      // Home directory name
#define HOME      "/home"     // Home directory path
#define ROOT      "/"         // Root directory path
#define SHOPEN    "xdg-open " // Open command (space necessary for valid bash commands)
#endif

#endif
