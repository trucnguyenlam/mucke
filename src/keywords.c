#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct _list {
  struct _node * data;
  struct _list * next;
} _list, *list;

typedef struct _node {
  char c;
  union {
    list asList;
    const char* asStr;
  } sons;
} _node, *node;

static int lineno = 0;

FILE *input=0, *output=0;

static void close_files()
{
  if(input!=stdin) fclose(input);
  if(output!=stdin) fclose(output);
}

static void error(const char * s)
{
  fprintf(stderr, "*** error: %s at line %d\n", s, lineno);
  close_files();
  exit(1);
}

static char* save_malloc(unsigned int i)
{
  char* res;
  if( !(res=(char*)malloc(i)) )
    error("memory overflow");
  return res;
}

static int numberOfNodes=0;
static node newNode(char c)
{
  node res = (node)save_malloc(sizeof(_node));
  res->c = c;
  res->sons.asList = 0;
  numberOfNodes++;
  return res;
}

static int numberOfStrings=0;
static node newStr(const char * s)
{
  node res = (node)save_malloc(sizeof(_node));
  res->c = '\0';
  res->sons.asStr = strcpy(save_malloc(strlen(s)+1), s);
  numberOfStrings++;
  return res;
}

#define car(l) ((l)->data)
#define cdr(l) ((l)->next)

static int numberOfListNodes=0;
static list cons(node n, list l)
{
  list res = (list)save_malloc(sizeof(_list));
  res->data = n;
  res->next = l;
  numberOfListNodes++;
  return res;
}

#define MAX 1000
static char buffer1[MAX], buffer2[MAX];

static int read(char* buf)
{
  int c,i;

  while( isspace(c=getc(input)) )
    if(c=='\n') lineno++;
  if(c==EOF) return 0;

  for(i=0; c != EOF && !isspace(c) && i<MAX; i++)
    {
      buf[i] = c;
      c = getc(input);
    }

  if(i>=MAX) error("word too long ...");

  if(c==EOF)
   {
     if(i==0) return 0;
     ungetc(c, input);
   }
  else
  if(c=='\n') lineno++;

  buf[i] = '\0';
  return 1;
}

static list root = 0;

static void insert(const char* k, const char* d)
{
  list l = root, *last = &root;

  while(l && *k)
    {
      for(; l && car(l)->c != *k; last = &cdr(l), l=cdr(l))
        ;
      if(l)
        {
	  k++;
	  last = &(car(l)->sons.asList);
	  l = car(l)->sons.asList;
	}
    }

  if(!*k && !car(*last)->c) error("multiply defined keys");

  for(; *k; k++, last = &(car(*last)->sons.asList))
    *last = cons( newNode(*k), *last);

  *last = cons( newStr(d), *last );
}

static int real_tab_size = 8;
static int _tabsize = 2;

                       /*00112233445566778899*/
static char _tabs[] =   "\t\t\t\t\t\t\t\t\t\t";

                       /*01234567*/
static char _spaces[] = "        ";

#define min(a,b) ((a)<(b)?(a):(b))

static void tab(unsigned int i)
{
  int d, m;
  i *= _tabsize;

  d = min( i/real_tab_size, sizeof(_tabs)-1 ) ;
  m = min( i%real_tab_size, sizeof(_spaces)-1 );

  fputs( _tabs + sizeof(_tabs) - 1 - d , output);
  fputs( _spaces + sizeof(_spaces) - 1 - m, output);
}

static char * default_return = "0";
static char * default_string = "s";

static int singleton(list l) { return l && !cdr(l); }

