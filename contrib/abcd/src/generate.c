/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: generate.c,v 1.2 2000-05-08 11:33:36 biere Exp $
 *==========================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <time.h>

/*------------------------------------------------------------------------*/

char * generate_rcsid =
"$Id: generate.c,v 1.2 2000-05-08 11:33:36 biere Exp $"
;

/*------------------------------------------------------------------------*/

char * STRDUP(const char * p)
{
  return strcpy((char*)malloc(strlen(p)+1),p);
}

char * RINDEX(char * p, int c)
{
  int l = strlen(p), i;
  for(i=l-1; i>=0; i--)
    if(p[i] == c) return p + i;
  
  return 0;
}

typedef struct Assoc
{
  char * key;
  char * value;
  struct Assoc * next;
}
Assoc;

static Assoc * assocs = 0;

static int all_are_functions = 0;
static int all_are_macros = 0;

static void fatal(const char * fmt, ...)
{
  va_list ap;
  va_start(ap,fmt);
  fprintf(stderr, "*** generate: ");
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  exit(1);
}

static Assoc * new_Assoc(char * key, char * value)
{
  Assoc * res = (Assoc*) malloc(sizeof(Assoc));
  res -> key = key;
  res -> value = value;
  res -> next = assocs;
  assocs = res;
  
  if(strcmp(key, "generate")==0)
    {
      if(strcmp(value, "macros")==0)
        {
	  all_are_functions = 0;
	  all_are_macros = 1;
	}
      else
      if(strcmp(value, "functions")==0)
        {
	  all_are_functions = 1;
	  all_are_macros = 0;
	}
      else
      if(strcmp(value, "custom")==0)
        {
	  all_are_functions = 0;
	  all_are_macros = 0;
	}
      else fatal("could not recognize value `%s' for `generate'\n", value);
    }

  return res;
}

static char * cat(const char * a, const char * b)
{
  int l = strlen(a) + strlen(b) + 1;
  char * res = (char*) malloc(l);
  strcpy(res, a);
  strcat(res, b);
  return res;
}

#define MAXBUFFER 1000
static char buffer[MAXBUFFER];

static int _readln(FILE * file, char * b, int max, int * lineno)
{
  if(feof(file))
    {
      b[0]='\0';
      return 0;
    }
  else
    {
      int i, ch = getc(file);

      for(i=0; ch != EOF && ch != '\n' && i<max-1; i++)
        {
	  b[i] = ch;
	  ch = getc(file);
	}
      
      if(i>=max-1) fatal("buffer overflow\n");

      b[i]='\0';
      (*lineno)++;
      return 1;
    }
}

static int readln(FILE * file, int * lineno)
{
  return _readln(file, buffer, MAXBUFFER, lineno);
}

static char * strip_white_space(char * p)
{
  char * q;

  while(*p && (*p==' ' || *p=='\t'))
    p++;
  
  for(q=p+strlen(p)-1; q>=p && (*q==' ' || *q=='\t'); q--)
    ;
  
  q[1]=0;
  return p;
}

static void parse_cache_line()
{
  char * key, * value, * p;
  p = strip_white_space(buffer);
  if(*p=='\0' || *p=='%' || *p == '#') return;
  p = strtok(buffer, "=");
  if(p)
    {
      key = STRDUP(p);
      p = strtok(0, "");
      if(p)
        {
          value = STRDUP(p);
          (void) new_Assoc(key, value);
        }
      else free(key);
    }
}

static void read_cache(const char * name)
{
  int lineno=0;
  FILE * file = fopen(name, "r");
  if(!file) fatal("could not open cache `%s'\n", name);
  while(readln(file, &lineno)) parse_cache_line();
  fclose(file);
}

static const char * find(const char * key)
{
  Assoc * p;
  for(p=assocs; p; p=p->next)
    if(strcmp(p -> key, key) == 0) return p -> value;
  
  return 0;
}

static int is_header(char * name)
{
  char * p = RINDEX(name, '.');
  return strcmp(p, ".h") == 0;
}

