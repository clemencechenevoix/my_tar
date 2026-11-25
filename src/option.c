// ----- include ----- //
#include "../include/my_tar.h"

// ----- function ----- //

// --- main var (flags) --- //

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