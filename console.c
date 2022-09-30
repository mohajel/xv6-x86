// Console input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include <stdbool.h>


static void consputc(int);

static int panicked = 0;

static struct {
  struct spinlock lock;
  int locking;
} cons;

static void
printint(int xx, int base, int sign)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    consputc(buf[i]);
}
//PAGEBREAK: 50

// Print to the console. only understands %d, %x, %p, %s.
void
cprintf(char *fmt, ...)
{
  int i, c, locking;
  uint *argp;
  char *s;

  locking = cons.locking;
  if(locking)
    acquire(&cons.lock);

  if (fmt == 0)
    panic("null fmt");

  argp = (uint*)(void*)(&fmt + 1);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      consputc(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if(c == 0)
      break;
    switch(c){
    case 'd':
      printint(*argp++, 10, 1);
      break;
    case 'x':
    case 'p':
      printint(*argp++, 16, 0);
      break;
    case 's':
      if((s = (char*)*argp++) == 0)
        s = "(null)";
      for(; *s; s++)
        consputc(*s);
      break;
    case '%':
      consputc('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      consputc('%');
      consputc(c);
      break;
    }
  }

  if(locking)
    release(&cons.lock);
}

void
panic(char *s)
{
  int i;
  uint pcs[10];

  cli();
  cons.locking = 0;
  // use lapiccpunum so that we can call panic from mycpu()
  cprintf("lapicid %d: panic: ", lapicid());
  cprintf(s);
  cprintf("\n");
  getcallerpcs(&s, pcs);
  for(i=0; i<10; i++)
    cprintf(" %p", pcs[i]);
  panicked = 1; // freeze other CPU
  for(;;)
    ;
}

//PAGEBREAK: 50
#define BACKSPACE 0x100
#define CRTPORT 0x3d4
static ushort *crt = (ushort*)P2V(0xb8000);  // CGA memory

static void
cgaputc(int c)
{
  int pos;

  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT+1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT+1);

  if(c == '\n')
    pos += 80 - pos%80;
  else if(c == BACKSPACE){
    if(pos > 0) --pos;
  } else
    crt[pos++] = (c&0xff) | 0x0700;  // black on white

  if(pos < 0 || pos > 25*80)
    panic("pos under/overflow");

  if((pos/80) >= 24){  // Scroll up.
    memmove(crt, crt+80, sizeof(crt[0])*23*80);
    pos -= 80;
    memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
  }

  outb(CRTPORT, 14);
  outb(CRTPORT+1, pos>>8);
  outb(CRTPORT, 15);
  outb(CRTPORT+1, pos);
  crt[pos] = ' ' | 0x0700;
}

void
consputc(int c)
{
  if(panicked){
    cli();
    for(;;)
      ;
  }

  if(c == BACKSPACE){
    uartputc('\b'); uartputc(' '); uartputc('\b');
  } else
    uartputc(c);
  cgaputc(c);
}

#define _DEBUG_MODE_ 0
#define INPUT_BUF 128
#define SAFE_ZONE 130 //for '\0'
#define AUTO_COMPLEATION_SIZE 15
#define NO_SUGGESTION -1
#define C(x)  ((x)-'@')  // Control-x

struct 
{
  char buf[INPUT_BUF];
  uint r;  // Read index
  uint w;  // Write index
  uint e;  // Edit index
} input;

static int index = -1;
static char auto_complete_table[AUTO_COMPLEATION_SIZE][SAFE_ZONE];

int get_size(char* string)
{
  for (int i = 0; i < SAFE_ZONE; ++i)
    if (string[i] == '\0')
      return i;
  return 0;
}

 bool str_compare(char* str1, char* str2)
 {
  int size;
  if ((size = get_size(str1)) != get_size(str2))
    return false;

  for (int i = 0; i < size; ++i)
    if (str1[i] != str2[i])
      return false;
  return true;
 }

//prints with changing buffer
void cout(char* str)
{
  for (int i = 0; i < get_size(str); ++i)
  {
    consputc(str[i]);
    input.buf[input.e % INPUT_BUF] = str[i];
    input.e ++;
  }
}

//prints without changing buffer
void print_consoul(char* str)
{
  for (int i = 0; i < get_size(str); ++i)
    consputc(str[i]);
}

// must check size befor using this func
void str_copy(char* from_string, char* to_string)
{
  int size = get_size(from_string);

  for (int i = 0; i < size; ++i)
    to_string[i] = from_string[i];

  to_string[size] = '\0';
}

void add_to_auto_complete_table(char* last_line)
{
  index ++;
  if (index == AUTO_COMPLEATION_SIZE)
    index = 0;

  str_copy(last_line, auto_complete_table[index]);

  if (_DEBUG_MODE_)
  {
    print_consoul("\nadded to auto complete:");
    print_consoul(auto_complete_table[index]);
    print_consoul("\n to index :");
    char ind = '0' + index;
    consputc(ind);
  }
}

//FUNC
void 
get_last_line(char* last_line)
{
  int start = input.w % INPUT_BUF;
  int end = input.e % INPUT_BUF;
  int index = 0;

  while(1)
  {
    if (start >= INPUT_BUF)
      start -= INPUT_BUF;

    last_line[index] = input.buf[start];

    if (start == end)
      break;

    index ++;
    start ++;
  }
  last_line[index] = '\0';
}

