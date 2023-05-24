#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM        0
#define CMD_OR          1
#define CMD_AND         2
#define CMD_CHAIN       3

/* for convert_number() */
#define CONVERT_LOWERCASE       1
#define CONVERT_UNSIGNED        2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE       ".simple_shell_history"
#define HIST_MAX        4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
        int num;
        char *str;
        struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *              allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
*@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
        char *arg;
        char **argv;
        char *path;
        int argc;
        unsigned int line_count;
        int err_num;
        int linecount_flag;
        char *fname;
        list_t *env;
        list_t *history;
        list_t *alias;
        char **environ;
        int env_changed;
        int status;

        char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
        int cmd_buf_type; /* CMD_type ||, &&, ; */
        int readfd;
        int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
        0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
        char *type;
        int (*func)(info_t *);
} builtin_table;


/* toem_Shloop.c */
int HSH(info_t *, char **);
int Find_Builtin(info_t *);
void Find_CMD(info_t *);
void Fork_CMD(info_t *);

/* toem_Parser.c */
int Is_CMD(info_t *, char *);
char *Dup_Chars(char *, int, int);
char *Find_Path(info_t *, char *, char *);

/* Loophsh.c */
int Loophsh(char **);

/* toem_Errors.c */
void _Eputs(char *);
int _EputChar(char);
int _PutFd(char c, int fd);
int _PutsFd(char *str, int fd);

/* toem_String.c */
int _StrLength(char *);
int _StrCmp(char *, char *);
char *Starts_With(const char *, const char *);
char *_StrCat(char *, char *);

/* toem_String1.c */
char *_StrCpy(char *, char *);
char *_StrdUp(const char *);
void _Puts(char *);
int _PutChar(char);

/* toem_Exits.c */
char *_StrnCpy(char *, char *, int);
char *_StrnCat(char *, char *, int);
char *_StrChr(char *, char);

/* toem_Tokenizer.c */
char **Strtow(char *, char *);
char **Strtow2(char *, char);

/* toem_Realloc.c */
char *_MemSet(char *, char, unsigned int);
void FFree(char **);
void *_Realloc(void *, unsigned int, unsigned int);

/* toem_Memory.c */
int Bfree(void **);

/* toem_Atoi.c */
int Interactive(info_t *);
int Is_Delim(char, char *);
int _IsAlpha(int);
int _Atoi(char *);

/* toem_Errors1.c */
int _ErrAtoi(char *);
void Print_Error(info_t *, char *);
int Print_d(int, int);
char *Convert_Number(long int, int, int);
void Remove_Comments(char *);

/* toem_BuiltIn.c */
int _MyExit(info_t *);
int _MyCD(info_t *);
int _MyHelp(info_t *);

/* toem_BuiltIn1.c */
int _MyHistory(info_t *);
int _MyAlias(info_t *);

/*toem_GetLine.c */
ssize_t Get_Input(info_t *);
int _GetLine(info_t *, char **, size_t *);
void SigintHandler(int);

/* toem_GetInfo.c */
void Clear_Info(info_t *);
void Set_Info(info_t *, char **);
void Free_Info(info_t *, int);

/* toem_Environ.c */
char *_GetEnv(info_t *, const char *);
int _MyEnv(info_t *);
int _MySetEnv(info_t *);
int _MyUnsetEnv(info_t *);
int Populate_Env_List(info_t *);

/* toem_GetEnv.c */
char **Get_Environ(info_t *);
int _UnSetEnv(info_t *, char *);
int _SetEenv(info_t *, char *, char *);

/* toem_History.c */
char *Get_History_file(info_t *info);
int Write_History(info_t *info);
int Read_History(info_t *info);
int Build_History_list(info_t *info, char *buf, int linecount);
int Renumber_History(info_t *info);

/* toem_Lists.c */
list_t *Add_Node(list_t **, const char *, int);
list_t *Add_Node_End(list_t **, const char *, int);
size_t Print_List_Str(const list_t *);
int Delete_Node_At_Index(list_t **, unsigned int);
void Free_List(list_t **);

/* toem_Lists1.c */
size_t List_Len(const list_t *);
char **List_To_Strings(list_t *);
size_t Print_List(const list_t *);
list_t *Node_Starts_With(list_t *, char *, char);
ssize_t Get_Node_Index(list_t *, list_t *);

/* toem_Vars.c */
int Is_Chain(info_t *, char *, size_t *);
void Check_Chain(info_t *, char *, size_t *, size_t, size_t);
int Replace_Alias(info_t *);
int Replace_Vars(info_t *);
int Replace_String(char **, char *);

#endif
