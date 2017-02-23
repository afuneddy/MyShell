#include "header.h"

/*PURPOSE : EXTRACTS COMMAND NAME FROM PATH */
/* INPUT :  TAKES A COMMAND PATH, */
/* OUTPUT : RETURNS COMMAND NAME */

char *
extractcmd (char *token)
{
  char *cmd = malloc (sizeof (char) * K);
  int m = 0, i;

  for (i = 0; token[i]; i++)
    {
      if (token[i] == '/')
	{
	  m = 0;
	}
      else
	cmd[m++] = token[i];
    }
  cmd[m] = 0;
  return cmd;
}


/* PURPOSE: CHECKS AND EXECUTES BUILT-IN FUNCTIONS*/
/* INPUT:  RETURNS TRUE IF cd or FALSE IF NOT,THEN EXECUTES CMD*/
/* OUTPUT: INT, 0 FALSE, 1 TRUE */

int
checkcmdtype (REDIR * cmdline)
{
  int k;
  if (strcmp (cmdline[0].arg, "cd") == FALSE)
    {
      chdir (cmdline[1].arg);
      return TRUE;
    }
  else if (strcmp (cmdline[0].arg, "exit") == 0)
    {
      exit (0);
    }
  else if (strcmp (cmdline[0].arg, "logout") == 0)
    {
      exit (0);
    }
  return FALSE;
}


/* PURPOSE: PROCESS SET PATH */
/* INPUTS: PARSED COMMANDLINE STRINGS */
/* OUTPUT: RETURNS TRUE IF PATH IS SET ELSE FALSE */


int
process_set (REDIR * cmdline)
{
  int n, k, stop, i = 0;
  char *cmdpath;
  k = 0;
  if ((strcmp (cmdline[0].arg, "set") == 0)
      && (strcmp (cmdline[1].arg, "path") == 0) &&
      (strcmp (cmdline[2].type, "assign") == 0) &&
      (strcmp (cmdline[3].type, "lbrack") == 0) &&
      (strcmp (cmdline[4].type, "quote") == 0) &&
      (strcmp (cmdline[5].type, "word") == 0) &&
      (strcmp (cmdline[6].type, "quote") == 0))
    {
      k = 6;
      if ((strcmp (cmdline[k + 1].type, "rbrack") == 0)
	  && (strcmp (cmdline[k + 2].type, "col") == 0))
	stop = 1;
    }
  else
    {

      fprintf (stderr,
	       "Usage: set path=(\"path1\",\"path2\",...,\"pathn\");\n");
      return FALSE;
    }
  if (!stop)
    while (cmdline[k + 4].type)
      {
	if ((strcmp (cmdline[k + 1].type, "comma") == 0)
	    && (strcmp (cmdline[k + 2].type, "quote") == 0) &&
	    (strcmp (cmdline[k + 3].type, "word") == 0) &&
	    (strcmp (cmdline[k + 4].type, "quote") == 0))
	  k = k + 4;
	else
	  {
	    fprintf (stderr,
		     "Usage: set path=(\"path1\",\"path2\",...,\"pathn\");\n");
	    return FALSE;
	  }
	if (strcmp (cmdline[k + 1].type, "comma") == 0)
	  continue;
	else if ((strcmp (cmdline[k + 1].type, "rbrack") == 0)
		 && (strcmp (cmdline[k + 2].type, "col") == 0))
	  {
	    k = k + 2;
	  }
      }
  cmdpath = malloc (sizeof (char) * K);

  for (n = 5; cmdline[n].arg != NULL; n = n + 4)
    {
      dd = opendir (cmdline[n].arg);
      if (dd == NULL)
	continue;
      while (dentry = readdir (dd))
	{
	  sprintf (cmdpath, "%s/%s", cmdline[n].arg, dentry->d_name);
	  lookUpAndAdd (dentry->d_name, cmdpath);
	}

    }
  return TRUE;
}

/* PURPOSE: POPULATE PATH LIST WITH ALL COMMANDS FROM PATH*/
/*INPUT: VOID*/
/*OUTPUT: VOID*/

void
findAddcmds ()
{
  int n, i = 0;
  char *cmdpath;

  cmdpath = malloc (sizeof (char) * K);

  for (n = 0; pathlist[n] != NULL; n++)
    {
      dd = opendir (pathlist[n]);
      if (dd == NULL)
	continue;
      while (dentry = readdir (dd))
	{
	  sprintf (cmdpath, "%s/%s", pathlist[n], dentry->d_name);
	  lookUpAndAdd (dentry->d_name, cmdpath);
	}
    }
}

/* PURPOSE: EXTRACT EACH PATH FROM PATH VARIABLE *
/* INPUT: VOID*/
/* OUTPUT: RETURNS A POINTER TO PATHS */


char **
getpathlist ()
{
  char *path, *p;
  char **pathlist;
  char b[K];
  int i, n;

  pathlist = malloc (sizeof (char *) * K);
  path = getenv ("PATH");
  strcpy (b, path);
  for (p = b, n = 0; *p; p++)
    if (*p == COLON)
      {
	*p = 0;
	pathlist[n++] = p + 1;
      }
  return pathlist;
}


/*PURPOSE: CHECK REDIRECTION AND CREATE AND RETURN FILE DESCRIPTORS */
/* INPUT : ARGUMENT TYPE - LESS,PIPE, GREATER, LESS, ARGUMENT OR FILE NAME */
/* OUTPUT: RETURNS A FILE DESCRIPTOR OR NOTFOUND */


int
redirectcmd (char *type, char *filename)
{

  int fd;

  if (access (filename, F_OK) == 0)
    {				/* file exists */
      if (strcmp (type, "less") != 0)
	if (access (filename, W_OK) == 0)
	  {
	    if (strcmp (type, "greater") == 0)
	      fd = open (filename, O_WRONLY | O_TRUNC);
	    else if (strcmp (type, "dgreater") == 0)
	      fd = open (filename, O_WRONLY | O_APPEND);
	  }
	else
	  {
	    fprintf (stderr, "%s: permission denied\n", filename);
	    return NOTFOUND;
	  }
      if (strcmp (type, "less") == 0)
	if (access (filename, R_OK) == 0)
	  {
	    if (strcmp (type, "less") == 0)
	      fd = open (filename, O_RDONLY);
	  }
	else
	  {
	    fprintf (stderr, "%s: permission denied\n", filename);
	    return NOTFOUND;
	  }
    }
  else
    {
      fd = open (filename, O_WRONLY | O_CREAT, 0644);
      if (fd < 0)
	{
	  fprintf (stderr, "Cannot create or open: %s\n", filename);
	  return NOTFOUND;
	}
    }
  if ((strcmp (type, "dgreater") == 0) || (strcmp (type, "greater") == 0))
    {
      close (1);
      dup (fd);
    }
  else if (strcmp (type, "less") == 0)
    {
      close (0);
      dup (fd);
    }
  return fd;
}
