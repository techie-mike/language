//
// Created by texnar on 16.09.2020.
//

#include "utilities.h"

CompilerException::CompilerException (char type_error,
                                      std::string &&string_user_error,
                                      std::string &&string_compiler_error,
                                      const char* time,
                                      const char* date,
                                      const char* file,
                                      const char* function,
                                            int   line):
    type_error            (type_error),
    string_user_error     (string_user_error),
    string_compiler_error (string_compiler_error),
    time                  (time),
    date                  (date),
    file                  (file),
    function              (function),
    line                  (line) {}

const char* CompilerException::what () const noexcept {
    return "brake program";
}
