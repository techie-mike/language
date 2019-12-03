//
// Created by texnar on 03/12/2019.
//

#ifndef FRONTEND_FUNCTION_H
#define FRONTEND_FUNCTION_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

const int TYPE_NUMBER = 1, TYPE_STRING = 2, TYPE_SYMBOLS = 3;

const int LENGTH_NAME_TOKEN = 100;

typedef int token_t;
typedef int token_names_t;

struct one_token {
    char* name;
    double value;
    int type;
    int line;
};

struct Tokens {
    one_token* data;

    void lexicalAnalysis (char** text);
    void readFile (const char* name_file);
    explicit Tokens (token_names_t DEFAULT_LENGTH = 50, token_names_t DEFAULT_LENGTH_NAMES = 200);
    ~Tokens();

private:
    char* all_names_;
    token_t length_token_;
    token_t size_token_;
    token_names_t length_names_;
    token_names_t size_names_;

    int read_line_;

    void dump ();

    int nummemchr (char* memptr, int val, size_t num_block);
    int lexicalAnalysisWriteNumber (char** text);
    int lexicalAnalysisWriteString (char** text);
    int lexicalAnalysisWriteSymbols (char** text);

    void createToken (char* name, double number, int type, int line);

    void autoLengthNamesIncrease (int factor = 2);
    void autoLengthTokenIncrease (int factor = 2);
};


#endif //FRONTEND_FUNCTION_H
