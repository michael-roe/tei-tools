#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "betacode.h"

#define BUFFER_SIZE 1024
static wchar_t buff[BUFFER_SIZE];

void parse_tree(xmlNode *node)
{
xmlNode *current;
xmlChar *content;
xmlChar *n;

  for (current = node; current; current = current->next)
  {
    if (current->type == XML_ELEMENT_NODE)
    {
      wprintf(L"<%s", current->name);
      if (strcmp(current->name, "lb") == 0)
      {
        n = xmlGetProp(current, "n");
        if (n)
        {
          wprintf(L" n=\"%s\"", n);
        }
      }
      wprintf(L">");
      parse_tree(current->children);
      wprintf(L"<\\%s>", current->name);
    }
    else
    {
      content = xmlNodeGetContent(current);
      if (content)
      {
        if (is_ascii_whitespace(content))
        {
          wprintf(L"%s", content);
        }
        else
        {
          mbstowcs(buff, content, sizeof(buff));
          buff[BUFFER_SIZE - 1] = '\0';
          if (wcslen(buff) == BUFFER_SIZE - 1)
          {
            fprintf(stderr, "*** buffer filled up ***\n");
          }
          (void) check_betacode(buff);
          wprintf(L"%ls", buff);
        }
      }
    }
  }
}

void parse_text(xmlNode *node)
{
xmlNode *current;

  for (current = node; current; current = current->next)
  {
    if (current->type == XML_ELEMENT_NODE)
    {
      if (strcmp(current->name, "body") == 0)
      {
        parse_tree(current->children);
      }
    }
  }
}

void parse_tei(xmlNode *node)
{
xmlNode *current;

  for (current = node; current; current = current->next)
  {
    if (current->type == XML_ELEMENT_NODE)
    {
      if (strcmp(current->name, "TEI") == 0)
      {
        /*
         * <TEI> elements can be nested recursively
         */
        parse_tei(current->children);
      }
      else if (strcmp(current->name, "text") == 0)
      {
        parse_text(current->children);
      }
    }
  }
}

void parse_root(xmlNode *node)
{
xmlNode *current;

  for (current = node; current; current = current->next)
  {
    if (current->type == XML_ELEMENT_NODE)
    {
      if (strcmp(current->name, "TEI") == 0)
      {
        parse_tei(current->children);
      }
      else
      {
        fprintf(stderr, "Unexpected tag: <%s>\n", current->name);
        exit(-1);
      }
    }
  }
}

int main(int argc, char **argv)
{
  xmlDoc *doc; 
  xmlNode *root_element;

  setlocale(LC_ALL, getenv("LANG"));

  LIBXML_TEST_VERSION

  doc = xmlReadFile("ecclesiazusae.xml", NULL, 0);

  root_element = xmlDocGetRootElement(doc);

  parse_root(root_element);

  xmlCleanupParser();

  return 0;
}
