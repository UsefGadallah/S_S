#include "Shell.h"

/**
 * _MyEnv - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _MyEnv(info_t *info)
{
	Print_List_Str(info->env);
	return (0);
}

/**
 * _GetEnv - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_GetEnv(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = Starts_With(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _MySetEnv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _MySetEnv(info_t *info)
{
	if (info->argc != 3)
	{
		_Eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_SetEnv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _MyUnSetEnv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _MyUnSetEnv(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_Eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_UnSetEnv(info, info->argv[i]);

	return (0);
}

/**
 * Populate_Env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int Populate_Env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		Add_Node_End(&node, environ[i], 0);
	info->env = node;
	return (0);
}
