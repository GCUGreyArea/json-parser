
#include "json_func.h"
#include <signal.h>

extern char * string;
extern struct json_token_list * list;
extern struct stack * token_stack;
extern int poped;
extern int pushed;
extern int depth;
extern int count;

void sigint_handler() {
    printf("pushed: %d, poped: %d, depth: %d\n", pushed, poped, depth);
    printf("list->idx=%d\n",get_token_list_size(list));
}

static char * read_file(char * name) {
    char * buffer = NULL;
    long length;
    FILE * f = fopen (name, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length + 1);
        if (buffer)
        {
            fread (buffer, 1, length, f);
            buffer[length] = '\0';
        }
        fclose (f);
    }

    return buffer;
}


void main(int argc, char** argv) {
    signal(SIGINT,sigint_handler);  
    signal(SIGSEGV,sigint_handler);

    printf("Running\n");
    if(argc == 1) {
        printf("Please supply a JSON string to parse\n");
        exit(1);
    }

    if(argc == 3 && strcmp(argv[1],"-f") == 0) {
        string = read_file(argv[2]);
        if(string) {
            printf("made string\n");

            list = make_tokens(1024);
            token_stack = make_stack(1024);
            printf("made token_stack\n");
            parse_string_bison(string);
            printf("parsed string\n");
            iterate_list(list,string);
            printf("Done...\n");
            delete_list(list);

            free(string);        
        }

        exit(1);
    }
    else {
        string = argv[1];
        parse_string_bison(string);
    }
}
