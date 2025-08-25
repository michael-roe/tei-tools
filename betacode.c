#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <wchar.h>
#include <wctype.h>

#include "betacode.h"

static wchar_t *permitted = L" abgdevzhqiklmncoprtufxyw()/=\\+|?.,:;'-_\n\t";

struct mapping {
  wchar_t src;
  wchar_t dst;
};

static struct mapping table[] = {
  {'a', 0x3b1},
  {'b', 0x3b2},
  {'c', 0x3be},
  {'d', 0x3b4},
  {'e', 0x3b5},
  {'f', 0x3c6},
  {'g', 0x3b3},
  {'h', 0x3b7},
  {'i', 0x3b9},
  {'k', 0x3ba},
  {'l', 0x3bb},
  {'m', 0x3bc},
  {'n', 0x3bd},
  {'o', 0x3bf},
  {'p', 0x3c0},
  {'q', 0x3b8},
  {'r', 0x3c1},
  /* sigma is special, because initial and final sigma differ */
  {'t', 0x3c4},
  {'u', 0x3c5},
  {'v', 0x3dd},
  {'w', 0x3c9},
  {'x', 0x3c7},
  {'y', 0x3c8},
  {'z', 0x3b6},
  {'\'', 0x2019}, /* right single quotation mark */
  {'-', 0x2010}, /* hyphen */
  {'_', 0x2014}, /* em dash */
  /*
   * period, comma, middle dot, and semicolon map to themselves 
   * space and newline also map to themselves, even if the betacode
   * specification doesn't say so explicitly.
   */
  {0, 0}
};

static struct mapping diacritics[] = {
  {')', 0x313},
  {'(', 0x314},
  {'/', 0x301},
  {'=', 0x342},
  {'\\', 0x300},
  {'+', 0x308},
  {'|', 0x345},
  {'?', 0x323},
  {0, 0}
};

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

static wchar_t translate(wchar_t c)
{
  struct mapping *mp;

  mp = table;

  while (mp->src)
  {
    if (mp->src == c)
    {
      return mp->dst;
    }
    mp++;
  }
}

static wchar_t diacritic(wchar_t c)
{
  struct mapping *mp;

  mp = diacritics;

  while (mp->src)
  {
    if (mp->src == c)
    {
      return mp->dst;
    }
    mp++;
  }
}

void betacode_translate(uint32_t *dst, uint32_t *src, int size)
{
  wchar_t *in_ptr;
  wchar_t *out_ptr;
  wchar_t c;
  int upper_case;

  in_ptr = src;
  out_ptr = dst;
  upper_case = 0;

  while (*src)
  {
    if (*src == '*')
    {
      upper_case = 1;
      src++;
    }
    else if (*src == 's')
    {
     /*
      * Sigma is special, because final sigma and medial sigma
      * are different characters.
      */

      if (src[1] == '1')
      {
        *dst = 0x3c3;
        src++;
      }
      else if (src[1] == '2')
      {
        *dst = 0x3c2;
        src++;
      }
      else if (src[1] == '3')
      {
        *dst = 0x3f2;
        src++;
      }
      else if (src[1] == '\0')
      {
        *dst = 0x3c2;
      }
      else if ((src[1] == ' ') || (src[1] == '.') || (src[1] == ','))
      {
        /* Probably should check for other punctuation */
        *dst = 0x3c2;
      }
      else
      {
        *dst = 0x3c3;
      }

      if (upper_case)
      {
        *dst = towupper(*dst);
        upper_case = 0;
      }
      src++;
      dst++;
    }
    else if (c = translate(*src))
    {
      *dst = c;
      if (upper_case)
      {
        *dst = towupper(*dst);
        upper_case = 0;
      }
      src++;
      dst++;
    }
    else if (c = diacritic(*src))
    {
      *dst = c;
      src++;
      dst++;
    }
    else
    {
      *dst = *src;
      src++;
      dst++;
    }
  }

  *dst = '\0';
}

void betacode_table()
{
  struct mapping *mp;

  mp = table;

  while (mp->src)
  {
    fwprintf(stderr, L"%lc %lc\n", mp->src, mp->dst);
    mp++;
  }

  mp = diacritics;

  while (mp->src)
  {
    /* Use alpha a suitable base character to demonstrate diacritics */
    fwprintf(stderr, L"%lc %lc%lc\n", mp->src, 0x3b1, mp->dst);
    mp++;
  }
}
