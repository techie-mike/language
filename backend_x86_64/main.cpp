#include <cstdio>

#include "src/backend_x86_64.h"

int main (int num_arguments, char *strings[])
{
    Backend back = {};
    try {
        back.compiler_.checkArguments (num_arguments, strings);
        back.tree_.readTreeFromFile (strings[1]);
        back.tree_.fullVisit (Backend::whatItIs);

        back.compiler_.compilingCode ();

        if (back.debug_) {
            back.tree_.dump ("..\\logs\\backend_x86", Backend::treeColoring);
            back.compiler_.writeInObjFile ("..\\logs\\obj_text_log.obj");
        }

        back.linker_.secondLinking (back.name_file_to_write_);
    }
    catch (const char *string_error) {
        printf ("Error: %s\n", string_error);
        return 1;
    }

    back.freeMemory ();
    return 0;
}
