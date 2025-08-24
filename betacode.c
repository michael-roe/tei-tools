#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wchar.h>

#include "betacode.h"

static wchar_t *permitted = L" abgdevzhqiklmncoprtufxyw()/=\\+|?.,:;'-\n\t";

static wchar_t buff[1024];

int check_betacode(wchar_t *str)
{
  wchar_t *cp;

  cp = str;

  while (*cp)
  {
    if (*cp == 's')
    {
    }
    else if (*cp == '*')
    {
    }
    else if (wcschr(permitted, *cp) == NULL)
    {
      fprintf(stderr, "Unexpected character: %x (%lc)\n", *cp, *cp);
      fprintf(stderr, "%ls\n", str);
      return 1;
    }
    cp++;
  }
  return 0;
}

int is_ascii_whitespace(char *str)
{
  char *cp;

  cp = str;
  while (*cp)
  {
    /*
     * It is a property of the UTF-8 encoding that characters above 0x7f
     * have encodings with the top bit set.
     */

    if (*cp > 0x7f)
    {
      return 0;
    }

    if (!isspace(*cp))
    {
      return 0;
    }

    cp++;
  }
  return 1;
} 
