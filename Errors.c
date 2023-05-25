#include "Shell.h"

/**
 *_Eputs - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _Eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_EputChar(str[i]);
		i++;
	}
}

/**
 * _EputChar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _EputChar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * _PutFd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _PutFd(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 *_PutsFd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _PutsFd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _PutFd(*str++, fd);
	}
	return (i);
}