//FUNC
void get_reverse_of_last_line(char* last_line, char* reverse)
{
  int size = get_size(last_line);
  int index = 0;

  for (int i = size - 1; i >= 0; --i)
  {
    reverse[index] = last_line[i];
    index ++;
  }
  reverse[index] = '\0';
}

bool has_same_beginig(char* last_line, char* suggestion)
{
  int size;
  if ((size = get_size(last_line)) >= get_size(suggestion))
    return false;

  for (int i = 0; i < size; ++i)
    if (last_line[i] != suggestion[i])
      return false;
  return true;
}

//FUNC
int get_seggestion(char* last_line)
{
  if (get_size(last_line) == 0)
    return NO_SUGGESTION;//NO_SUGGESTION for empty consoul!

  int priority = index;
  do
  {
    if (priority == -1)
      priority = AUTO_COMPLEATION_SIZE - 1;
    if (has_same_beginig(last_line, auto_complete_table[priority]))
      return priority;
    priority --;
  } while (priority != index);

  return NO_SUGGESTION;
}

//FUNC
void clear_line(int line_size)
{
  for (int i = 0; i < line_size; ++i)
    consputc(BACKSPACE);
  input.e -= line_size;
}

bool is_num(char ch)
{
  if (ch >= '0' && ch <='9')
    return true;
  return false;
}

//FUNC
void filter_numbers(char* line, char* filterd)
{
  int size = get_size(line);
  int index = 0;

  for (int i = 0; i < size; ++i)
    if (!is_num(line[i]))
    {
      filterd[index] = line[i];
      index ++;
    }
  filterd[index] = '\0';
}

void consoleintr(int (*getc)(void))
{
  char last_line[SAFE_ZONE];
  get_last_line(last_line);

  if (_DEBUG_MODE_)
  {
    cprintf("\t -- begenning of consoleintr func -- \n \t -- buffer ::");
    for (int i = 0; i < 8; ++i)
      consputc(input.buf[i]);
    cprintf("\n\t -- last_line : %s -- \n", last_line);
    cprintf("\n\t -- Read : %d -- \n", input.r);
    cprintf("\t -- Write : %d -- \n", input.w);
    cprintf("\t -- Edit : %d -- \n", input.e);
  }

  int c, doprocdump = 0;

  acquire(&cons.lock);
  while((c = getc()) >= 0)
  {
    if (_DEBUG_MODE_)
      consputc(c);

    switch(c)
    {
      case C('N'): // Delete numbers
        clear_line(get_size(last_line));
        char line_without_num[SAFE_ZONE];
        filter_numbers(last_line, line_without_num);
        cout(line_without_num);
      break;

      case C('R'): // Reverse line
        clear_line(get_size(last_line));
        char reverse_last_line[SAFE_ZONE];
        get_reverse_of_last_line(last_line, reverse_last_line);
        cout(reverse_last_line);
      break;

      case C('P'):  // Process listing.
        // procdump() locks cons.lock indirectly; invoke later
        doprocdump = 1;
      break;

      case C('U'):  // Kill line.
        clear_line(get_size(last_line)); //works perfectly fine
      break;

      case C('H'): case '\x7f':  // Backspace
        if(input.e != input.w){
          input.e--;
          consputc(BACKSPACE);
        }
      break;

      case ('\t'): //Tab autocompletion
        int suggestion = get_seggestion(last_line);
        if (suggestion != NO_SUGGESTION)
        {
          clear_line(get_size(last_line));
          cout(auto_complete_table[suggestion]);
        }
      break;

      case ('\r'): //Enter
        if (get_size(last_line) > 1 && 
            str_compare(last_line, auto_complete_table[index]) == false)
          add_to_auto_complete_table(last_line);
      default:
        if(c != 0 && input.e-input.r < INPUT_BUF){
          c = (c == '\r') ? '\n' : c;
          input.buf[input.e++ % INPUT_BUF] = c;
          consputc(c);
          if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
            input.w = input.e;
            wakeup(&input.r);
          }
        }
      break;
      }
  }
  release(&cons.lock);
  if(doprocdump) {
    procdump();  // now call procdump() wo. cons.lock held
  }
  if (_DEBUG_MODE_)
    cprintf("\t -- end of consoleintr func -- \n\n\n");
}

int
consoleread(struct inode *ip, char *dst, int n)
{
  uint target;
  int c;

  iunlock(ip);
  target = n;
  acquire(&cons.lock);
  while(n > 0){
    while(input.r == input.w){
      if(myproc()->killed){
        release(&cons.lock);
        ilock(ip);
        return -1;
      }
      sleep(&input.r, &cons.lock);
    }
    c = input.buf[input.r++ % INPUT_BUF];
    if(c == C('D')){  // EOF
      if(n < target){
        // Save ^D for next time, to make sure
        // caller gets a 0-byte result.
        input.r--;
      }
      break;
    }
    *dst++ = c;
    --n;
    if(c == '\n')
      break;
  }
  release(&cons.lock);
  ilock(ip);

  return target - n;
}

int
consolewrite(struct inode *ip, char *buf, int n)
{
  int i;

  iunlock(ip);
  acquire(&cons.lock);
  for(i = 0; i < n; i++)
    consputc(buf[i] & 0xff);
  release(&cons.lock);
  ilock(ip);

  return n;
}

void
consoleinit(void)
{
  initlock(&cons.lock, "console");

  devsw[CONSOLE].write = consolewrite;
  devsw[CONSOLE].read = consoleread;
  cons.locking = 1;

  ioapicenable(IRQ_KBD, 0);
}

