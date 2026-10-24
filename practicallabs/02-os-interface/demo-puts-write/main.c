#include <stddef.h>
<<<<<<< HEAD
=======
#include <stdint.h>
>>>>>>> 2525c43d70d42982a3c8f448ed537438526f5a8f

/* x86-64 syscall number for write (see /usr/include/asm/unistd_64.h). */
#define SYS_write	1

/* Standard file descriptor for stdout. */
#define STDOUT_FILENO	1

/*
 * Layer 1: the generic syscall wrapper.
 *
 * This one is given to you. On x86-64 Linux the syscall number goes in RAX,
 * the arguments in RDI, RSI, RDX, R10, R8, R9, and the `syscall` instruction
 * transfers control to the kernel, which returns its result in RAX.
 *
 * You will reuse this function, unchanged, in every exercise of this session.
 */
long my_syscall(long number, long arg1, long arg2, long arg3,
		long arg4, long arg5, long arg6)
{
	long ret;
	register long r10 asm("r10") = arg4;
	register long r8  asm("r8")  = arg5;
	register long r9  asm("r9")  = arg6;

	asm volatile (
		"syscall"
		: "=a" (ret)
		: "a" (number), "D" (arg1), "S" (arg2), "d" (arg3),
		  "r" (r10), "r" (r8), "r" (r9)
		: "rcx", "r11", "memory"
	);

	return ret;
}

/*
 * TODO 1: Layer 2 -- the write(2) wrapper.
 *
 *   ssize_t write(int fd, const void *buf, size_t count);
 *
 * Forward fd, buf and count to my_syscall() as the first three arguments,
 * using SYS_write as the syscall number. The remaining argument slots are 0.
 * Remember that everything is passed as a machine word, so buf must be cast.
 */
long my_write(int fd, const char *buf, size_t count)
{
	/* TODO: replace this with a call to my_syscall(...). */
<<<<<<< HEAD
	return -1;
=======
	//return -1;
	return my_syscall(SYS_write, fd, (long)buf, count, 0, 0, 0);
>>>>>>> 2525c43d70d42982a3c8f448ed537438526f5a8f
}

/*
 * TODO 2: Layer 3 -- a puts()-like helper.
 *
 *   int puts(const char *s);
 *
 * Steps:
 *   1. Measure the length of s by hand. You may NOT call strlen() -- there
 *      is no libc below this point.
 *   2. Write the string to STDOUT_FILENO using my_write().
 *   3. Write the trailing newline that puts() adds.
 *   4. Return 0.
 */
int my_puts(const char *s)
{
	/* TODO: implement using my_write(). */
<<<<<<< HEAD
=======
	uint32_t len = 0;
	while(*(s + len) != '\0'){
		len++;
	}

	char *buf = alloca(len + 1);

	for(unsigned i = 0; i < len; ++i){
		buf[i] = s[i];
	}
	buf[len] = '\n';
	buf[len + 1] = '\0';

	my_write(STDOUT_FILENO, buf, len + 1);

	//my_write(STDOUT_FILENO, s, len);
	//my_write(STDOUT_FILENO, "\n", 1);
>>>>>>> 2525c43d70d42982a3c8f448ed537438526f5a8f
	return -1;
}

int main(void)
{
	my_puts("Hello from a raw write(2) system call!");
	my_puts("No printf, no libc puts -- just my_syscall().");

	return 0;
}
