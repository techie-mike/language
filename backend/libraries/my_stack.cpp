//
// Created by texnar on 07/10/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "my_stack.h"
#define LEN_STACK (unsigned int)(&stack->st_bird2 -  &stack->st_bird1)
#define LEN_DATA  (unsigned int)(&stack->buf_bird2 -  &stack->buf_bird1)

Elem_t StackPop(Stack_t* stack, Elem_t* num) {
    StackVerific(stack);

//    if (stack->size < 0) *n_err = 1;
    assert(stack->size != 0);

    *num = stack->data[--stack->size];

    stack->data[stack->size] = POISON;

    AutoLenghtDecrease(stack);

#if LEVEL_VERIFIC >= 2
    NewHash(stack);
#endif

    StackVerific(stack);
    return 0;// write number of error (in future)
}

void NewPointInStack(Stack_t* stack){
    assert(stack != nullptr);
    assert(stack->buffer != nullptr);

    stack->data = (Elem_t*) ((char*)stack->buffer + sizeof(Bird_t));
    assert(stack->data != nullptr);

#if LEVEL_VERIFIC != 0
    stack->buf_bird1 = (Bird_t*) stack->buffer;
    stack->buf_bird2 = (Bird_t*) ((char*)stack->buffer + stack->length * sizeof(Elem_t) + sizeof(Bird_t));

    assert(stack->buf_bird1 != nullptr);
    assert(stack->buf_bird2 != nullptr);

    *stack->buf_bird1 = 0xDEADBEEF;
    *stack->buf_bird2 = *stack->buf_bird1;
#endif
}

int StackPush(Stack_t* stack,  Elem_t value) {
    StackVerific(stack);
//    printf("hash_data %u\n", &stack->hash_data);
//    printf("hash_data %u\n", MurmurHashData(stack));

    AutoLenghtIncrease(stack);

    if (stack->size > stack->length - 1) return false;
    stack->data[stack->size++] = value;
//
//    printf("Value - %d\n", value);
//    printf("Stack - %d", stack->data[stack->size - 1]);

#if LEVEL_VERIFIC >= 2
    NewHash(stack);
#endif
    StackVerific(stack);

    //StackDump(stack, "Debug", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    return 0; //write error (in future)
}

#if LEVEL_VERIFIC >= 2
void NewHash(Stack_t *stack) {
    assert(stack != nullptr);
    assert(stack->buffer != nullptr);

    stack->hash_data  = MurmurHash(stack, LEN_DATA, &stack->hash_data);
    stack->hash_stack = MurmurHash(stack, LEN_STACK, &stack->hash_stack);
}
#endif

void StackVerific(Stack_t *stack) {
    assert(stack != nullptr);
    assert(stack->buffer != nullptr);
#if  LEVEL_VERIFIC >= 2
    assert(stack->hash_data  == MurmurHash(stack, LEN_DATA, &stack->hash_data));
    assert(stack->hash_stack == MurmurHash(stack, LEN_STACK, &stack->hash_stack));
#endif

    //StackDump(stack, "Debug", __FILE__, __LINE__, __PRETTY_FUNCTION__);
#if LEVEL_VERIFIC != 0
    if (stack->st_bird1 != stack->st_bird2){
        printf("Error in birds of stack\nI refuse to work!\n");
        assert(stack->st_bird1 == stack->st_bird2 == 0xDEADBEEF);
    }

    if (*stack->buf_bird1 != *stack->buf_bird2){
        printf("Error in birds of stack\nI refuse to work!\n");
        assert(*stack->buf_bird1 == *stack->buf_bird2 == 0xDEADBEEF);
    }
#endif
    if (stack->size < 0){
        printf("Size of stack < 0\nI refuse to work!\n");
        assert(stack->size < 0);
    }

    for (int i = stack->size; i < stack->length; i++){
        if (stack->data[i] != POISON){
            printf("Error in poison number\nI refuse to work!\n");
            assert(stack->data[i] == POISON);
        }
    }
    //printf("hash_data - %u\n", MyHash(stack));
}


bool StackInit(struct Stack_t* stack, int DEFAULT_LENGHT ,int DEFAULT_HYSTER) {
    assert(stack != nullptr);

#if LEVEL_VERIFIC != 0
    stack->st_bird1 = 0xDEADBEEF;
    stack->st_bird2 = stack->st_bird1;
#endif
    stack->size = 0;
    //stack->last_max = 0;
    stack->hyster = DEFAULT_HYSTER;
    stack->length = DEFAULT_LENGHT;

    stack->buffer = (void*) calloc(stack->length * sizeof(Elem_t) + 2 * sizeof(Bird_t), sizeof(char));
    assert(stack->buffer != nullptr);
    NewPointInStack(stack);

    // 'POISON' it is poisonous value in my stack //
    for (int i = stack->size; i < stack->length; ++i) {
        stack->data[i] = POISON;
    }
#if LEVEL_VERIFIC >= 2
    stack->hash_data  = MurmurHash(stack, LEN_DATA, &stack->hash_data);
    stack->hash_stack = MurmurHash(stack, LEN_STACK, &stack->hash_stack);
    return true;
#endif
}