static char * strip_suffix(char * name)
{
  char * p = RINDEX(name, '.');
  if(p)
    {
      int l = p - name;
      char * res = (char*) malloc(l + 1);
      strncpy(res, name, l);
      res[l]='\0';
      return res;
    }
  else 
    {
      fatal("`%s' has no valid suffix\n", name);
      return 0;
    }
}

static void print_buffer(FILE * dst, char * p, int newline)
{
  if(p)
    {
      fprintf(dst, "%s", p);
      p = strtok(0, "");
      if(p) fprintf(dst, " %s", p);
    }

  if(newline) fputc('\n', dst);
}

static char * rest_of_buffer(int lineno)
{
  char * p = strtok(0, "");

  if(p) return STRDUP(strip_white_space(p));
  else 
    {
      fatal("something missing at line %d\n", lineno);
      return 0;
    }
}

static void print_rest(FILE * dst, int newline)
{
  char * p = strtok(0, "");
  if(p)
    {
      p = strip_white_space(p);
      if(*p) fprintf(dst, "%s", p);
    }
  if(newline) fputc('\n', dst);
}

static int found_end()
{
  char * p;

  for(p=buffer; *p && (*p==' ' || *p=='\t'); p++)
    ;
  
  if(*p) return strncmp(p, "END", 3) == 0;
  else return 0;
}

static void filter(
  FILE * src, FILE * dst, const char * name, int is_header, int * lineno)
{
  char * p;

  p = strtok(buffer, " \t");

  if(p)
    {
      if(is_header)
        {
	  static int first = 0, is_function = 1, start = 0, is_macro = 0;

          if(strcmp(p, "FUNCTION")==0)
            {
	      char * type = 0;

	      type = rest_of_buffer(*lineno);
	      is_function = (strcmp(type, "void") != 0);
	      start = *lineno;

	      if(!readln(src, lineno)) 
	        fatal(
		  "function body missing at line %d in %s\n", *lineno, name);
	      
	      p = strtok(buffer, " \t");
	      if(p)
	        {
		  const char * value;

		  fprintf(dst, "#line %d \"%s\"\n", *lineno-1, name);

		  if(!all_are_functions &&
		     (all_are_macros || !(value = find(p)) ||
		      strcmp(value, "macro")==0))
		    {
		      is_macro=1;
		      fprintf(dst, "#define %s( \\\n", p);
		    }
		  else 
		    {
		      is_macro=0;
		      fprintf(dst, "extern %s\n%s(\n", type, p);
		    }
		}
	      else fatal(
  "empty line after FUNCTION at line %d in %s\n", *lineno, name);

	      free(type);
	      first = 1;
	    }
          else
          if(strcmp(p, "TYPE")==0)
            {
	      if(is_macro)
	        {
	          if(first==0) fprintf(dst, ", \\\n");
	          first=0;
		}
	      else
	        {
	          if(first==0) fprintf(dst, ", \n");
	          fprintf(dst, "  ");
	          print_rest(dst, 0);
	          first=0;
		}
	    }
          else
          if(strcmp(p, "ARG")==0)
            {
	      if(is_macro)
	        {
		  fprintf(dst, "  ");
		  print_rest(dst, 0);
		}
	      else
	        {
		  fprintf(dst, "\n  ");
		  print_rest(dst, 0);
		}
	    }
          else
          if(strcmp(p, "BODY")==0)
            {
	      int found = 0;

	      if(is_macro)
	        {
		  fprintf(dst, ") \\\n");
		  if(is_function) fprintf(dst, "( \\\n");
		  else fprintf(dst, "do { \\\n");

		  while(readln(src, lineno) && !(found=found_end()))
		    {
		      fprintf(dst, "%s \\\n", buffer);
		    }
		  
		  if(is_function) fprintf(dst, ")\n");
		  else fprintf(dst, "} while(0)\n");
		}
	      else
	        {
	          fprintf(dst, ");\n");

		  while(readln(src, lineno) && !(found=found_end()))
		    ;
		}
	      
	      if(!found) fatal(
  "could not found END of function at line %d in %s\n", start, name);

	    }
          else print_buffer(dst, p, 1);
	}
      else
        {
	  static int first = 0, is_function = 1, start = 0, is_macro = 0;

          if(strcmp(p, "FUNCTION")==0)
            {
	      char * type = 0;

	      type = rest_of_buffer(*lineno);
	      is_function = (strcmp(type, "void") != 0);
	      start = *lineno;

	      if(!readln(src, lineno)) 
	        fatal(
  "function body missing at line %d in %s\n", *lineno, name);
	      
	      p = strtok(buffer, " \t");
	      if(p)
	        {
		  const char * value;
		  if(!all_are_functions &&
		     (all_are_macros || !(value = find(p)) ||
		      strcmp(value, "macro")==0))
		    {
		      is_macro=1;
		      fprintf(dst, "/* %s is a macro */\n", p);
		    }
		  else 
		    {
		      is_macro=0;
	              fprintf(dst, "#line %d \"%s\"\n", *lineno, name);
		      fprintf(dst, "%s\n%s(\n", type, p);
		    }
		}
	      else fatal(
  "empty line after FUNCTION at line %d in %s\n", *lineno, name);

	      free(type);
	      first = 1;
	    }
          else
          if(strcmp(p, "TYPE")==0)
            {
	      if(is_macro)
	        {
		  /* empty */
		}
	      else
	        {
	          if(first==0) fprintf(dst, ", \n");
	          fprintf(dst, "  ");
	          print_rest(dst, 0);
	          first=0;
		}
	    }
          else
          if(strcmp(p, "ARG")==0)
            {
	      if(is_macro)
	        {
		  /* empty */
		}
	      else
	        {
		  fputs("\n  ", dst);
		  print_rest(dst, 0);
		}
	    }
          else
          if(strcmp(p, "BODY")==0)
            {
	      int found = 0;

	      if(is_macro)
	        {
		  while(readln(src, lineno) && !(found=found_end()))
		    ;
		}
	      else
	        {
	          fprintf(dst, ")\n{\n");
	          if(is_function) fprintf(dst, "  return \n");
		  fprintf(dst, "#line %d \"%s\"\n", *lineno+1, name);

	          while(readln(src, lineno) && !(found=found_end()))
	            {
		      if(is_function) fprintf(dst, "  ");
		      fprintf(dst, "%s\n", buffer);
		    }

		  if(found)
		    {
	              if(is_function) fprintf(dst, "  ;\n");
	              fprintf(dst, "}\n");
		    }
		}
	      
	      if(!found) fatal(
  "could not found END of function at line %d in %s\n", start, name);

	    }
          else print_buffer(dst, p, 1);
	}
    }
  else print_buffer(dst, p, 1);
}

