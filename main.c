#include "header.h"

/*THIS IS THE MAIN FUNCTION */
/* IT DISPLAYS PROMPT, GETS COMMANDLINE STRING, PARSES IT, LOOKUP COMMAND*/
/* IN HASH TABLE AND EXECUTES IT */

int
main (int argc, char *argv[], char *envp[])
{
  FILE *fdex;
  char *cmd2;
  int loc, fd, n, k, m, i;

  cmdline = malloc (sizeof (char) * K);
  tokens = malloc (sizeof (char *) * K);

  pathlist = getpathlist ();

  findAddcmds ();
  for (;;)
    {
      i = 0;
      fprintf (stdout, "Afundoh >>");
      if ((fgets (cmdline, K, stdin) == NULL))	// || (cmdline[0] == '\n'))
	{
	  fprintf (stdout, "\n");
	  exit (0);
	}
      else
	{
	  if (cmdline[0] == '\n')
	    continue;

	  REDIR *redirects = getredirect (cmdline);
	  if (strcmp (redirects[0].arg, "set") == 0)
	    {
	      if (process_set (redirects) == TRUE)
		fprintf (stdout, "Path successfully set\n");
	      else
		fprintf (stderr, "Failed to set path\n");
	      continue;
	    }
	  if (checkcmdtype (redirects) == TRUE)
	    continue;
	  k = 0;
	  for (; redirects[k].type; k++)
	    if (strcmp (redirects[k].type, "word") == 0)

	      tokens[k] = strdup (redirects[k].arg);
	    else
	      break;
	  tokens[k] = 0;

	  loc = lookupcmd (extractcmd (tokens[0]));

	  if (loc == NOTFOUND)
	    {
	      fdex = fopen (tokens[0], "r");
	      if (fdex)
		{
		  execmd (tokens[0], tokens, envp, redirects);
		  fclose (fdex);
		}
	      else
		fprintf (stderr, "%s:Command not found!\n", tokens[0]);
	    }
	  else
	    {
	      execmd (ht[loc].path, tokens, envp, redirects);
	    }
	}
    }
  closedir (dd);
}