#if LEVEL_VERIFIC >= 2
unsigned int MurmurHash (Stack_t* stack, unsigned int len, unsigned int* point_hash)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;
    const unsigned char * data = 0;

    if (point_hash == &stack->hash_stack){
        data = (const unsigned char *) &stack->st_bird1;
    }
    if (point_hash == &stack->hash_data){
        data = (const unsigned char *) &stack->buf_bird1;
    }

    //const unsigned char * data = (const unsigned char *) &stack->st_bird1;
    //unsigned int len = stack->st_bird2 - stack->st_bird1 + sizeof(stack->st_bird1);
    unsigned int h = seed ^ len;

    unsigned int last_hash = stack->hash_stack;
    stack->hash_stack  = 0;
    // const unsigned char * data = (const unsigned char *)key;
    unsigned int k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    stack->hash_stack = last_hash;
    return h;
}
#endif

int AutoLenghtIncrease(Stack_t *stack) {
    assert(stack != nullptr);
    assert(stack->buffer != nullptr);

    if ((stack->length) == stack->size) {
        void *point = nullptr;

        //printf("%lu", FACTOR_DYNAMIC * stack->length * sizeof(Elem_t) + FACTOR_DYNAMIC * sizeof(Bird_t));
        point = realloc((stack->buffer), (FACTOR_DYNAMIC * stack->length * sizeof(Elem_t) + FACTOR_DYNAMIC * sizeof(Bird_t)));
        if (point == nullptr) {
            printf("Error in realloc()\n");
            assert(point != nullptr);
        }
        stack->buffer = point;
        stack->length *= FACTOR_DYNAMIC;

        NewPointInStack(stack);

        for (int i = stack->size; i < stack->length; ++i) {
            stack->data[i] = POISON;
        }
        //stack->last_max;
    }
    return 0;
}

void StackDump(Stack_t *stack, const char welcome[], const char name_file[], const int line_prog, const char which_func[]){
    assert(stack != nullptr);

    printf("Dump (%s) From %s (%d) %s\n", welcome, name_file, line_prog, which_func);
    printf("\tStack[%p]\n", stack);
    printf("\t{\n");

#if LEVEL_VERIFIC != 0
    printf("\tst_bird1 = %llX\n", stack->st_bird1);
#endif

    printf("\tsize = %d\n", stack->size);
    printf("\tdata [%d][%p]\n", stack->length, &stack->data);
    printf("\t\t{\n");
    for (int i = 0; i < stack->length; ++i) {
        if (i < stack->size){
            printf("\t\t*[%d] = %d\n", i, stack->data[i]);

        } else{
            printf("\t\t [%d] = %d(POISON)\n", i, stack->data[i]);
        }
    }
    printf("\t\t}\n");
    //printf("\t\tError = %d\n", err);

#if LEVEL_VERIFIC != 0
    printf("\t\tst_bird2 = %llX\n", stack->st_bird2);
#endif

#if LEVEL_VERIFIC >= 2
    printf("\t\thash_stack = %u\n", stack->hash_stack);
    printf("\t\thash_data = %u\n", stack->hash_data);
#endif
    printf("\t}\n");
}

int AutoLenghtDecrease(Stack_t *stack) {
    assert(stack != nullptr);
    assert(stack->buffer != nullptr);

    if ((stack->length / FACTOR_DYNAMIC - stack->hyster) >=  stack->size){
        void *point = nullptr;

        point = realloc(stack->buffer, (stack->length / FACTOR_DYNAMIC * sizeof(Elem_t) + FACTOR_DYNAMIC * sizeof(Bird_t)));
        if (point == nullptr) {
            printf("Error in realloc()\n");
            assert(point != nullptr);
        }

        stack->buffer = point;
        stack->length /= FACTOR_DYNAMIC;

        NewPointInStack(stack);
    }
    return 0;
}

void StackClose(Stack_t* stack) {
    free(stack->buffer);
    stack->buffer = nullptr;
    stack->length = 1;
    stack->size = 0;
    stack->hyster = 0;
}

#undef LEN_STACK
#undef LEN_DATA
