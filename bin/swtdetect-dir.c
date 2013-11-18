#include "ccv.h"
#include <sys/time.h>
#include <ctype.h>
#include <stdio.h>
#include <dirent.h>

unsigned int get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void print_words(const char* prefix, ccv_array_t* words)
{
    if (words) {
        int numwords = words->rnum;
        int i;

        for (i = 0; i < numwords; i++) {
            ccv_rect_t* rect = (ccv_rect_t*) ccv_array_get(words, i);
            printf("%s %d %d %d %d\n", prefix, rect->x, rect->y, rect->width, rect->height);
        }
    }
}

void print_usage(const char* name) {
    printf("Usage:\n");
    printf("\t%s <dirname>...\n", name);
}

/*int process_file(const char* fname) {*/
int process_file(const char* fname) {

    ccv_dense_matrix_t* image = 0;
    ccv_read(fname, &image, CCV_IO_GRAY | CCV_IO_ANY_FILE);

    if (image == NULL) {
        return 1;
    }

    ccv_array_t* words = ccv_swt_detect_words(image, ccv_swt_default_params);
    if (words != NULL) {
        print_words(fname, words);
        ccv_array_free(words);
    }

    ccv_matrix_free(image);

    return 0;
}

#define MAX_PATH_LEN 2048
int process_directory(const char* dname) {
    DIR* dir = opendir(dname);

    if (dir == NULL) {
        return 1;
    }

    struct dirent *dentry;
    char fname[MAX_PATH_LEN];

    while ((dentry = readdir(dir)) != NULL) {
        if (dentry->d_name[0] == '.') continue;

        snprintf(fname, MAX_PATH_LEN, "%s/%s", dname, dentry->d_name);

        if (dentry->d_type & DT_REG) {
            process_file(fname);

        } else if (dentry->d_type & DT_DIR) {
            process_directory(fname);
        }
    }

    closedir(dir);

    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }

    ccv_enable_default_cache();

    int i;
    for (i = 1; i < argc; i++) {
        process_directory(argv[i]);
    }

    ccv_drain_cache();

    return 0;
}

