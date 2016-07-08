#ifndef _Class_h_INCLUDED
#define _Class_h_INCLUDED

class ClassInfo {
  char* _name;
public:
  ClassInfo(char* n) : name(n) { }
  char* name() { return _name; }
};

class ClassWithInfo {
public:
  virtual const ClassInfo* classInfo() const = 0;
  friend int sameClass(const ClassWithInfo& a, const ClassWithInfo& b);
};

#define CLASSINFO_STATIC_MEMBER_NAME _classInfo
#define CLASSINFO_VIRTUAL_METHOD_NAME classInfo
#define CLASSINFO_STATIC_METHOD_NAME staticClassInfo

/*---------------------------------------------------------------------------.
 | The next macro is for the declaration of the static pointer to classInfo  |
 | that should be declared in each Class which is derived from               |
 | ClassWithInfo. You should write a call to this macro in the private part  |
 | of the definition of the derived class.                                   |
 |                                                                           |
 | For inheritance I should mention that if you derive a class B from a      |
 | class A that is a subclass of ClassWithInfo and for which you used these  |
 | macros but for B you didn't then you will get the neat effect that        |
 | B::classInfo is the same as A::classInfo and an object of A has the same  |
 | ClassInfo as an object of B.                                              |
 `---------------------------------------------------------------------------*/
#define CLASSINFO_DECL \
\
  static const ClassInfo* CLASSINFO_STATIC_MEMBER_NAME;\
\
public:\
\
  virtual const ClassInfo* CLASSINFO_VIRTUAL_METHOD_NAME() const\
    { return CLASSINFO_STATIC_MEMBER_NAME; }\
\
  static const ClassInfo* CLASSINFO_STATIC_METHOD_NAME();\
\
private:\
\
  enum { c ## CLASSINFO_DECL_DUMMY }               /* just for the semicolon */

/*---------------------------------------------------------------------------.
 | The next macros should provide the same ClassInformation for an envelope  |
 | class if you do not want to have seperate ClassInfo for it and instead    |
 | use the ClassInfo of the letter class.                                    |
 `---------------------------------------------------------------------------*/
#define CLASSINFO_DECL_from(rep) \
\
  const ClassInfo* CLASSINFO_VIRTUAL_METHOD_NAME()\
    { return rep ?  (rep->CLASSINFO_VIRTUAL_METHOD_NAME()) : 0;}\
\
  enum { c ## CLASSINFO_DECL_from_DUMMY }          /* just for the semicolon */

/*---------------------------------------------------------------------------.
 | here we have the intialization code for the above static member           |
 `---------------------------------------------------------------------------*/
#define CLASSINFO_CODE(c,s) \
\
const ClassInfo* c ## ::CLASSINFO_STATIC_MEMBER_NAME = new ClassInfo(s);\
\
const ClassInfo* c ## ::CLASSINFO_STATIC_METHOD_NAME()\
  { return CLASSINFO_STATIC_MEMBER_NAME; }\
\
enum { c ## CLASSINFO_CODE_DUMMY }                 /* just for the semicolon */

/*---------------------------------------------------------------------------.
 | ATTENTION: Dont't forget the semicolon at the end of the macrocalls       |
 `---------------------------------------------------------------------------*/

#endif
