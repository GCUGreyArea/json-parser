%{
    #include <stdio.h>
    #include <string.h>
	  #include <stdlib.h>
	  #include <string.h>
    
    #include "json_func.h"
	  #include "json.tab.h"

    extern int yylex();
    int yyerror(char * er);

    extern unsigned int pos;
    extern unsigned int line;
    extern unsigned int col;
    extern struct json_token_list * list;
    extern struct stack * token_stack;

    extern void debug_list();

    #define YYMAXDEPTH  100000
    #define YYINITDEPTH 8000
%}

%token  STRING;
%token  SEP;
%token  COMMA;
%token  OPEN_OBJ;
%token  CLOSE_OBJ;
%token  OPEN_LST;
%token  CLOSE_LST;
%token  FLOAT;
%token  INT;
%token  BOOL_TRUE;
%token  BOOL_FALSE;
%token  NULL_VALUE;

%%

/* bison needs the top most rule to come first */
json_obj: 
    list {/*printf("parse: LIST @ %d\n", pos);*/}
  | obj  {/*printf("parse: OBJ @ %d\n", pos);*/ }
;

list:
    OPEN_LST value_lst CLOSE_LST {/*printf("parse: OPEN_LST value_lst CLOSE_LST\n");*/}
 |  OPEN_LST CLOSE_LST           {/*printf("parse: OPEN_LST CLOSE_LST\n");*/}
;

obj:
    OPEN_OBJ obj_value_lst CLOSE_OBJ {/*printf("parse: OPEN_OBJ value_lst CLOSE_OBJ\n");*/}
  | OPEN_OBJ CLOSE_OBJ               {/*printf("parse: OPEN_OBJ CLOSE_OBJ\n");*/} 
;

obj_value_lst:
    kv_pair                     {/*printf("parse: kv_pair\n");*/}
  | kv_pair COMMA obj_value_lst {/*printf("parse: kv_pair COMMA obj_value_lst\n");*/}
  ;

kv_pair:
  STRING SEP value {/*printf("parse: STRING SEP value\n");*/}
  ;

value:
    obj        {/*printf("parse: value: obj\n");*/}
  | list       {/*printf("parse: value: list\n");*/}
  | INT        {/*printf("parse: value: INT\n");*/}
  | FLOAT      {/*printf("parse: value: FLOAT\n");*/}
  | STRING     {/*printf("parse: value: STRING\n");*/}        
  | BOOL_FALSE {}
  | BOOL_TRUE  {}
  | NULL_VALUE {}               
  ;

value_lst:
    INT COMMA value_lst   {/*printf("INT COMMA value_lst\n");*/}
  | FLOAT COMMA value_lst {/*printf("FLOAT COMMA value_lst\n");*/}
  | obj COMMA value_lst   {/*printf("obj COMMA value_lst\n");*/}
  | list COMMA value_lst  {/*printf("OPEN_OBJ CLOSE_OBJ\n");*/}
  | list                  {/*printf("list\n");*/}
  | obj                   {/*printf("obj\n");*/}
  | FLOAT                 {/*printf("FLOAT\n");*/}
  | INT                   {/*printf("INT\n");*/}
  | BOOL_FALSE            {}
  | BOOL_TRUE             {}
  | NULL_VALUE            {}
  ;

%% 

int yyerror(char * er) {
    printf("Error: %s at line %d -> pos %d\n", er, line, col);
    printf("YYINITDEPTH=%d\n",YYINITDEPTH);
    return -1;
}


/* Declarations */
void set_input_string(const char* in);
void end_lexical_scan(void);

/* This function parses a string using yyparse. 
   Note that this is not reentrant */
void parse_string_bison(const char* in) { 

  reset_tokens(list);
  reset_stack(token_stack);

  set_input_string(in);
  int rv = yyparse();
  end_lexical_scan();
  if(rv != 0) {
      exit(1);
  }
}

