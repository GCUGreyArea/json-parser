#include <gtest/gtest.h>
#include <glog/logging.h>

#include "json_func.h"

TEST(token_list,testBigUpdtes) {
    struct json_token_list * list = make_tokens(10);
    const char * test = "test string";

    for(unsigned int i=0; i<4096*20;i++) {
        auto * n = get_next_token(list);
        auto * c = get_current_token(list);
        c->key = true;
        n->depth = i;
        n->string = (char*) malloc(strlen(test)+1);
        strcpy(n->string,test);
    }


    for(unsigned int i=0; i<4096*20;i++) {
        auto * n = get_token(list,i);
        ASSERT_TRUE(n->key);
        ASSERT_TRUE(n->depth == i);
        ASSERT_STREQ(n->string,test);
    }

    delete_list(list); 
}

