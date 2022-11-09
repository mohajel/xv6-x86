#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

int min(int a, int b){ return a < b ? a : b;}
int max(int a, int b){ return a > b ? a : b;}

int countDigits(int number){

    int count = 0;

    if(number < 0){
        number = - number;
        count++;
    }

    while(number > 0){
        count++;
        number = number / 10;
    }

    return count;
}

int power(int base, int power){
    int res = 1;

    while (power-- > 0)
    {
        res *= base;
    }

    return  res;
}

int abc(int number){ return number < 0 ? -number : number; }

int getDigit(int number, int digitIndex){
    return ((abc(number)/power(10, digitIndex))%10);
}

void writeNumber(char* dst, int* writeCurser, int dstCapacity, int src){

    int srcDigits = countDigits(src);

    if(src < 0 && *writeCurser < dstCapacity - 1){
        src = -src;
        dst[(*writeCurser)++] = '-';
        srcDigits--;
    }

    while(srcDigits-- > 0 && *writeCurser < dstCapacity - 1){
        dst[(*writeCurser)++] = getDigit(src,srcDigits) + '0';
    }
}


//will put arg inside string and add a \n to end
void cookString(int bufferSize, char* buffer, char* source, int* args, int argCount){

    const int STATE_PERCENT_SEEN = 0;
    const int STATE_D_SEEN = 1;
    const int STATE_NORMAL = 2;

    int len = strlen(source);

    int bufferIndex = 0;
    int argIndex = 0;

    int state = STATE_NORMAL;

    for(int i = 0; i < len && bufferIndex < bufferSize; i++){

        char currentChar = source[i];

        switch(state){
            case STATE_NORMAL:
                if(currentChar == '%'){
                    state = STATE_PERCENT_SEEN;
                }else{
                    buffer[bufferIndex++] = currentChar;
                }
                break;
            case STATE_PERCENT_SEEN:
                if(currentChar == 'd'){
                    state = STATE_D_SEEN;

                    int goalNumber = args[(argIndex++)%argCount];
                    writeNumber(buffer,&bufferIndex,bufferSize, goalNumber);

                }else{
                    buffer[bufferIndex++] = '%';
                    buffer[bufferIndex++] = currentChar;
                    state = STATE_NORMAL;
                }
                break;
            case STATE_D_SEEN:
                buffer[bufferIndex++] = currentChar;
                state = STATE_NORMAL;
                break;
        }
    }

    int newLineIndex, nullTerminatorIndex;

    if(bufferIndex  >= bufferSize - 1){
      nullTerminatorIndex = bufferSize - 1;
      newLineIndex = nullTerminatorIndex - 1;
      bufferIndex = bufferSize;
    }else{
      newLineIndex = bufferIndex++;
      nullTerminatorIndex = bufferIndex++;
    }

    buffer[newLineIndex] = '\n';
    buffer[nullTerminatorIndex] = '\0';
}

int print(char* message, int arg1, int arg2, int arg3){
    char buffer[350] = {0};

    int args[3] = {arg1, arg2, arg3};
    cookString(350, buffer, message, args, 3);

    return write(0,buffer, strlen(buffer));
}

int printSingleArg(char* message, int arg){
  return print(message, arg, NOTHING, NOTHING);
}

int printArgLess(char* message){
  return print(message, NOTHING, NOTHING, NOTHING);
}