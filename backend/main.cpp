#include "libraries/MyTree.h"
int main() {
    Tree tree;
    tree.readTreeFromFile ("../workspace/treee.txt-utf8");
    tree.writeConvertCode ("test.nasm");
    tree.dump ();
    return 0;
}