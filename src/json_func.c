#include "json_func.h"

/**
 * @brief A list to hold all parsed json tokens
 * @todo Fix the issue with place not functioning correctly so that we don't
 * need to copy the string
 *
 */
struct json_token_list
{
    struct json_token *list;
    unsigned int size;
    unsigned int idx;
};

/**
 * @brief Create a new JSON parsed object list
 *
 * @param size
 * @return struct json_token_list*
 */
struct json_token_list *make_tokens(unsigned int size)
{
    struct json_token_list *list = (struct json_token_list *)malloc(sizeof(struct json_token_list));

    if (list == NULL)
    {
        printf("Memory allocation failure in make_tokens\n");
        abort();
    }

    list->list = (struct json_token *)malloc(sizeof(struct json_token) * size);
    if (list->list == NULL)
    {
        printf("Memory allocation failure in make_tokens\n");
        abort();
    }

    list->idx = 0;
    list->size = size;

    return list;
}

/**
 * @brief Get the next token object from the JSON tokens list
 *
 * @param list
 * @return struct json_token*
 */
struct json_token *get_next_token(struct json_token_list *list)
{
    if (list->idx >= list->size)
    {
        struct json_token *new_list = (struct json_token *) realloc(list->list, sizeof(struct json_token) * (list->size * 2));
        if (new_list == NULL)
        {
            printf("Memory allocation failure in get_next_token\n");
            abort();
        }
        
        list->list = new_list;
        list->size = (list->size * 2);
    }

    list->list[list->idx].index = list->idx;
    list->list[list->idx].size = 0;
    list->list[list->idx].upper = -1;
    list->list[list->idx].string = NULL;

    struct json_token * next = &(list->list[list->idx]);
    list->idx++;

    return next;
}

struct json_token *get_current_token(struct json_token_list *list)
{
    if (list && list->idx)
    {
        struct json_token * tk = &(list->list[list->idx-1]);
        return tk;
    }

    return NULL;
}

/**
 * @brief Get the token object at index
 *
 * @param list
 * @return struct json_token*
 */
struct json_token *get_token(struct json_token_list *list, unsigned int idx)
{
    if (list)
    {
        struct json_token * tk = &(list->list[idx]);
        return tk;

    }

    return NULL;
}

/**
 * @brief Get the token list object
 *
 * @param list
 * @return struct json_token*
 */
struct json_token *get_token_list(struct json_token_list *list)
{
    if (list)
    {
        return list->list;
    }

    return NULL;
}

unsigned int get_token_list_size(struct json_token_list *list)
{
    if (list)
    {
        return list->idx;
    }

    return 0;
}

/**
 * @brief Create new memory and copy a string into it
 *
 * @param str
 * @return char*
 */
char *copy_string(char *str)
{
    unsigned int len = strlen(str);
    char *s = (char *)malloc(len + 1);
    if (s == NULL)
    {
        printf("memory allocation failure in print_item(struct json_token * t, char * str)\n");
        abort();
    }

    strcpy(s, str);
    s[len] = '\0';

    return s;
}

char *get_token_type_string(enum json_type t)
{
    switch (t)
    {
    case JSON_FLOAT:
        return "JSON_FLOAT";
    case JSON_STRING:
        return "JSON_STRING";
    case JSON_INT:
        return "JSON_INT";
    case JSON_OBJECT:
        return "JSON_OBJECT";
    case JSON_LIST:
        return "JSON_LIST";
    case JSON_BOOL: 
        return "JSON_BOOL";
    default:
        break;
    }

    return "UNKNONWN_TYPE";
}

char *copy_token_string(struct json_token *t, char *string)
{
    int start = t->place.start;
    int length = t->place.length;

    char *str = (char *)malloc(length + 1);
    if (str == NULL)
    {
        printf("Memory allocation failure in copy_token_string(struct json_token * t, char * string)\n");
        abort();
    }

    memcpy(str, string + start, length);
    str[length] = '\0';

    return str;
}

/**
 * @brief Print an item from the list
 *
 * @param t
 * @param str
 */
