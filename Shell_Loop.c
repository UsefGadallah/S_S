#include "Shell.h"

/**
 * HSH - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int HSH(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		Clear_Info(info);
		if (Interactive(info))
			_Puts("$ ");
		_EputChar(BUF_FLUSH);
		r = Get_Input(info);
		if (r != -1)
		{
			Set_Info(info, av);
			builtin_ret = Find_BuiltIn(info);
			if (builtin_ret == -1)
				Find_CMD(info);
		}
		else if (Interactive(info))
			_PutChar('\n');
		Free_Info(info, 0);
	}
	Write_History(info);
	Free_Info(info, 1);
	if (!Interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * Find_BuiltIn - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int Find_BuiltIn(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _MyExit},
		{"env", _MyEnv},
		{"help", _MyHelp},
		{"history", _MyHistory},
		{"setenv", _MySetEnv},
		{"unsetenv", _MyUnsetEnv},
		{"cd", _MyCd},
		{"alias", _MyAlias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_StrCmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * Find_CMD - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void Find_CMD(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!Is_Delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = Find_Path(info, _GetEnv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		Fork_CMD(info);
	}
	else
	{
		if ((Interactive(info) || _GetEnv(info, "PATH=")
			|| info->argv[0][0] == '/') && Is_CMD(info, info->argv[0]))
			Fork_CMD(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			Print_Error(info, "not found\n");
		}
	}
}

/**
 * Fork_CMD - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void Fork_CMD(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, Get_Environ(info)) == -1)
		{
			Free_Info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				Print_Error(info, "Permission denied\n");
		}
	}
}
