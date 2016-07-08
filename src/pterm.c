/* This is a C file */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
#include <signal.h>
#include <unistd.h>
#endif

#include "pterm.h"

#define pterm_Number_num	1
#define pterm_Ident_num		2
#define pterm_Keyword_num	3
#define pterm_List_num		4

extern int yylineno;

static void (*print_str)(const char *) = 0;
static void (*print_int)(int) = 0;

void pterm_install_print_routines(
  void (*ps)(const char *),
  void (*pi)(int)
)
{
  print_str = ps;
  print_int = pi;
}

static void pterm_do_nothing(int i)
{
  if(print_str)
    {
      print_str("*** fatal error in pterm at line ");
      print_int(yylineno);
      print_str("\n");
    }
  else
    {
      print_str("*** fatal error in pterm\n");
    }
# ifdef DEBUG
    kill(getpid(), SIGSEGV);
# endif
  exit(i);
}

static void(*err_handler)(int) = &pterm_do_nothing;

static pterm pterm_node_list = 0;

pterm pterm_new_node()
{
  pterm new = (pterm)malloc(sizeof(struct _pterm));
  if(!new) { err_handler(1); }
  new->next = pterm_node_list;
  new->lineno = yylineno;
  pterm_node_list = new;
  return new;
}

int pterm_line(pterm p)
{
  if(!p) err_handler(1);
  return p->lineno;
}

pterm pterm_create_Number(int i)
{
  pterm new = pterm_new_node(); 
  new->info = pterm_Number_num;
  new->data.asNumber.value = i;
  return new;
}

pterm pterm_create_Ident(const char * s)
{
  pterm new = pterm_new_node(); 
  new->info = pterm_Ident_num;
  if(!s) s = "";
  if(!(new->data.asIdent.name = (char*) malloc(strlen(s)+1)))
    err_handler(1);
  strcpy(new->data.asIdent.name, s);
  return new;
}

pterm pterm_create_Keyword(int t)
{
  pterm new = pterm_new_node();
  new->info = pterm_Keyword_num;
  new->data.asKeyword.token = t;
  return new;
}

pterm pterm_cons(pterm h, pterm t)
{
  pterm new = pterm_new_node();
  new->info = pterm_List_num;
  new->data.asList.head = h;
  new->data.asList.tail = t;
  return new;
}

void pterm_delete_all()
{
  pterm p = pterm_node_list, tmp;

#ifdef DEBUG
  if(print_str != 0)
    (*print_str)("pterm_delete_all called\n");

  /*
      for(tmp = p; tmp; tmp = tmp -> next)
        {
          (*print_int)( (int)tmp );
          (*print_str)( "\n" );
        }
  */

#endif

  pterm_node_list=0;

  for(; p; p=tmp)
    {
      tmp = p->next;
      // free(p);
    }
}

pterm pterm_car(pterm p)
{
#ifdef DEBUG
  if(!p) err_handler(1);
#endif
  return p->data.asList.head;
}

pterm pterm_cdr(pterm p)
{
#ifdef DEBUG
  if(!p) err_handler(1);
#endif
  return p->data.asList.tail;
}

int pterm_isList(pterm p)
{
  return p == 0 || p->info == pterm_List_num;
}

int pterm_isNumber(pterm p)
{
  return p ? p->info == pterm_Number_num : 0;
}

int pterm_isIdent(pterm p)
{
  return p ? p->info == pterm_Ident_num : 0;
}

int pterm_isKeyword(pterm p)
{
  return p ? p->info == pterm_Keyword_num : 0;
}

int pterm_value(pterm p)
{
#ifdef DEBUG
  if(!pterm_isNumber(p)) err_handler(1);
#endif
  return p->data.asNumber.value;
}

int pterm_length(pterm p)
{
  int i;
  pterm l;
#ifdef DEBUG
  if(!pterm_isList(p)) err_handler(1);
#endif
  for(l=p, i=0; l && pterm_isList(l); l=pterm_cdr(l))
    i++; 
  if(l) i++;
  return i;
}

int pterm_token(pterm p)
{
#ifdef DEBUG
  if(!pterm_isKeyword(p)) err_handler(1);
#endif
  return p->data.asKeyword.token;
}

char * pterm_name(pterm p)
{
#ifdef DEBUG
  if(!pterm_isIdent(p)) err_handler(1);
#endif
  return p->data.asIdent.name;
}

static void pterm_print_tab(int l)
{
  int i;
  for(i=0; i<l; i++) print_str("  ");
}

static const char* (*pterm_token_to_str)(int);

static void pterm_print_aux(pterm p, int l)
{
  pterm_print_tab(l);
  if(p)
    {
      switch(p->info) {
        case pterm_Number_num:
	  print_int(pterm_value(p));
	  print_str("\n");
	  break;
	case pterm_Keyword_num:
	  print_str(pterm_token_to_str(pterm_token(p)));
	  print_str("\n");
	  break;
	case pterm_Ident_num:
	  print_str(pterm_name(p));
	  print_str("\n");
	  break;
	case pterm_List_num:
          print_str("[\n");
          for(; p && pterm_isList(p); p = pterm_cdr(p))
	    pterm_print_aux( pterm_car(p), l+1);
	  if(p)
	    {
	      pterm_print_aux(p, l+1);
	      pterm_print_tab(l);
	      print_str("]*\n");
	    }
	  else
	    {
	      pterm_print_tab(l);
	      print_str("]\n");
	    }
	  break;
	default:
	  break;
      };
    }
  else print_str("[]\n");
}

void pterm_print(
  pterm p,
  const char * (*tok_to_str)(int),
  void (*ps)(const char *),
  void (*pi)(int)
)
{
  pterm_install_print_routines(ps,pi);
  pterm_token_to_str = tok_to_str;
  pterm_print_aux(p,0);
}
