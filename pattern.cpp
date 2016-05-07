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
#define DOFSYNC 4
#define DONOTFSYNC 5

#define KB 1024
#define MB 1024*1024

typedef int t_pat;
typedef int t_rw;
typedef bool t_fsync;

void access_file(const char *filepath, int chunk_size, int file_size, t_rw rw, 
        t_pat pattern, t_fsync dofsync)
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
            // printf("pwrite %d %d\n", chunk_size, offset);
            if (dofsync) {
                fsync(fd);
            }
        } else if (rw == READ) {
            pread(fd, buf, chunk_size, offset);
            // printf("pread %d %d\n", chunk_size, offset);
        } else {
            printf("ERROR");
            exit(1);
        }
    }
    close(fd);
}

std::string exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

void drop_caches() 
{
    exec("sync");
    exec("echo 3 > /proc/sys/vm/drop_caches");
}

void check_arguments(int argc, char **argv)
{    
    if (argc != 2) {
        printf("Usage: %s target-folder\n", argv[0]);
        printf("Example: %s /tmp\n", argv[0]);
        exit(1);
    }
}

class Parameter
{
    public:
        std::string target_folder;
        std::string exp_name;
        std::string file_name;
        t_pat pattern;
        int chunk_size;
        int file_size;
        t_pat rw;
        t_fsync dofsync;

    Parameter(std::string my_target_folder, std::string my_exp_name, 
            std::string my_file_name, t_pat my_pattern, 
            int my_chunk_size, int my_file_size, t_rw my_rw, 
            t_fsync my_dofsync):
        target_folder(my_target_folder),
        exp_name(my_exp_name),
        file_name(my_file_name),
        pattern(my_pattern),
        chunk_size(my_chunk_size),
        file_size(my_file_size),
        rw(my_rw),
        dofsync(my_dofsync)
    {}
};


class Experiment
{
    public:
        Parameter _para;

        Experiment(Parameter);
        void run();
};

Experiment::Experiment(Parameter my_para):
    _para(my_para)
{
}

void Experiment::run()
{
    struct timeval start, end, result;
    std::string filepath;

    filepath = _para.target_folder + "/" + _para.file_name;

    gettimeofday(&start, NULL);

    access_file(filepath.c_str(), _para.chunk_size, _para.file_size, 
            _para.rw, _para.pattern, _para.dofsync);

    gettimeofday(&end, NULL);

    timersub(&end, &start, &result);

    printf("cpp %s %ld.%ld\n", _para.exp_name.c_str(), result.tv_sec, result.tv_usec);

}


int main(int argc, char **argv)
{
    int i;
    int n;

    check_arguments(argc, argv);

    std::string target_folder = std::string(argv[1]);

    Parameter exps[] = {
        Parameter(target_folder, "SeqSmallWriteNoFsync", "cppdata", SEQUENTIAL, 4*KB, 128*MB, WRITE, false),
        Parameter(target_folder, "RandSmallWriteFsync", "cppdata", RANDOM, 4*KB, 128*MB, WRITE, true),
        Parameter(target_folder, "SeqSmallWriteFsync", "cppdata", SEQUENTIAL, 4*KB, 128*MB, WRITE, true),
        Parameter(target_folder, "SeqSmallRead", "cppdata", SEQUENTIAL, 4*KB, 128*MB, READ, false),
        Parameter(target_folder, "SeqBigRead", "cppdata", SEQUENTIAL, 128*MB, 128*MB, READ, false),
        Parameter(target_folder, "RandSmallRead", "cppdata", RANDOM, 4*KB, 128*MB, READ, false)
        };


    n = sizeof(exps) / sizeof(Parameter);
    for (i = 0; i < n; i++) {
        Experiment exp = Experiment(exps[i]);
        sleep(1);
        drop_caches();
        exp.run();
    }

    return 0;
}

