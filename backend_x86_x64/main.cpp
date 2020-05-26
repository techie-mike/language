#include <iostream>
#include <stdio.h>

#include "libraries\backend_x86_x64.h"

int main (int num_arguments, char *strings[])
{
    backend back = {};
    back.tree.readTreeFromFile (strings[1]);
    back.tree.fullVisit (backend::whatItIs);
    back.tree.dump      (backend::treeColoring);

    back.compiler.compilingCode ();
    back.compiler.writeInObjFile ("obj_text_log.obj");
//    back.linker.linking ();
//    back.linker.writeExeInFile ("testlink.exe");
    back.linker.secondLinking ("program.exe");
    back.freeMemory ();

    return 0;
}
