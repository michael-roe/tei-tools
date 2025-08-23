#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

#include "betacode.h"

static char *permitted = " abgdevzhqiklmncoprtufxyw()/=\\+|?.,:;'-\n\t";

static wchar_t buff[1024];

int check_betacode(char *str)
{
  unsigned char *cp;
  wchar_t *wcp;
  int not_ascii;

  not_ascii = 0;
  for (cp = str; *cp; cp++)
  {
    if (*cp > 0x7f)
    {
      not_ascii = 1;
    }
  }

  if (not_ascii)
  {
    mbstowcs(buff, str, sizeof(buff));
    wcp = buff;
    while (*wcp)
    {
      if (*wcp > 0x7f)
      {
        fprintf(stderr, "Unexpected character: %x (%lc)\n", *wcp, *wcp);
      }
      wcp++;
    }     
  }
  else
  {
    cp = str;

    while (*cp)
    {
      if (*cp == 's')
      {
      }
      else if (*cp == '*')
      {
      }
      else if (strchr(permitted, *cp) == NULL)
      {
        fprintf(stderr, "Unexpected character: %x (%c)\n", (unsigned char) *cp, *cp);
        fprintf(stderr, "%s\n", str);
        return 1;
      }
      cp++;
    }
  }
  return 0;
}
