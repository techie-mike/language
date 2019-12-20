#include "libraries/MyTree.h"

int main(int num_arg, char *poin_arg[]) {
    Tree diff;
    diff.readTreeFromFile (num_arg, poin_arg);
    diff.optimisationOfConstants ();

    diff.optimisationUnusedMembers (true);
    diff.diffFunc ();
    diff.writeTreeInFile (poin_arg[1]);
    if (num_arg > 2)
        if (!strcmp (poin_arg[2], "--d"))
            diff.dump();
    return 0;
}