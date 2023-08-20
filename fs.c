#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#inculde <stlib.h>

//path -> path of file / directory for which attributes have been requested
//st -> stat structure that is to be filled when attirbutes get read

static int simple_getattr(const char* path, struct stat* st) {

    st->st_uid = getuid();
    st->st_gid = getgid();
    st->st_atime = time(NULL); // last access time = now
    st->st_mtime = time(NULL); // last modification time = now

    if(strcmp(path, "/") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    }

    else {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        st->st_size = 1024;
    }

    return 0;
}

static int simple_readdir(const char* path, void* buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi) {

    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);

    if(strcmp(path, "/") == 0) {
        filler( buffer, "file_x", NULL, 0 );
		filler( buffer, "file_y", NULL, 0 );
    }

    return 0;
}

//return number of bytes read
static int simple_read(const char* path, char* buffer, size_t size, off_t offset, struct fuse_file_info* fi) {

    //defining files here for sake of simplicity. Will be improved later

    char file_x_contents = "In the bleak midwinter";
    char file_y_contents = "Frosty wind made moan";

    char* req_text = NULL;

    if(strcmp(path, "/file_x") == 0) {
        req_text = file_x_contents;
    }

    else if(strcmp(path, "/file_y") == 0) {
        req_text = file_y_contents;
    }

    else return -1; //temporarily

    memcpy(buffer, req_text + offset, size);
    
    return strlen(buffer);
}

static struct fuse_operations ops = {
    .getattr = simple_getattr
    .readdir = simple_readdir,
    .read = simple_read
};