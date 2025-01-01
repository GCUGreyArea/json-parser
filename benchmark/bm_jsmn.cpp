#include <benchmark/benchmark.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn.h"
#include "json_func.h"


static char * read_file(const char * name) {
    char * buffer = NULL;
    long length;
    FILE * f = fopen (name, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = (char*) malloc (length + 1);
        if (buffer)
        {
            fread (buffer, 1, length, f);
            buffer[length] = '\0';
        }
        fclose (f);
    }

    return buffer;
}


static void BM_RunJSMNCParser(benchmark::State &state) {

    char * buffer = read_file("/home/barry/workspace/json/test/resources/large-file.json");
    if (buffer == nullptr) {
        throw std::runtime_error("failed to open file");
    }

    jsmntok_t tokens[4096];
    jsmn_parser p;

    for (auto _ : state) {
        jsmn_init(&p);
        jsmn_parse(&p,buffer,strlen(buffer),tokens,4096);
    }

    free(buffer);

}

extern char * string;
extern struct json_token_list * list;
extern struct stack * token_stack;
extern int poped;
extern int pushed;
extern int depth;
extern int count;


static void BM_RunJSONParser(benchmark::State &state) {

    string = read_file("/home/barry/workspace/json/test/resources/large-file.json");
    if (string == nullptr) {
        throw std::runtime_error("failed to open file");
    }
    
    list = make_tokens(1024);
    token_stack = make_stack(1024);

    for (auto _ : state) {
        parse_string_bison(string);
        reset_tokens(list);
        reset_stack(token_stack);
    }

    delete_list(list);
    delete_stack(token_stack);

    free(string);

}

BENCHMARK(BM_RunJSMNCParser);
BENCHMARK(BM_RunJSONParser);
