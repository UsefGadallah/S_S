#include "Shell.h"

/**
 * List_Len - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t List_Len(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * List_To_Strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **List_To_Strings(list_t *head)
{
	list_t *node = head;
	size_t i = List_Len(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_StrLength(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _StrCpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * Print_List - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t Print_List(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_Puts(Convert_Number(h->num, 10, 0));
		_PutChar(':');
		_PutChar(' ');
		_Puts(h->str ? h->str : "(nil)");
		_Puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * Node_Starts_With - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *Node_Starts_With(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = Starts_With(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * Get_Node_Index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t Get_Node_Index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
