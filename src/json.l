%{
    /** 
    ** This is a basic lexer for JQ Path strings 
    ** We now need a parser to make sure everything 
    ** is parsed in the correct order and to do error 
    ** checking on input strings.
    **/

    #include <stdio.h>
    #include "json.tab.h"
    #include "json_func.h"
    #include "hash.h"

    unsigned int count = 0;
    unsigned int line = 0;
    unsigned int col = 0;
    unsigned int pos = 0;
    int depth = 0;
    bool key = true;
    char * string = NULL;
    struct json_token_list * list = NULL;
    struct stack * token_stack = NULL;
    unsigned int utk = 0;


    int poped = 0;
    int pushed = 0;


    void parse_string_bison(const char* in);
    void do_object();
    void do_string();
    void do_list();
    void do_float();
    void do_int();
    void do_bool();
    void do_null();
%}

%%

\"(\\.|[^\\"])*\" { pos = count; count += strlen(yytext); do_string(); return STRING; }
:                 { pos = count; count += strlen(yytext); return SEP;}
,                 { pos = count; count += strlen(yytext); return COMMA;}
\{                { pos = count; count += strlen(yytext); do_object(); return OPEN_OBJ;} 
\}                { pos = count; count += strlen(yytext); utk = pop(token_stack); poped++; depth--; return CLOSE_OBJ;}
\[                { pos = count; count += strlen(yytext); do_list(); return OPEN_LST;}
\]                { pos = count; count += strlen(yytext); utk = pop(token_stack); poped++; depth--; return CLOSE_LST;}
[0-9]+\.[0-9]+    { pos = count; count += strlen(yytext); do_float(); return FLOAT;}
[0-9]+            { pos = count; count += strlen(yytext); do_int(); return INT;}
true              { pos = count; count += strlen(yytext); do_bool(); return BOOL_TRUE;}
false             { pos = count; count += strlen(yytext); do_bool(); return BOOL_FALSE;}
null              { pos = count; count += strlen(yytext); do_null(); return NULL_VALUE; }
[\t\r\n]          { count++; line++; col=0;}   /* skip whitespace which is not part of a string */
[ ]               { count++; col++;}
.                 { count++; col++;/* count any characters that are not counted by other rules */}

%%

void do_object() {
    struct json_token * t = get_next_token(list);
    t->place.start = pos;
    t->place.length = 0;
    t->key = false;
    t->place.length = 0;
    t->type = JSON_OBJECT;


    struct json_token * u = get_token(list,utk);
    u->size++;
    t->upper = u->index;
    push(token_stack,utk);
    pushed++;
    
    utk = t->index;

    depth++;
}

void do_list() {
    struct json_token * t = get_next_token(list);

    t->place.start = pos;
    t->place.length = 0;
    t->key = false;
    t->place.length = 0;
    t->type = JSON_LIST;

    struct json_token * u = get_token(list,utk);
    u->size++;
    t->upper = u->index;
    push(token_stack,utk);
    pushed++;
    
    utk = t->index;
}

void do_string() {
    // Assume string has a parent
    struct json_token * c = get_current_token(list);
    unsigned int idx = c->index;
    struct json_token * t = get_next_token(list);
    c = get_token(list,idx);
    if(c) {
        if(c->type == JSON_OBJECT || c->type == JSON_LIST) {
            t->key = true;
        }
        else {
            t->key = false;
        }
    }

    t->place.start = pos;
    t->place.length = strlen(yytext);
    t->type = JSON_STRING;

    struct json_token * u = get_token(list,utk);
    if(u != NULL) {
        u->size++;
        t->upper = u->index;
    }
    
    depth++;
}

void do_int() {
    struct json_token * t = get_next_token(list);
    t->place.start = pos;
    t->place.length = strlen(yytext);
    t->key = false;
    t->type = JSON_INT;
    
    struct json_token * u = get_token(list,utk);
    if(u != NULL) {
        u->size++;
        t->upper = u->index;
    }

}

void do_float() {
    struct json_token * t = get_next_token(list);
    t->place.start = pos;
    t->place.length = strlen(yytext);
    t->key = false;
    t->type = JSON_FLOAT;

    struct json_token * u = get_token(list,utk);
    if(u != NULL) {
        u->size++;
        t->upper = u->index;
    }
}


void do_bool() {
    struct json_token * t = get_next_token(list);
    t->place.start = pos;
    t->place.length = strlen(yytext);
    t->key = false;
    t->type = JSON_BOOL;

    struct json_token * u = get_token(list,utk);
    if(u != NULL) {
        u->size++;
        t->upper = u->index;
    }
}

void do_null() {
    struct json_token * t = get_next_token(list);
    t->place.start = pos;
    t->place.length = strlen(yytext);
    t->key = false;
    t->type = JSON_NULL;
    
    struct json_token * u = get_token(list,utk);
    if(u != NULL) {
        u->size++;
        t->upper = u->index;
    }
}

void parse_string_lex(const char *str) {

    printf("In parse string\n");
    
    if (str == NULL) {return;}

    YY_BUFFER_STATE buffer = yy_scan_string(str);
    yy_switch_to_buffer(buffer);
    int token;
    while ((token = yylex()) != 0) {
        // printf("Token: %d\n", token);
    }
    yy_delete_buffer(buffer);
}

void set_input_string(const char* in) {
    yy_scan_string(in);
}

void end_lexical_scan(void) {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

int yywrap() {
    return(1);
}

void debug_list() {
    iterate_list(list,string);
}
