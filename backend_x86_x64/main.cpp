#include <iostream>
#include <stdio.h>

#include "libraries\backend_x86_x64.h"

int main (int num_arguments, char *strings[])
{
    _backend back = {};
    back.tree.readTreeFromFile (strings[1]);
    back.tree.fullVisit (_backend::whatItIs);
    back.tree.dump      (_backend::treeColoring);

    back.compiler.compilingCode ();
    back.compiler.writeInObjFile ("obj_text_log.obj");


    return 0;
}
