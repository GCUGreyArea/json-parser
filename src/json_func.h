#ifndef JSON_HEADER
#define JSON_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Types of json itmes that are legal 
 * 
 */
enum json_type {
    JSON_OBJECT,
    JSON_LIST,
    JSON_STRING,
    JSON_FLOAT,
    JSON_INT,
    JSON_BOOL,
    JSON_NULL
};

/**
 * @brief A pair of values that identifies the place in the string the JSON item
 * comes from 
 *
 */
struct json_place {
    unsigned int start;
    unsigned int length;
};

/**
 * @brief Structure to save a JSON token into
 * 
 */
struct json_token {
    struct json_place place; //! Statrt and length in the string
    enum json_type type;     //! Type of item
    char * string;           //! Copy of the string
    int index;               //! Index in the array
    int upper;               //! Upper object or list this belongs to
    int size;                //! If containg obj then number os subs, otherwise -1
    unsigned int depth;      //! Depth of the path for this value
    unsigned int hash;       //! Hash value as the current path 
    bool key;                //! If this is a key or a value
};

/**
 * @brief A list to hold all parsed json tokens
 * @todo Fix the issue with place not functioning correctly so that we don't
 * need to copy the string
 *
 */

struct json_token_list;

/**
 * @brief Create a new JSON parsed object list
 * 
 * @param size 
 * @return struct json_token_list* 
 */
struct json_token_list * make_tokens(unsigned int size);

/**
 * @brief Get the next token object from the JSON tokens list
 * 
 * @param list 
 * @return struct json_token* 
 */
struct json_token * get_next_token(struct json_token_list * list);
struct json_token * get_current_token(struct json_token_list * list);

/**
 * @brief Create new memory and copy a string into it  
 *
 * @param str 
 * @return char* 
 */
char * copy_string(char * str);
char * copy_token_string(struct json_token * t, char * string);
char * get_token_type_string(enum json_type t);
struct json_token * get_token(struct json_token_list * list, unsigned int idx);
unsigned int get_token_list_size(struct json_token_list * list);
void reset_tokens(struct json_token_list * list);

/**
 * @brief Print an item from the list
 * 
 * @param t 
 * @param str 
 */
void print_item(struct json_token * t, char * str);

/**
 * @brief Iterate through all the items in the parsed list and print them out
 *
 * @param list 
 * @param str 
 */
void iterate_list(struct json_token_list * list, char * str);

/**
 * @brief Free up memory used by the list
 * 
 * @param list 
 */
void delete_list(struct json_token_list * list);

/**
 * @brief Stack that we can use to keep track of containing objects 
 *
 * @param size 
 * @return struct json_token_list* 
 */

struct stack;

/**
 * @brief Make a new stack
 * 
 */

struct stack * make_stack(unsigned int size);
void push(struct stack * s, unsigned int i);
unsigned int pop(struct stack * s);
unsigned int elelments(struct stack * s);
void reset_stack(struct stack * s);
void delete_stack(struct stack * s);
void parse_string_bison(const char* in);
void init_parser(unsigned int size);
void free_parser();

#ifdef __cplusplus
}
#endif


#endif//JSON_HEADER