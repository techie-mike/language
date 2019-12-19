#include "libraries/MyTree.h"

int main(int num_arguments, char *strings[]) {
    Tree tree;
    tree.readTreeFromFile (num_arguments, strings);
    tree.writeConvertCode ("../workspace/back_code.gop");
    return 0;
}