#ifndef NOTHING
#define NOTHING -1
#endif

struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
void get_callers(int syscall_number);
int get_parent_pid(void);

int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int find_largest_prime_factor();
void change_file_size(const char *path, int length);



// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int min(int a, int b);
int max(int a, int b);
int countDigits(int number);
int power(int base, int power);
int abc(int number);
int getDigit(int number, int digitIndex);
void writeNumber(char* dst, int* writeCurser, int dstCapacity, int src);
void cookString(int bufferSize, char* buffer, char* source, int* args, int argCount);
int print(char* message, int arg1, int arg2, int arg3);
int printSingleArg(char* message, int arg);
int printArgLess(char* message);