#include "header.h"


/* PURPOSE: TAKES THE COMMAND LINE STRING ENTERED BY USER, PARSES IT AND RETURN ARRAYED STRUCTURE*/
/*IT STORES < > << | & IN STRUCTURE DEFINED BY *REDIR*, RESPECTIVELY WITH TYPES less, greater, dgreater,*/
/* pipe, bground. THEIR args ARE FILE NAMES IF NECCESSARY. */

REDIR *
getredirect (char *args)
{
  int i, k, n, prnch;
  char *a = malloc (sizeof (char *));
  REDIR *redirects = malloc (sizeof (REDIR) * K);

  i = k = n = 0;
  prnch = FALSE;

  for (i = 0; args[i]; i++)
    {
      redirects[n].type = strdup ("");
      redirects[n].arg = strdup ("");

      if ((args[i] == ' '))
	{
	  continue;
	}
      else if (args[i] == '\n')
	{
	  break;
	}
      else if ((args[i] == '>') && (args[i + 1] == '>'))
	{
	  prnch = FALSE;
	  redirects[n].type = strdup ("dgreater");
	  for (i = i + 2; args[i]; ++i)
	    {
	      if ((args[i] == ' ') && (prnch == TRUE))
		break;
	      if (args[i] == ' ')
		continue;
	      if ((args[i] == '<') || (args[i] == '\n') || (args[i] == '>')
		  || (args[i] == '"') || (args[i] == '=')
		  || (args[i] == ';') || (args[i] == ',')
		  || (args[i] == '(') || (args[i] == ')')
		  || (args[i] == '|') || (args[i] == '&'))
		break;
	      else
		{
		  prnch = TRUE;
		  redirects[n].arg[k++] = args[i];
		}
	    }
	  redirects[n++].arg[k] = 0;
	  --i;
	  k = 0;
	}
      else if ((args[i] == '<'))
	{
	  prnch = FALSE;
	  redirects[n].type = strdup ("less");
	  for (i = i + 1; args[i]; ++i)
	    {
	      if ((args[i] == ' ') && (prnch == TRUE))
		break;
	      if (args[i] == ' ')
		continue;
	      if ((args[i] == '<') || (args[i] == '\n') || (args[i] == '>')
		  || (args[i] == '"') || (args[i] == '=')
		  || (args[i] == ';') || (args[i] == ',')
		  || (args[i] == '(') || (args[i] == ')')
		  || (args[i] == '|') || (args[i] == '&'))
		break;
	      else
		{
		  prnch = TRUE;
		  sprintf (a, "%c", args[i]);
		  strcat (redirects[n].arg, a);
		}
	    }
	  --i;
	  n++;
	  k = 0;
	}
      else if ((args[i] == '>') && (args[i + 1] != '>'))
	{
	  prnch = FALSE;
	  redirects[n].type = strdup ("greater");
	  for (i = i + 1; args[i]; ++i)
	    {
	      if ((args[i] == ' ') && (prnch == TRUE))
		break;
	      if (args[i] == ' ')
		continue;
	      if ((args[i] == '<') || (args[i] == '\n') || (args[i] == '>')
		  || (args[i] == '"') || (args[i] == '=')
		  || (args[i] == ';') || (args[i] == ',')
		  || (args[i] == '(') || (args[i] == ')')
		  || (args[i] == '|') || (args[i] == '&'))
		break;
	      else
		{
		  prnch = TRUE;
		  sprintf (a, "%c", args[i]);
		  strcat (redirects[n].arg, a);
		}
	    }
	  --i;
	  n++;
	  k = 0;
	}
      else if (args[i] == '|')
	{
	  i++;
	  while (isspace (args[i]))
	    ++i;
	  redirects[n].type = strdup ("pipe");
	  redirects[n++].arg = strdup (args + i);
	  break;
	}
      else if (args[i] == '=')
	{
	  i++;
	  while (isspace (args[i]))
	    ++i;
	  redirects[n].type = strdup ("assign");
	  redirects[n++].arg = strdup ("=");
	  --i;
	}
      else if (args[i] == '(')
	{
	  i++;
	  while (isspace (args[i]))
	    ++i;
	  redirects[n].type = strdup ("lbrack");
	  redirects[n++].arg = strdup ("(");
	  --i;
	}
      else if (args[i] == ')')
	{
	  i++;
	  while (isspace (args[i]))
	    ++i;
	  redirects[n].type = strdup ("rbrack");
	  redirects[n++].arg = strdup (")");
	  --i;
	}
      else if (args[i] == '"')
	{
	  i++;
	  while (isspace (args[i]))
	    ++i;
	  redirects[n].type = strdup ("quote");
	  redirects[n++].arg = strdup ("\"");
	  --i;
	}
      else if (args[i] == ',')
	{
	  i++;
	  while (isspace (args[i]))
	    ++i;
	  redirects[n].type = strdup ("comma");
	  redirects[n++].arg = strdup (",");
	  --i;
	}
      else if (args[i] == ';')
	{
	  i++;
	  while (isspace (args[i]))
	    ++i;
	  redirects[n].type = strdup ("col");
	  redirects[n++].arg = strdup (";");
	  --i;
	}
      else if (args[i] == '&')
	{
	  redirects[n++].type = strdup ("bground");
	}
      else
	{
	  redirects[n].arg = strdup ("");
	  redirects[n].type = strdup ("word");
	  for (; args[i]; ++i)
	    {
	      if ((args[i] == ' ') || (args[i] == '\n') || (args[i] == '<')
		  || (args[i] == '"') || (args[i] == '=')
		  || (args[i] == ';') || (args[i] == ',')
		  || (args[i] == '(') || (args[i] == ')')
		  || (args[i] == '>') || (args[i] == '|') || (args[i] == '&'))
		break;
	      else
		{
		  sprintf (a, "%c", args[i]);
		  strcat (redirects[n].arg, a);
		}
	    }
	  --i;
	  n++;
	}
      prnch = FALSE;

    }

  return redirects;
}
