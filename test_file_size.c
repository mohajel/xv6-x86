#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void
create_file(char* path, int size); 

void
delete_file(char* path);

int 
get_file_size(char* path);

void 
test_change_file_size(char* path, int init_size, int change_amount);


int main(int argc, char *argv[])
{

    printArgLess("starting test...");

    test_change_file_size("./temp_file_inc_size.txt",1024,+512);
    test_change_file_size("./temp_file_dec_size.txt",1024,-256);

    printArgLess("test finished");

    exit();
}



void 
test_change_file_size(char* path, int init_size, int change_amount){

    //creating file
    create_file(path, init_size);

    //testing size
    int size_before = get_file_size(path);

    //sys call
    change_file_size(path, init_size + change_amount);

    //testing size
    int size_after = get_file_size(path);

    //report
    print("test finished init size : %d changed_size : %d success : %d", size_before, size_after, size_before + change_amount == size_after);

    //delete file
    delete_file(path);
}

void
create_file(char* path, int size){

    int fd = open(path, O_CREATE | O_RDWR);

    if(fd < 0) {
        printArgLess("Failed to create file");
        exit();
    }

    char buffer[2] = {0,0};

    for(int i = 0; i < size; i++){
        buffer[0] = '0'+(i%10);
        if(write(fd,buffer, 1) != 1){
            printArgLess("Failed to write to file");
            exit();
        }   
    }

    close(fd);
}

void
delete_file(char* path){
    unlink(path);
}


int 
get_file_size(char* path){

    struct stat fileInfo;

    int fd = open(path, O_RDONLY);

    if(fd < 0){
        printArgLess("Failed to check file size");
        exit();
    }

    if(fstat(fd, &fileInfo) < 0){
        printArgLess("Failed to check file size");
        exit();
    }

    close(fd);

    return fileInfo.size;
}
