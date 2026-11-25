// ----- include ----- //
#include <stdio.h> // debuging

#include <fcntl.h> // include: open
#include <unistd.h> // include: close
#include <stdlib.h> // include: malloc, free

// ----- structure ----- //

typedef struct flags_s{
    char action;
    int* array_file;
    int length_file;
    int index_tarball;
    int status;
}flag_t;

typedef struct posix_header
{                              /* byte offset */
  char name[100];               /*   0 */
  char mode[8];                 /* 100 */
  char uid[8];                  /* 108 */
  char gid[8];                  /* 116 */
  char size[12];                /* 124 */
  char mtime[12];               /* 136 */
  char chksum[8];               /* 148 */
  char typeflag;                /* 156 */
  char linkname[100];           /* 157 */
  char magic[6];                /* 257 */
  char version[2];              /* 263 */
  char uname[32];               /* 265 */
  char gname[32];               /* 297 */
  char devmajor[8];             /* 329 */
  char devminor[8];             /* 337 */
  char prefix[155];             /* 345 */
  char padding[12];             /* 500 */
                                /* 512 */
}start_header_t;

typedef struct length_header
{
  int length_array[18];
  int length;
}length_header_t;

// ----- Function ----- //

int my_length(char*);
void my_strcpy(char*, char*);
void my_memset(char*, char, int);
int my_write(char*, int);
int my_power(int, int);
void my_itoa(char*, int);
int my_atoi(char*);

// - check - //
int check_if_char_in_string(char*, char);
int check_if_file_exist(char*);

// - error - //
flag_t* error_flag(flag_t*, char*, int);