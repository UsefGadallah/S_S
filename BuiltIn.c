#include "Shell.h"

/**
 * _MyExit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int _MyExit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = _Erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			Print_Error(info, "Illegal number: ");
			_Eputs(info->argv[1]);
			_EputChar('\n');
			return (1);
		}
		info->err_num = _Erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * _MyCd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _MyCd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_Puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _GetEnv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _GetEnv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_StrCmp(info->argv[1], "-") == 0)
	{
		if (!_GetEnv(info, "OLDPWD="))
		{
			_Puts(s);
			_PutChar('\n');
			return (1);
		}
		_Puts(_GetEnv(info, "OLDPWD=")), _PutChar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _GetEnv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		Print_Error(info, "can't cd to ");
		_Eputs(info->argv[1]), _EputChar('\n');
	}
	else
	{
		_SetEnv(info, "OLDPWD", _GetEnv(info, "PWD="));
		_SetEnv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _MyHelp - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _MyHelp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_Puts("help call works. Function not yet implemented \n");
	if (0)
		_Puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
