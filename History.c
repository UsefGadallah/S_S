#include "Shell.h"

/**
 * Get_History_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *Get_History_file(info_t *info)
{
	char *buf, *dir;

	dir = _GetEnv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_StrLength(dir) + _StrLength(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_StrCpy(buf, dir);
	_StrCat(buf, "/");
	_StrCat(buf, HIST_FILE);
	return (buf);
}

/**
 * Write_History - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int Write_History(info_t *info)
{
	ssize_t fd;
	char *filename = Get_History_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_PutsFd(node->str, fd);
		_PutFd('\n', fd);
	}
	_PutFd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * Read_History - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int Read_History(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = Get_History_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			Build_History_List(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		Build_History_List(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		Delete_Node_At_Index(&(info->history), 0);
	Renumber_History(info);
	return (info->histcount);
}

/**
 * Build_History_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int Build_History_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	Add_Node_End(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * Renumber_History - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int Renumber_History(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
