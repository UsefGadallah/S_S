#include "Shell.h"

/**
 * Clear_Info - initializes info_t struct
 * @info: struct address
 */
void Clear_Info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * Set_Info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void Set_Info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = Strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _StrDup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		Replace_Alias(info);
		Replace_Vars(info);
	}
}

/**
 * Free_Info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void Free_Info(info_t *info, int all)
{
	FFree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			Free_List(&(info->env));
		if (info->history)
			Free_List(&(info->history));
		if (info->alias)
			Free_List(&(info->alias));
		FFree(info->environ);
			info->environ = NULL;
		Bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_PutChar(BUF_FLUSH);
	}
}
