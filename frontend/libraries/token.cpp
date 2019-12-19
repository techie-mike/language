//
// Created by texnar on 03/12/2019.
//

#include "token.h"

Tokens::Tokens (token_names_t DEFAULT_LENGTH_TOKENS, token_names_t DEFAULT_LENGTH_NAMES,
        token_names_t DEFAULT_LENGTH_NAME_TOKEN) :
    data ((one_token*) calloc (DEFAULT_LENGTH_TOKENS, sizeof(one_token))),
    length_token_ (DEFAULT_LENGTH_TOKENS),
    length_names_ (DEFAULT_LENGTH_NAMES),
    all_names_ ((char*) calloc(DEFAULT_LENGTH_NAMES, sizeof(char))),
    size_token_ (0),
    size_names_ (0),
    read_line_ (1),
    TYPE_NUMBER (1),
    TYPE_STRING (2),
    TYPE_SYMBOLS (3),
    LENGTH_NAME_TOKEN (DEFAULT_LENGTH_NAME_TOKEN),
    name_file_ (nullptr)
{

}

Tokens::~Tokens() {
    free (data);
    free (all_names_);
}

void Tokens::lexicalAnalysis(char **text) {
    int num_read = 1;

    while (num_read) {
        num_read = lexicalAnalysisWriteNumber (text);

        if (!num_read) {
            num_read = lexicalAnalysisWriteString (text);

            if (!num_read)
                num_read = lexicalAnalysisWriteSymbols (text);
        }
//        dump();
    }
//    dump();
    createToken((char*) "\0", 0, TYPE_STRING, read_line_);

}

void Tokens::createToken (char *name, double number, int type, int line) {
    autoLengthNamesIncrease ();
    autoLengthTokenIncrease ();

    token_t index = size_token_;

    data[index].name = all_names_ + size_names_;
    strcpy (all_names_ + size_names_, name);
    size_names_ += strlen (name) + 1;

//    printf("%s \n", data[index].name);
    data[index].value = number;
    data[index].type = type;
    data[index].line = line;
    size_token_++;
}

void Tokens::autoLengthNamesIncrease (int factor) {
    if (size_names_ + LENGTH_NAME_TOKEN >= length_names_) {
        token_names_t last_length_names = length_names_;
        length_names_ *= factor;
        char* new_names = (char*) calloc (length_names_, sizeof(char));

        if (new_names) {
            for (int i = 0; i < last_length_names; i++)
                new_names[i] = all_names_[i];

            for (int i = 0; i < size_token_; i++) {
                if (data[i].name != nullptr)
                    data[i].name = data[i].name - all_names_ + new_names;
            }

            free(all_names_);
            all_names_ = new_names;

        } else
            printf("Error in new_address\n");
    }

}

void Tokens::autoLengthTokenIncrease (int factor) {
    if (size_token_ + 50 >= length_token_) {
        length_token_ *= factor;
        data = (one_token *) realloc (data, length_token_ * sizeof(one_token));
//        one_token* new_data = (one_token *) calloc (length_token_, sizeof(data[0]));
//        if (new_data){
//            for (int i = 0; i < size_token_; i++)
//                new_data[i] = data[i];
//            for (int i = size_token_; i < length_token_; i++)
//                data[i] = {};
//            free (data);
//            data = new_data;
//            }
        if (data)
            for (int i = size_token_; i < length_token_; i++)
                data[i] = {};

        else
            printf("Error in new_address of autoLengthNamesIncrease\n");
    }
}

int Tokens::lexicalAnalysisWriteNumber (char **text) {
    int num_read = 0;
    double number = 0;
    sscanf (*text, " %lg%n", &number, &num_read);

    if (num_read){
        read_line_ += nummemchr (*text, '\n', num_read);
        char str_num[LENGTH_NAME_TOKEN] = {};
        sprintf (str_num, "%lg", number);
        createToken (str_num, number, TYPE_NUMBER, read_line_);
        *text += num_read;
    }
    return num_read;
}

int Tokens::lexicalAnalysisWriteString(char **text) {
    int num_read =  0;
    char string[LENGTH_NAME_TOKEN] = {};

//              !!!IMPORTANT!!!
//   scanf very bad work with cyrillic symbol,
//   so i use win-1251 and it don't see code of symbol,
//   so i manually write code of symbol
//    A - \300      п - \337
//    р - \340      џ - \377
//    Ј - \250      И - \270
//              !!!IMPORTANT!!!

    sscanf (*text, "  %[Р-пр-џЈИ0-9A-Za-z]%n", string, &num_read);

    if (num_read) {
        read_line_ += nummemchr(*text, '\n', num_read);
        createToken (string, 0, TYPE_STRING, read_line_);
        *text += num_read;
    }

    return num_read;
}

extern long ItLength (FILE* file);
/*{
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    long result = ftell(file);
    fseek(file, 0, SEEK_SET);

    return result;
}*/

void Tokens::readFile (int num_arguments, char *strings[]) {
    if (num_arguments <= 1) {
        printf ("File name not received!\n");
        abort ();
    }
    char* name_file = strings[1];
    name_file_ = name_file;

    char new_name_file[200] = {};
    strcat (new_name_file, name_file);
    char* dot = strrchr (new_name_file, '.');
    *dot = '\0';
    strcat (new_name_file, "-win1251.gop");
    char system_command[200] = {"iconv -f utf-8 -t windows-1251 "};
    strcat (system_command, name_file);
    strcat (system_command, " -o ");
    strcat (system_command, new_name_file);
    system (system_command);


    FILE* file = fopen (new_name_file, "rb");
    if (file == nullptr) {
        printf ("Can't find/open file, please, check name of file!\n");
        abort ();
    }

    long length_of_file = ItLength (file) + 1;
    char* text = (char*) calloc (length_of_file, sizeof(char));
    char* copy_text = text;
    fread (text, sizeof (char), length_of_file - 1, file);

    lexicalAnalysis(&copy_text);
//    dump ();
    free (text);
    char command[100] = "rm ";
    strcat (command, new_name_file);
    system (command);
}

int Tokens::lexicalAnalysisWriteSymbols(char **text) {
    int num_read =  0;
    char string[2] = {};
    sscanf (*text, " %c%n", string, &num_read);

    if (num_read) {
        read_line_ += nummemchr(*text, '\n', num_read);
//        createToken(string, 0, TYPE_SYMBOLS, read_line_);
        createToken(string, 0, TYPE_STRING, read_line_);
//-------------------------------------------------------------//
//        I stay TYPE_STRING for more universal parsing text,
//        so i can quickly replace from "+" to "sum"
//-------------------------------------------------------------//

        *text += num_read;
    }

    return num_read;
}

int Tokens::nummemchr(char *memptr, int val, size_t num_block) {
    char* copy_memptr = memptr;
//    num_block--;
//    memptr--;

    int num_enters = 0;
    while  (true) {

//        memptr = (char *) memchr ((void *) memptr, val, num_block - (memptr - copy_memptr) - 1 );
        memptr = (char *) memchr ((void *) memptr, val, num_block - 1);
        if (memptr == nullptr)
            break;
        memptr++;
        num_block -= memptr - copy_memptr;
        num_enters++;
    }
    return num_enters;
}

void Tokens::dump () {
    for (int i = 0; i < size_token_; i++) {
        printf("Token %d [0x%p]\n", i, &(data[i]));
        printf("Name: '%s'\n", data[i].name);
        printf("Value: %lg\n", data[i].value);
        printf("Type:  %d\n", data[i].type);
        printf("Line:  %d\n\n", data[i].line);
    }
}


