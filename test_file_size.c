#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include <stdio.h>
#include <string.h>

#define NOTHING -1


void 
print(char* message, int first_arg, int second_arg, int third_arg);

void
create_file(char* path, int size); 

void
delete_file(char* path);

int 
get_file_size(char* path);

void 
test_change_file_size(char* path, int init_size, int change_amount);


main(int argc, char *argv[])
{
    print("starting test...",NOTHING,NOTHING,NOTHING);

    test_change_file_size("./temp_file_inc_size.txt",1024,+512);
    test_change_file_size("./temp_file_dec_size.txt",1024,-256);

    print("test finished",NOTHING,NOTHING,NOTHING);

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
    print("test finished init size : %d changed_size : %d success : %d", size_before, size_after, size_before == size_after);

    //delete file
    delete_file(path);
}


void 
print(char* message, int first_arg, int second_arg, int third_arg){

    char buffer[250] = {0};

    sprintf(buffer, message, first_arg, second_arg, third_arg);

    strcat(buffer, "\n");

    write(0,buffer, strlen(buffer));

}


void
create_file(char* path, int size){

    int fd = open(path, O_CREATE | O_RDWR);

    if(fd < 0) {
        print("Failed to create file", NOTHING, NOTHING, NOTHING);
        exit();
    }

    char buffer[2] = {0,0};

    for(int i = 0; i < size; i++){
        buffer[0] = '0'+(i%10),0;
        if(write(fd,buffer, 1) != 1){
            print("Failed to write to file", NOTHING, NOTHING, NOTHING);
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
        print("Failed to check file size", NOTHING, NOTHING, NOTHING);
        exit();
    }

    if(fstat(fd, &fileInfo) < 0){
        print("Failed to check file size", NOTHING, NOTHING, NOTHING);
        exit();
    }

    close(fd);

    return fileInfo.size;
}
