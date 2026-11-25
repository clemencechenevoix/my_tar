// ----- include ----- //
#include "../include/my_tar.h"

// ----- main ----- //

int main(int argc, char** argv) {
    flag_t* struct_flag = get_main_var(argv, argc);
    if (struct_flag->status < 0) {
        free_flag(struct_flag);
        return 0;
    }

    length_header_t* struct_length_header = init_length_header();

    free_flag(struct_flag);
    free_length_header(struct_length_header);
    return 0;
}