static void generate(char * name)
{
  int ih = is_header(name), lineno=0;
  char * base = strip_suffix(name), * src_name = cat(base, ".tmpl");
  FILE * src = fopen(src_name, "r"), * dst;
  time_t t;

  if(!src) fatal("could not open `%s' for reading\n", src_name);
  dst = fopen(name, "w");
  if(!dst) fatal("could not open `%s' for writing\n", name);

  fprintf(dst, "/* THIS FILE IS AUTOMICALLY GENERATED -- DO NOT EDIT\n");
  time(&t);
  fprintf(dst, " * from `%s' on %s */\n\n", src_name, ctime(&t));
  if(ih)
    {
      fprintf(dst, "#ifndef _%s_h_INCLUDED\n", base);
      fprintf(dst, "#define _%s_h_INCLUDED\n\n", base);
    }

  while(readln(src,&lineno)) filter(src,dst,src_name,ih,&lineno);

  if(ih) fprintf(dst, "#endif\n");

  free(base);
  free(src_name);

  fclose(dst);
  fclose(src);
}

int main(int argc, char ** argv)
{
  int i;
  Assoc * p, * tmp;

  read_cache("config.cache");

  for(i=1; i<argc; i++) generate(argv[i]);

  for(p=assocs; p; p=tmp)
    {
      free(p -> key);
      free(p -> value);
      tmp = p -> next;
      free(p);
    }

  exit(0);
  return 0;
}
