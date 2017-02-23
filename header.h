

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define FALSE 0
#define TRUE 1
#define NOTFOUND  -1
#define COLON  ':'
#define MAX    256

#define TABLESIZE   0x10000
#define TABLEMASK   0x0ffff
#define K     1024
#define FNAME ".a1b2c3d4"
#define HASHV  50

typedef struct cmdtable
{
  char *cmd;
  char *path;
} CMDTABLE;

typedef struct redir
{
  char *type;
  char *arg;
} REDIR;

CMDTABLE ht[TABLESIZE];
int hash (char s[]);
void makehashtab (char *fname);
void lookUpAndAdd (char *s, char *path);
int lookupcmd (char *cmd);
int process_set (REDIR * p);
void printargs (char **toks, int len);
void execmd (char *cmd, char **args, char **envpi, REDIR * redirs);
void findAddcmds ();
int checkcmdtype (REDIR * cmdline_parsed);
char *extractcmd (char *token);
int redirectcmd (char *type, char *filename);
char **getpathlist ();
REDIR *getredirect (char *args);

char **pathlist;
char *cmdline, *broken, *cmd;
char **tokens;
DIR *dd;			/* directory descriptor */
struct dirent *dentry;
