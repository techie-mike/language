//
// Created by texnar on 07/10/2019.
//

#ifndef STACK_MY_STACK_H

#define STACK_MY_STACK_H

typedef int Elem_t;
typedef unsigned long long Bird_t;

bool StackInit(struct Stack_t* stack, int DEFAULT_LENGHT = 10, int DEFAULT_HYSTER = 3);
int StackPush(Stack_t* stack,  Elem_t value);
int StackPop(Stack_t* stack, Elem_t* num);
unsigned MyHash(Stack_t* stack);
void StackVerific(Stack_t* stack);
void NewPointInStack(Stack_t* stack);
int AutoLenghtIncrease(Stack_t* stack);
int AutoLenghtDecrease(Stack_t* stack);
//unsigned int MurmurHashData (Stack_t* stack, unsigned int len);
unsigned int MurmurHash (Stack_t* stack, unsigned int len, unsigned int* last_hash);
//unsigned int MurmurHashStack (Stack_t* stack, unsigned int len);
void StackDump(Stack_t* stack, const char welcome[], const char name_file[], int line_prog, const char which_func[]);
void StackClose(Stack_t* stack);

#if  LEVEL_VERIFIC >= 2
void NewHash(Stack_t* stack);
#endif

//const int DEFAULT_LENGHT = 10;


struct Stack_t{
#if LEVEL_VERIFIC != 0
    Bird_t st_bird1;
#endif

    Elem_t* data;
    void* buffer; // 'void *' for show that it is not data
#if LEVEL_VERIFIC != 0
    Bird_t* buf_bird1; // or better do with 'void *'
    Bird_t* buf_bird2;
#endif
    int size;
    int length;

//    int last_max;
    int hyster;
#if LEVEL_VERIFIC >= 2
    unsigned int hash_data;
    unsigned int hash_stack;
#endif

#if LEVEL_VERIFIC != 0
    Bird_t st_bird2;
#endif
    //Bird_t *buf_bird2;
};

const Elem_t POISON = -777;
const int FACTOR_DYNAMIC = 2;

#endif //STACK_MY_STACK_H


