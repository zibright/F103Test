/**
  ******************************************************************************
  * @file    syscalls.c
  * @brief   Minimal newlib system call stubs.
  *
  *          _write() currently discards output. Hook it up to a UART if
  *          printf-style logging is needed.
  ******************************************************************************
  */

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

#undef errno
extern int errno;

/* Symbols provided by the linker script */
extern char _end;
extern char _estack;
extern uint32_t _Min_Stack_Size;

static char *heap_end;

/**
  * @brief  Extend the heap, keeping the configured minimum stack free.
  */
void *_sbrk(ptrdiff_t incr)
{
  char *prev_heap_end;

  if (heap_end == NULL)
  {
    heap_end = &_end;
  }

  prev_heap_end = heap_end;

  if ((uint32_t)heap_end + (uint32_t)incr >
      (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size)
  {
    errno = ENOMEM;
    return (void *)-1;
  }

  heap_end += incr;
  return (void *)prev_heap_end;
}

int _write(int file, char *ptr, int len)
{
  (void)file;
  (void)ptr;
  return len; /* Output discarded, override to redirect to a UART. */
}

int _read(int file, char *ptr, int len)
{
  (void)file;
  (void)ptr;
  (void)len;
  return 0;
}

int _close(int file)
{
  (void)file;
  return -1;
}

int _fstat(int file, struct stat *st)
{
  (void)file;
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  (void)file;
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  (void)file;
  (void)ptr;
  (void)dir;
  return 0;
}

int _kill(int pid, int sig)
{
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

void _exit(int status)
{
  (void)status;
  while (1)
  {
  }
}

int _getpid(void)
{
  return 1;
}
