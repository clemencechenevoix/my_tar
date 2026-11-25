// ----- include ----- //
#include "../include/my_tar.h"

// ----- function ----- //

length_header_t* init_length_header() {
    length_header_t* struct_length_header = malloc(sizeof(length_header_t));
    struct_length_header->length = 16;
    
    struct_length_header->length_array[0] = 100;
    struct_length_header->length_array[1] = 8;
    struct_length_header->length_array[2] = 8;
    struct_length_header->length_array[3] = 8;
    struct_length_header->length_array[4] = 12;
    struct_length_header->length_array[5] = 12;
    struct_length_header->length_array[6] = 8;
    struct_length_header->length_array[7] = 1;
    struct_length_header->length_array[8] = 100;
    struct_length_header->length_array[9] = 6;
    struct_length_header->length_array[10] = 2;
    struct_length_header->length_array[11] = 32;
    struct_length_header->length_array[12] = 32;
    struct_length_header->length_array[13] = 8;
    struct_length_header->length_array[14] = 8;
    struct_length_header->length_array[15] = 155;

    return struct_length_header;
}

// --- main var --- //
int check_argv_1(flag_t* struct_flag, char** argv) {
    int length = my_length(argv[1]);
    if (argv[1][0] =! "-" || length < 2 || length > 3) {
        error_flag(struct_flag, "my_tar: You must specify one and only one of the '-ctrux' options\n", -1);
        return -1;
    }

    char* autorize_action = malloc(sizeof(char) * 6);
    my_strcpy(autorize_action, "ctrux");

    int is_f = 0;
    int index = 2;

    if (check_if_char_in_string(autorize_action, argv[1][1]) == 0) {
        free(autorize_action);
        error_flag(struct_flag, "my_tar: You must specify one and only one of the '-ctrux' options\n", -1);
        return -1;
    }
    if (length == 3) {
        if(argv[1][2] != 'f') {
            free(autorize_action);
            error_flag(struct_flag, "my_tar: You must specify one and only one of the '-ctrux' options\n", -1);
            return -1;
        } else {
            is_f = 1;
            struct_flag->index_tarball = 2;
            index += 1;
        }
    }
    free(autorize_action);
    return is_f;
}

int check_argv(flag_t* struct_flag, char** argv, int argc, int is_f, int index) {
    int nb_file = 0;

    while (index < argc) {
        if (argv[index][0] == '-') {
            if ((is_f == 0) || (my_length(argv[index]) == 2 && argv[index][1] == 'f')) {
                if(index + 1 >= argc) {
                    error_flag(struct_flag, "my_tar: You may indicate an archive with -f\n", -3);
                    return -1;
                }else {
                    index += 1;
                    struct_flag->index_tarball = index;
                    is_f = 1;
                }
            } else {
                error_flag(struct_flag, "my_tar: You may not specify more than one '-ctrux' or '-f' option\n", -2);
                return -1;
            }
        } else {
            if (check_if_file_exist(argv[index]) == 1){
                struct_flag->array_file[nb_file] = index;
                nb_file += 1;
            } else {
                printf("my_tar: %s", argv[index]);
                error_flag(struct_flag, ": can't open file or directory\n", -4);
                return -1;
            }
        }
        index += 1;
    }
    struct_flag->length_file = nb_file;
    return is_f;
}

flag_t* get_main_var(char** argv, int argc) {
    flag_t* struct_flag = malloc(sizeof(flag_t));
    struct_flag->status = 0;

    if(argc < 3) {
        return error_flag(struct_flag, "my_tar: You must specify one of the '-ctrux' options\n", -1);
    }
    
    int is_f = check_argv_1(struct_flag, argv);
    if (struct_flag->status < 0) {
        return struct_flag;
    }

    struct_flag->array_file = malloc(sizeof(int) * (argc - 2));

    int index = 2;
    if (is_f == 1) {
        index += 1;
    }

    is_f = check_argv(struct_flag, argv, argc, is_f, index);
    if (struct_flag->status < 0) {
        return struct_flag;
    }
    
    if (is_f == 0) {
        return error_flag(struct_flag, "my_tar: Refusing to write archive contents to terminal (missing -f option?)\n", -5);
    }

    return struct_flag;
}

// --- create --- //

int create_file(char* file_name) {
    int fichier = open(file_name, O_CREAT);
    if (fichier != -1) {
        close(fichier);
    }
    return 0;
}

