#include <stdio.h>

#define WORD	0
#define KOMMA	1
#define LD	2
#define RD	3

#define MAXWORD	1024
#define MAXPOS 100

char buffer[MAXWORD];

int position[MAXPOS];
int sp=0;

int isIdentChar(int ch)
{
  return (ch >= '0' && ch <= '9') ||
         (ch >= 'a' && ch <= 'z') ||
         (ch >= 'A' && ch <= 'Z') || ch == '_';
}

int isWhiteSpace(int ch)
{
  return (ch == ' ' || ch == '\n' || ch == '\t');
}

int lineno = 1;

int next_token()
{
  int ch;
  
  while(isWhiteSpace(ch = getc(stdin)))
    if(ch=='\n') lineno++;

  if(ch==EOF) return -1;

  if(isIdentChar(ch))
    {
      int i = 0;
      buffer[i++] = ch;
      while(isIdentChar(ch = getc(stdin)))
	{
	  if(i>=MAXWORD-1)	// leave space for trailing zero
	    {
	      fprintf(stderr,
"*** extractcmpts:%d: maximal length of word exceeded\n",
                      lineno);
	      exit(1);
	    }
	  else buffer[i++] = ch;
	}
      
      ungetc(ch,stdin);
      buffer[i] = 0;

      return WORD;
    }
  else
  if(ch==',') return KOMMA;
  else
  if(ch=='{') return LD;
  else
  if(ch=='}') return RD;
  else
    {
      fprintf(stderr,
"*** extractcmpts:%d: can not process character 0x%X\n",
              lineno, ch);
      exit(1);
    }

}

void usage()
{
  printf("\
'extractcmpts' ( <NUMBER>? '.' ) * <NUMBER>\n\
\n\
  (without spaces between dots and numbers!)\n\
");
  exit(1);
}

void parseposition(const char * s)
{
  int res=0;

  while(*s)
    {
      if(*s=='.')
        {
	  if(sp>=MAXPOS)
	    {
	      fprintf(stderr,
"*** extractcmpts: number of positions exceeded\n");
              exit(1);
	     
	    }
	  else
	    {
	      position[sp++] = res;
	      res = 0;
	    }
	}
      else
      if(*s < '0' || *s > '9')
        {
	  fprintf(stderr,
"*** extractcmpts: could not parse commandline argument\n");
          exit(1);
        }
      else res = 10 * res + *s - '0';
      
      if(*++s==0)
        {
	  if(sp>=MAXPOS)
	    {
	      fprintf(stderr,
"*** extractcmpts: number of positions exceeded\n");
              exit(1);
	     
	    }
	  else
	    {
	      position[sp++] = res;
	      res = 0;
	    }
	}
    }
}

static void getonlyone(const char * s)
{
  int depth, pos, token, open;

  parseposition(s);

  depth = 0; pos = 0;

  while(1)
    {
      token = next_token();

      if(depth == sp - 1 && pos == position[depth])
        {
	  if(token != WORD)
	    {
	      fprintf(stderr,
"*** extractcmpts:%d: expected a word\n", lineno);
	      exit(1);
	    }

	  break;
	}

      if(token < 0)
        {
	  fprintf(stderr,
"*** extractcmpts: oops EOF encountered\n");
	  exit(1);
	}
      
      if(token == KOMMA)
        {
	  if(position[depth]==pos)
	    {
	      fprintf(stderr,
"*** extractcmpts:%d: oops Komma at wron position\n", lineno);
              exit(1);
	    }
          else pos ++;
	}
      else
      if(token == LD)
        {
	  if(position[depth] == pos)
	    {
	      depth++;
	      pos = 0;
	    }
	  else				// skip this cmpts
	    {
	      open = 1;
	      while(open>0)
	        {
		  token = next_token();
                  if(token < 0)
                    {
	              fprintf(stderr,
"*** extractcmpts: oops EOF in nonclosed cmpts encountered\n");
	              exit(1);
		    }

		  if(token == LD) open++;
		  else
		  if(token == RD) open--;
		}
	    }
	}
      else
      if(token == WORD)
        {
	  if(position[depth]==pos)
	    {
	      fprintf(stderr,
"*** extractcmpts:%d: oops Word at wron position\n", lineno);
              exit(1);
	    }
          else continue;
	}
      else
      if(token == LD)
        {
	  if(position[depth] == pos)
	    {
	      depth++;
	      pos = 0;
	    }
	  else				// skip this cmpts
	    {
	      open = 1;
	      while(open>0)
	        {
		  token = next_token();
                  if(token < 0)
                    {
	              fprintf(stderr,
"*** extractcmpts: oops EOF in nonclosed cmpts encountered\n");
	              exit(1);
		    }

		  if(token == LD) open++;
		  else
		  if(token == RD) open--;
		}
	    }
	}
      else
        {
	  fprintf(stderr,
"*** extractcmpts:%d: oops can not process token\n", lineno);
          exit(1);
	}
    }
  
  printf("%s\n", buffer);
}

static void tab(int t)
{
  int i;
  for(i=0; i<t; i++)
    printf("  ");
}

static void printall()
{
  int open = 0, depth = 0;
  while(1)
    {
      int token = next_token();
      if(token<0) return;
      if(token == WORD)
        {
	  tab(depth);
	  printf("%s\n", buffer);
	}
      else
      if(token == LD)
        depth++;
      else
      if(token == RD)
        {
	  depth--;
	  if(depth<0)
	    {
	      fprintf(stderr,
"*** extractcmpts:%d: too much closing `}'\n", lineno);
              exit(1);
	    }
        }
      else
      if(token == KOMMA)
        continue;
      else
        {
	  fprintf(stderr,
"*** extractcmpts:%d: unknown token\n", lineno);
          exit(1);
	}
    }
}

main(int argc, char ** argv)
{
  if(argc>2) usage();

  if(argc==2) getonlyone(argv[1]);
  else printall();

  exit(0);
}
