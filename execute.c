
#include "header.h"

/* PURPOSE: EXECUTE COMMAND BY CREATING CHILD PROCESS */
/* INPUTS:COMMAND, ARGUMENTS, ENVP , PARSED COMMANDLINE ITEMS */
/* OUTPUT: REDIRECTS IF NECESSARY*/

void
execmd (char *cmd, char **args, char **envp, REDIR * redirects)
{
  int i, fd, fd2, k, m, bg, stop;
  int pid;
  int status;
  int n, loc;
  FILE *fdex, *pfd;

  bg = stop = FALSE;
  pid = fork ();
  if (pid)
    {
	wait (&status);
    }
  else
    {

      for (k = 0; redirects[k].type; k++)
	{
	  if ((strcmp (redirects[k].type, "less") == 0) &&
	      ((strcmp (redirects[k + 1].type, "dgreater") == 0) ||
	       (strcmp (redirects[k + 1].type, "greater") == 0)))
	    {
	      fd = redirectcmd (redirects[k].type, redirects[k].arg);
	      fd2 = redirectcmd (redirects[k + 1].type, redirects[k + 1].arg);
	      execve (cmd, args, envp);
	      stop = TRUE;
	      close (fd);
	      close (fd2);
	    }
	  else if (strcmp (redirects[k].type, "less") == 0)
	    {
	      fd = redirectcmd (redirects[k].type, redirects[k].arg);
	      execve (cmd, args, envp);
	      stop = TRUE;
	      close (fd);
	    }
	  if ((strcmp (redirects[k].type, "dgreater") == 0)
	      || (strcmp (redirects[k].type, "greater") == 0))
	    {
	      fd = redirectcmd (redirects[k].type, redirects[k].arg);
	      stop = TRUE;
	      execve (cmd, args, envp);
	      close (fd);
	    }
	  if ((strcmp (redirects[k].type, "pipe") == 0))
	    {
	      char *dat = malloc (sizeof (char) * K);
	      char **tokens = malloc (sizeof (char *) * K);
	      REDIR *predir = malloc (sizeof (REDIR) * K);
	      predir = getredirect (redirects[k].arg);
	      n = 0;
	      for (; predir[n].type; n++)
		if (strcmp (predir[n].type, "word") == 0)
		  tokens[n] = strdup (predir[n].arg);
		else
		  break;
	      tokens[n] = 0;
	      loc = lookupcmd (extractcmd (tokens[0]));

	      if (loc == NOTFOUND)
		{
		  fdex = fopen (tokens[0], "r");
		  if (fdex)
		    {
		      execve (tokens[0], tokens, envp);
		      fclose (fdex);
		    }
		  else
		    fprintf (stderr, "%s:Command not found!\n", tokens[0]);
		}
	      else
		{
		  pfd = popen (ht[loc].path, "r");
		  if (!pfd)
		    {
		      fprintf (stderr, "Could not open pipe");
		      return;
		    }
		  fgets (dat, K, pfd);
		  fprintf (stdout, "%s\n", dat);
//                execve (ht[loc].path, tokens, envp);
		  pclose (pfd);
		}
	    }

	}
      if (!stop)
	execve (cmd, args, envp);
      return;
    }
}
