#include "Shell.h"

/**
 * Get_Environ - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **Get_Environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = List_To_Strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _UnSetEnv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _UnSetEnv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = Starts_With(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = Delete_Node_At_Index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _SetEnv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _SetEnv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_StrLength(var) + _StrLength(value) + 2);
	if (!buf)
		return (1);
	_StrCpy(buf, var);
	_StrCat(buf, "=");
	_StrCat(buf, value);
	node = info->env;
	while (node)
	{
		p = Starts_With(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	Add_Node_End(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
