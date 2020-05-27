#include <cstdio>

#include "libraries\backend_x86_x64.h"

int main (int num_arguments, char *strings[])
{
    backend back = {};

    back.compiler.checkArguments (num_arguments, strings);
    back.tree.readTreeFromFile   (strings[1]);
    back.tree.fullVisit          (backend::whatItIs);

//              The note on future
//    typedef void (backend::compiler::*func) ();
//    func first = &backend::compiler::compilingCode;
    back.compiler.compilingCode ();

    if (back.debug_) {
        back.tree.dump ("..\\logs\\backend_x86", backend::treeColoring);
        back.compiler.writeInObjFile ("..\\logs\\obj_text_log.obj");
    }

    back.linker.secondLinking (back.name_file_to_write_);
    back.freeMemory ();

    return 0;
}
