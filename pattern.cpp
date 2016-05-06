#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define READ 0
#define WRITE 1
#define RANDOM 2
#define SEQUENTIAL 3

#define KB 1024
#define MB 1024*1024

void access_file(const char *filepath, int chunk_size, int file_size, int rw, 
        int pattern)
{
    int fd;
    int n_chunks;
    int i;
    char *buf;
    int offset;

    fd = open(filepath, O_RDWR|O_CREAT, 00666);
    buf = (char *) malloc(chunk_size);
    n_chunks = file_size / chunk_size;

    for (i = 0; i < n_chunks; i++) {
        if (pattern == RANDOM) {
            offset = (rand() % n_chunks) * chunk_size;
        } else if (pattern == SEQUENTIAL) {
            offset = i * chunk_size;
        } else {
            printf("ERROR");
            exit(1);
        }

        if (rw == WRITE) {
            pwrite(fd, buf, chunk_size, offset);
            printf("pwrite %d %d\n", chunk_size, offset);
        } else if (rw == READ) {
            pread(fd, buf, chunk_size, offset);
            printf("pread %d %d\n", chunk_size, offset);
        } else {
            printf("ERROR");
            exit(1);
        }
    }
    close(fd);
}


int main(int argc, char **argv)
{
    struct timeval start, end, result;

    gettimeofday(&start, NULL);

    access_file("bar2", 2*KB, 2*KB, WRITE, SEQUENTIAL);

    gettimeofday(&end, NULL);

    timersub(&end, &start, &result);

    printf("--- Performance ---\n");
    printf("duration %ld.%ld\n", result.tv_sec, result.tv_usec);
    printf("start    %ld.%ld\n", start.tv_sec, start.tv_usec);
    printf("end      %ld.%ld\n", end.tv_sec, end.tv_usec);
}

