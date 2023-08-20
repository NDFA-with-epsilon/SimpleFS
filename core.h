#ifndef _FS_DATA_STRUCT_
#define _FS_DATA_STRUCT_

#include <stddef.h>
#include <string.h>

char dir_list[256][256];
char file_list[256][256];

int current_dir_idx = -1;
int current_file_idx = -1;

char files_content[256][256];
int current_file_content_idx = -1;

int insert_directory(const char* dir_name) {
    if(dir_name == NULL) return -1;

    current_dir_idx++;
    strcpy(dir_list[current_dir_idx], dir_name);
    
    return 0;
}

int directory_exists(const char* path) {
    if(path == NULL || strlen(path) > 256) return -1;
    path++; //eliminate "/" -> only root

    for(int idx = 0; idx <= current_dir_idx; idx++) {
        if(strcmp(path, dir_list[idx]) == 0) return 0;
    }

    return -1;
}

int insert_file(const char* file_name) {
    if(file_name == NULL || strlen(file_name) > 256) return -1;

    current_file_idx++;

    strcpy(file_list[current_file_idx], file_name); 
    strcpy(files_content[current_file_idx], "");

    return 0;
}

int file_exists(const char* path) {
    if(path == NULL || strlen(path) > 256) return -1;

    path++;
    for(int idx = 0; idx < current_file_idx; idx++) {
        if(strcmp(path, file_list[idx]) == 0) return 0;
    }

    return -1;
}

int get_file_idx(const char* path) {
    if(path == NULL || strlen(path) > 256) return -1;
    
    path++;
    for(int idx = 0; idx < current_file_idx; idx++) {
        if(strcmp(path, file_list[idx]) == 0) return idx;
    }

    return -1;
}

int write_to_file(const char* path, const char* content) {
    if(path == NULL || strlen(path) > 256 || strlen(content) > 256) return -1;

    int file_idx = get_file_idx(path);
    if(file_idx == -1) return -1;

    strcpy(files_content[file_idx], content);
    return strlen(files_content[file_idx]);
}


#endif