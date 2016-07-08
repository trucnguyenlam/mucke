#ifndef _pterm_h_INCLUDED
#define _pterm_h_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pterm_Number_data { int value; } pterm_Number_data; 
typedef struct pterm_Ident_data { char * name; } pterm_Ident_data;
typedef struct pterm_Keyword_data { int token; } pterm_Keyword_data;

struct _pterm;
typedef struct pterm_List_data {
  struct _pterm* head;
  struct _pterm* tail;
} pterm_List_data;

typedef union pterm_data {
  pterm_Number_data	asNumber;
  pterm_Keyword_data	asKeyword;
  pterm_Ident_data	asIdent;
  pterm_List_data	asList;
} pterm_data;
  
typedef struct _pterm {
  int info;
  int lineno;
  struct _pterm* next;
  pterm_data data;
} _pterm, *pterm;

/* constructors */
extern pterm pterm_create_Number(int);
extern pterm pterm_create_Ident(const char*);
extern pterm pterm_create_Keyword(int);
extern pterm pterm_cons(pterm,pterm);

/* destructor */
extern void pterm_delete_all();

extern pterm pterm_cdr(pterm);
extern pterm pterm_car(pterm);

extern int pterm_isList(pterm);
extern int pterm_isNumber(pterm);
extern int pterm_isIdent(pterm);
extern int pterm_isKeyword(pterm);

extern int pterm_length(pterm);

extern int pterm_value(pterm);
extern int pterm_token(pterm);
extern char* pterm_name(pterm);
extern int pterm_line(pterm);

extern void pterm_print(
  pterm,
  const char* (*)(int),
  void (*)(const char *),
  void (*)(int)
);

extern void pterm_install_print_routines(
  void (*)(const char *),
  void (*)(int)
);

#ifdef __cplusplus
};
#endif

#endif