start_header_t* get_start_header(char* file_name) {
    start_header_t* struct_start_header = malloc(sizeof(start_header_t));
    struct stat informations;
    stat(file_name, &informations);
    // vérifié stat
    
    my_strcpy(struct_start_header->name,file_name);
    
    // stat
    my_itoa(struct_start_header->mode, informations.st_mode);
    my_itoa(struct_start_header->uid, informations.st_uid);
    my_itoa(struct_start_header->gid, informations.st_gid);
    my_itoa(struct_start_header->size, informations.st_size);
    my_itoa(struct_start_header->mtime, informations.st_mtime);
    my_itoa(struct_start_header->devmajor, major(informations.st_dev));
    my_itoa(struct_start_header->devminor, minor(informations.st_dev));

    my_itoa(struct_start_header->chksum, 512);
    struct_start_header->typeflag[0] = '0';
    my_strcpy(struct_start_header->linkname, "\0");
    my_strcpy(struct_start_header->magic, "ustar");
    my_strcpy(struct_start_header->version, "00");
    my_strcpy(struct_start_header->uname, "\0");
    my_strcpy(struct_start_header->gname, "\0");
    my_strcpy(struct_start_header->prefix, "\0");
    
    return struct_start_header;
}

void write_category_header(char* category_info, char* null_string, int length_category, int fd) {
    int length_info_category = my_length(category_info);
    printf("%s ,%d, %d, %d\n", category_info, length_info_category, length_category, length_category - length_info_category);
    write(fd, category_info, length_info_category);
    write(fd, null_string, length_category - length_info_category);
}

void write_start_header(start_header_t* struct_start_header, length_header_t* struct_length_header, char* null_string, int fd) {
    write_category_header(struct_start_header->name , null_string, struct_length_header->length_array[0], fd);
    write_category_header(struct_start_header->mode , null_string, struct_length_header->length_array[1], fd);
    write_category_header(struct_start_header->uid , null_string, struct_length_header->length_array[2], fd);
    write_category_header(struct_start_header->gid , null_string, struct_length_header->length_array[3], fd);
    write_category_header(struct_start_header->size , null_string, struct_length_header->length_array[4], fd);
    write_category_header(struct_start_header->mtime , null_string, struct_length_header->length_array[5], fd);
    write_category_header(struct_start_header->chksum , null_string, struct_length_header->length_array[6], fd);
    write(fd, struct_start_header->typeflag, 1);
    write_category_header(struct_start_header->linkname , null_string, struct_length_header->length_array[8], fd);
    write_category_header(struct_start_header->magic , null_string, struct_length_header->length_array[9], fd);
    write_category_header(struct_start_header->version , null_string, struct_length_header->length_array[10], fd);
    write_category_header(struct_start_header->uname , null_string, struct_length_header->length_array[11], fd);
    write_category_header(struct_start_header->gname , null_string, struct_length_header->length_array[12], fd);
    write_category_header(struct_start_header->devmajor , null_string, struct_length_header->length_array[13], fd);
    write_category_header(struct_start_header->devminor , null_string, struct_length_header->length_array[14], fd);
    write_category_header(struct_start_header->prefix , null_string, struct_length_header->length_array[15], fd);
    write_category_header(struct_start_header->atime , null_string, struct_length_header->length_array[16], fd);
    write_category_header(struct_start_header->ctime , null_string, struct_length_header->length_array[17], fd);
    write(fd, null_string, 12);
}

int add_tar_content(length_header_t* struct_length_header, char* file_name, char* null_string, int fd) { // caracter null = 0
    start_header_t* struct_start_header = get_start_header(file_name);
    write_start_header(struct_start_header, struct_length_header, null_string, fd);
    free_start_header(struct_start_header);
    write(fd, "\n", 1);
    write(fd, null_string, 512);

    return 0;
}

int create_archive(flag_t* struct_flag, length_header_t* struct_length_header, char** argv) {
    if(struct_flag->length_file == 0){
        printf("my_tar: Cowardly refusing to create an empty archive\n");
        return -1;
    }

    char null_string[512] = {0};
    int status = 0;
    DIR* directory;
    directory = opendir(argv[struct_flag->index_tarball]);
    if(directory != NULL){
        printf("my_tar: %s: Cannot open: Is a directory\n", argv[struct_flag->index_tarball]);
        closedir(directory);
        return -2;
    }
    if(check_if_file_exist(argv[struct_flag->index_tarball]) == 0){
        status = create_file(argv[struct_flag->index_tarball]); // pas oublier ajouté dernière ligne qui correspond à ?
        if(status < 0) {
            printf("my_tar: the program encountered an error\n");
            return status;
        }
    }

    int index = 0;
    int fichier = open(argv[struct_flag->index_tarball], O_WRONLY);
    while(index < struct_flag->length_file) {
        status = add_tar_content(struct_length_header, argv[struct_flag->array_file[index]], null_string, fichier); // à la fin
        if(status < 0) {
            printf("my_tar: the program encountered an error\n");
            close(fichier);
            return status;
        }
        index += 1;
    }
    write(fichier, null_string, 512);
    close(fichier);
    return status;
}