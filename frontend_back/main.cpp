#include "libraries/MyTree.h"

int main(int num_arguments, char *strings[]) {
    Tree tree;
    tree.readTreeFromFile (num_arguments, strings);
    tree.writeConvertCode ("../logs/back_code.gop");

    if (num_arguments > 2)
        if (!strcmp (strings[2], "--d"))
            tree.dump();
    return 0;
}