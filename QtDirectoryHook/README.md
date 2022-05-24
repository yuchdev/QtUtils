## Set directory hook

GUI application for setting a hook to directory or directories, tracing list of changed files or sub-directories, and performing copying of changed files to an other custom directory.

* Requies Qt 5 for compilation
* Under Windows Cmake requires to set the version explicitly, `-DCMAKE_SYSTEM_VERSION=6.1` for Windows 7, `6.2` for Windows 8, `6.3` for Windows 8.1 and `10.0` for Windows 10
* Assumed you have rights for setting a hook to a directory
* Filename wildcards cupported
* C++03 is supported for compatibility, the application is designed to be built by really old compilers

