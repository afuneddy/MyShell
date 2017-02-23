#include "header.h"



/* PURPOSE: HASH FUNCTION */
/* INPUT: COMMAND NAME */
/* OOUTPUT: RETURNS ARRAY  INDEX */

int
hash (char s[])
{
  int i, hv;

  hv = 0;
  for (i = 0; s[i]; i++)
    hv = ((hv * HASHV) + (s[i] - '-')) & TABLEMASK;
  return hv;
}

/*PURPOSE: ADD COMMANDS AND THEIR PATHS TO HASH TABLE STRUCTURE */
/* INPUT: COMMAND AND PATH */
/*OUTPUT : VOID */


void
lookUpAndAdd (char *s, char *path)
{
  int hv;
  hv = hash (s);
  while (ht[hv].cmd != NULL)
    {
      if (strcmp (ht[hv].cmd, s) == 0)
	return;
      hv = (hv + 1) & TABLEMASK;
    }
  ht[hv].cmd = strdup (s);
  ht[hv].path = strdup (path);
}

/* PURPOSE: LOOKUP COMMAND IN HASH TABLE*/
/* INPUTS: COMMAND */
/* OUTPUT: RETURN INDEX LOCATION OR NOTFOUND */

int
lookupcmd (char *s)
{
  int hv;

  hv = hash (s);
  while (ht[hv].cmd != NULL)
    {
      if (strcmp (ht[hv].cmd, s) == 0)
	return hv;
      hv = (hv + 1) & TABLEMASK;
    }
  return NOTFOUND;
}

