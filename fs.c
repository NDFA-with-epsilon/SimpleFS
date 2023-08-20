#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

#include "core.h"

#inculde <stlib.h>

//path -> path of file / directory for which attributes have been requested
//st -> stat structure that is to be filled when attirbutes get read

static int simple_getattr(const char* path, struct stat* st) {

    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL); // last access time = now
    st->st_mtime = time(NULL); // last modification time = now

    if(strcmp(path, "/") == 0 || directory_exists(path) == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    }

    else if (file_exists(path) == 0) {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    }

    else return -ENOENT;
    return 0;
}

static int simple_readdir(const char* path, void* buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi) {

    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);

    if(strcmp(path, "/") == 0) {
        for(int idx = 0; idx < current_dir_idx; idx++) {
            filler(buffer, dir_list[idx], NULL, 0);
        }

        for(int idx = 0; idx < current_file_idx; idx++) {
            filler(buffer, file_list[idx], NULL, 0);
        }
    }

    return 0;
}

//return number of bytes read
static int simple_read(const char* path, char* buffer, size_t size, off_t offset, struct fuse_file_info* fi) {

    //defining files here for sake of simplicity. Will be improved later

    // char file_x_contents = "In the bleak midwinter";
    // char file_y_contents = "Frosty wind made moan";

    // char* req_text = NULL;

    // if(strcmp(path, "/file_x") == 0) {
    //     req_text = file_x_contents;
    // }

    // else if(strcmp(path, "/file_y") == 0) {
    //     req_text = file_y_contents;
    // }

    // else return -1; //temporarily


    int file_idx = get_file_idx(path);
    if(file_idx == -1) return -1;

    char* content = files_content[file_idx];

    memcpy(buffer, content + offset, size);
    
    return strlen(buffer);
}

static int simple_mkdir(const char* path, mode_t mode) {
    if(path == NULL || strlen(path) > 256) return -1;

    path++;
    insert_directory(path);

    return 0;
}

static int simple_mknod(const char* path, mode_t mode, dev_t rdev) {
    if(path == NULL || strlen(path) > 256) return -1;

    path++;
    insert_file(path);

    return 0;
}

static int simple_write( const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info )
{
	return write_to_file( path, buffer );
}


static struct fuse_operations ops = {
    .getattr = simple_getattr
    .readdir = simple_readdir,
    .read = simple_read,
    .mkdir = simple_mkdir,
    .mknod = simple_mknod,
    .write = simple_write,
};