//
// Created by texnar on 03/12/2019.
//

#ifndef FRONTEND_TOKEN_H
#define FRONTEND_TOKEN_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


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

    Tokens (const Tokens& that) = delete;
    const char* name_file_;

    void lexicalAnalysis (char** text);
    void readFile (const char* name_file);
    explicit Tokens (token_names_t DEFAULT_LENGTH = 50, token_names_t DEFAULT_LENGTH_NAMES = 200,
           token_names_t DEFAULT_LENGTH_NAME_TOKEN = 100);
    ~Tokens();

    const int TYPE_NUMBER, TYPE_STRING, TYPE_SYMBOLS;
    const int LENGTH_NAME_TOKEN;

    token_t size_token_;
private:
    char* all_names_;
    token_t length_token_;

    token_names_t length_names_;
    token_names_t size_names_;

    int read_line_;

    void dump ();

    int nummemchr (char* memptr, int val, size_t num_block);
    int lexicalAnalysisWriteNumber (char** text);
    int lexicalAnalysisWriteString (char** text);
    int lexicalAnalysisWriteSymbols (char** text);

    void createToken (char* name, double number, int type, int line);
//    void convertInCommonName ();

    void autoLengthNamesIncrease (int factor = 2);
    void autoLengthTokenIncrease (int factor = 2);
};


#endif //FRONTEND_TOKEN_H