inline static char * true_false(char* b) {
    if(b[0] == 't' && b[1] == 'r' && b[2] == 'u' && b[3] == 'e') {
        return "true";
    }
    else if(b[0] == 'f' && b[1] == 'a' && b[2] == 'l' && b[3] == 's' && b[4] == 'e') {
        return "false";
    }

    return "unknown";
}

void print_item(struct json_token *t, char *str)
{
    if (t->string == NULL)
    {
        t->string = copy_token_string(t, str);
    }
    switch (t->type)
    {
    case JSON_FLOAT:
        printf("float: %f @ %d\n", atof(t->string), t->index);
        break;
    case JSON_STRING:
        printf("string: %s @ %d\n", t->string, t->index);
        break;
    case JSON_BOOL:
        printf("bool: %s @ %d\n", true_false(str + t->place.start), t->index);
        break;
    case JSON_NULL:
        printf("bool: %s @ %d\n", "null", t->index);
        break;
    case JSON_OBJECT:
        printf("object @ %d, size %d\n", t->index, t->size);
        break;
    case JSON_LIST:
        printf("list @ %d, size %d\n", t->index, t->size);
        break;
    default:
        printf("Unknown type\n");
    }
}

/**
 * @brief Iterate through all the items in the parsed list and print them out
 *
 * @param list
 * @param str
 */
void iterate_list(struct json_token_list *list, char *str)
{
    for (unsigned int i = 0; i < list->idx; i++)
    {
        print_item(&list->list[i], str);
        if (list->list[i].upper != -1)
        {
            printf("\tParent: ");
            print_item(&list->list[list->list[i].upper], str);
        }
    }
}

/**
 * @brief Free up memory used by the list
 *
 * @param list
 */
void delete_list(struct json_token_list *list)
{
    for (unsigned int i = 0; i < list->idx; i++)
    {
        if (list->list[i].string)
        {
            free(list->list[i].string);
        }
    }

    free(list->list);
    free(list);
}

void reset_tokens(struct json_token_list * list) {
    if(list) {
        list->idx = 0;
    }
}

/**
 * @brief Stack that we can use to keep track of containing objects
 *
 * @param size
 * @return struct json_token_list*
 */

struct container {
    unsigned int value;
};

struct stack
{
    struct container *ar;
    unsigned int size;
    unsigned int idx;
};

/**
 * @brief Make a new stack
 *
 */

struct stack *make_stack(unsigned int size)
{
    struct stack *s = (struct stack *)malloc(sizeof(struct stack));
    if (s == NULL)
    {
        printf("memory allocation failure in make_stack(unsigned int size)\n");
        abort();
    }
    s->ar = malloc(sizeof(struct container) * size);
    if (s->ar == NULL)
    {
        printf("memory allocation failure in make_stack(unsigned int size)\n");
        abort();
    }

    s->idx = 0;
    s->size = size;

    return s;
}

void push(struct stack *s, unsigned int i)
{
    if (s)
    {
        if (s->idx == s->size)
        {
            struct container *new_s = realloc(s->ar, sizeof(struct container) * s->size * 2);
            if (new_s == NULL)
            {
                printf("memory allocation failure in push(struct stack * s, unsigned long long int i)\n");
                abort();
            }
            
            s->ar = new_s;
            s->size = s->size * 2;
        }

        s->ar[s->idx++].value = i;
    }
}

unsigned int pop(struct stack *s)
{
    if (s)
    {
        if (s->idx != 0)
        {
            void * i = s->ar[--s->idx].value;
            return i;
        }
    }

    printf("no ellecment to return on stack pop(struct stack * s)\n");
    abort();
}

unsigned int elelments(struct stack *s)
{
    if (s)
    {
        return s->idx;
    }

    printf("Bad stack\n");
    abort();
}

void reset_stack(struct stack * s) {
    if(s) {
        s->idx=0;
    }
}


void delete_stack(struct stack * s) {
    if(s) {
        free(s->ar);
        free(s);
    }
}
