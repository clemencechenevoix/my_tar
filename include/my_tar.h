// ----- include ----- //
#include <sys/stat.h> // include: stat, lstat
#include <dirent.h> // opendir, closedir
#include <sys/sysmacros.h> // major, minor

#include "./utils.h"

// ----- function ----- //

// --- option.c --- //

length_header_t* init_length_header();

// --- main_var (flags) --- //
flag_t* get_main_var(char**, int);

// --- my_tar.c --- //
void my_tar();