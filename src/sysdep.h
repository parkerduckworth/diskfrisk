/* System Dependencies ~macOS High Sierra 10.13.3 */
#ifndef SYSDEP_H
#define SYSDEP_H

/* Bash syntax */
#define RDFROMS   "-c"      // Read-from-string option
#define SHOPEN    "open "   // Open command
#define SHELL     "sh"      // Initiate new environment context

/* OSX nomenclature */
#ifdef __APPLE__
#define HNAME     "Users"   // Home directory name
#define HOME      "/Users"  // Home directory path
#define ROOT      "/"       // Root directory path
#endif


//Defining for compilation on Linux
#ifdef __linux__

#define HNAME     "home"   // Home directory name
#define HOME      "/home"  // Home directory path
#define ROOT      "/"      // Root directory path
#endif

#endif 
