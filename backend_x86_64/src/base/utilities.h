//
// Created by texnar on 16.09.2020.
//

#ifndef BACKEND_X86_64_UTILITIES_H
#define BACKEND_X86_64_UTILITIES_H

#include <exception>
#include <string>

struct CompilerException:
        public std::exception {
    char type_error;
    std::string string_user_error;
    std::string string_compiler_error;
    const char* time;
    const char* date;
    const char* file;
    const char* function;
    const int   line;

    CompilerException (char type_error,
                       std::string&& string_user_error,
                       std::string&& string_compiler_error,
                       const char* time,
                       const char* date,
                       const char* file,
                       const char* function,
                             int   line);

    const static char USER_ERROR     = 1;
    const static char COMPILER_ERROR = 2;

    virtual const char* what() const noexcept;
};

#ifndef THROW_ERROR
#define THROW_ERROR(type, user_string, compiler_string) \
throw (CompilerException(CompilerException::type, user_string, compiler_string, __TIME__, __DATE__, __FILE__, __FUNCTION__, __LINE__));

#endif //THROW_ERROR

#endif //BACKEND_X86_64_UTILITIES_H