static void _print(list l, int i, char * pos)
{
  const char * p = 0;

  if(!l) return;

  if( singleton(l) )
    {
      tab(i);
      *pos = car(l)->c;
      if(*pos)
        {
	  fprintf(
	    output,
	    "if(*%s++=='%s%c'",
	    default_string,
	    *pos == '\\' ? "\\" : "",
	    *pos
	  );
	  l = car(l)->sons.asList;
	  while( singleton(l) && car(l)->c)
	    {
	      fputs(" &&\n", output);
	      tab(i);
              *++pos = car(l)->c;
	      fprintf(
	        output,
		"   *%s++=='%s%c'",
	        default_string,
	        *pos == '\\' ? "\\" : "",
	        *pos
	      );
	      l = car(l)->sons.asList;
	    }

	  if( !singleton(l) )
	    {
	      fputs("){\n", output);
              _print(l, i+1, pos+1);
	      tab(i);
	      fputs("} ", output);
	    }
	  else
	    {
	       pos[1] = '\0';
	       fprintf(output, ") return *%s ? %s : %s;\t/* %s */\n",
	         default_string, default_return,
		 car(l)->sons.asStr, buffer1);
	       tab(i);
	    }
	  fprintf(output, "else return %s;\n", default_return);
	}
      else
        fprintf(output,
	  "return %s;\t/* %s */\n", car(l)->sons.asStr, buffer1);
    }
  else
    {
      tab(i);
      fprintf(output, "switch(*%s++) {\n", default_string);
      for(;l; l=cdr(l))
        {
          if(!car(l)->c)
            p = car(l)->sons.asStr;
          else
            {
	      list k;
	      *pos = car(l)->c;
              tab(i+1);
	      fprintf(
	        output,
		"case '%s%c' :",
	        *pos == '\\' ? "\\" : "",
		*pos
	      );
	      if( singleton(k=car(l)->sons.asList) && !car(k)->c )
	        {
                  pos[1] = '\0';
	          fprintf(output, " return *%s ? %s : %s;\t/* %s */\n",
		    default_string, default_return,
		     car(k)->sons.asStr, buffer1);
		}
	      else
	        {
		  putc('\n', output);
                  _print( k, i+2, pos+1);
		}
	    }
        }
      if(p)
        {
          tab(i+1);
	  *pos = '\0';
	  fprintf(output,
	    "case '\\0': return %s;\t\t/* %s */\n", p, buffer1);
        }
      tab(i+1);
      fprintf(output, "default  : return %s;\n", default_return);
      tab(i);
      fputs("};\n", output);
    }
}

static void print() { _print(root, 1, buffer1); }

static const char* kw_basename(const char *p)
{
  const char *q;
  for(q=p; *p; p++)
    if(*q=='/'||*q=='\\') q=p;

  return q;
}

static void usage(const char * command)
{
  fprintf(stderr,
"usage %s [...] [<input-file>]\n"
"\n"
"  -v\n"
"\n"
"     verbose\n"
"\n"
"  -r <string>, --default-return-value <string>\n"
"\n"
"     Use <string> as default return value when no\n"
"     other string matches.\n"
"\n"
"\n"
"  -s <string>, --string-name <string>\n"
"\n"
"     Use <string> as the variable name of type `char *'.\n"
"\n"
"\n"
"  -o <file>, --output-file <file>\n"
"\n"
"\n"
"  <file>, --input-file <file>\n"
"\n",
kw_basename(command));
}

static int verbose = 0;

int main(int argc, char** argv)
{
  int i;

  for(i=1; i<argc; i++)
    {
      if( strcmp(argv[i],"-v") == 0 ||
          strcmp(argv[i],"--verbose") == 0 )
	{
	  verbose = 1;
	}
      else
      if( strcmp(argv[i],"-r") == 0 ||
          strcmp(argv[i],"--default-return-value") == 0 )
	{
	  if(++i>=argc||argv[i][0]=='-') error("argument is missing");
	  default_return = argv[i];
	}
      else
      if( strcmp(argv[i],"-s") == 0 ||
          strcmp(argv[i],"--string-name") == 0 )
	{
	  if(++i>=argc||argv[i][0]=='-') error("argument is missing");
	  default_string = argv[i];
	}
      else
      if( strcmp(argv[i],"-o") == 0 ||
          strcmp(argv[i],"--output-file") == 0 )
	{
	  if(++i>=argc||argv[i][0]=='-') error("argument is missing");
	  if(output) error("multiple output files not allowed!");
	  output = fopen(argv[i], "w");
	  if(!output)
	    {
	      fprintf(stderr,
 	        "*** error: could not open %s for write.\n",
	        argv[i]);
              close_files();
	      exit(1);
	    }
	}
      else
      if(argv[i][0] != '-' ||
         strcmp(argv[i],"--input-file") == 0)
        {
	  if( argv[i][0] == '-' && (++i>=argc || argv[i][0]=='-') )
	     error("argument is missing");
	  if(input) error("multiple input files not allowed!");
	  input = fopen(argv[i], "r");
	  if(!input)
	    {
	      fprintf(stderr,
	        "*** error: could not open %s for read.\n",
		argv[i]);
	      close_files();
	      exit(1);
	    }
	}
      else
        {
	  usage(argv[0]);
	  close_files();
	  exit(1);
	}
    }

  if(!output) output = stdout;
  if(!input) input = stdin;

  lineno = 1;
  while( read(buffer1) )
    {
      if(!read(buffer2))
        {
          fprintf(stderr,
	    "*** warning: odd number of words -- skipping `%s' at line %d\n",
	      buffer1, lineno);
	  break;
	}
      else
        insert(buffer1, buffer2);
    }

  if(verbose)
    {
      fprintf(stderr, "%8d internal nodes\n", numberOfNodes);
      fprintf(stderr, "%8d list nodes\n", numberOfListNodes);
      fprintf(stderr, "%8d string nodes\n", numberOfStrings );
    }

  print();

  close_files();
  exit(0);

  return 0;
}
