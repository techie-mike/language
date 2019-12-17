#include "libraries/MyTree.h"

int main() {
    Tree tree;
//    tree.readTreeFromFile ("../workspace/ruslan.txt");
    tree.readTreeFromFile ("../workspace/treee.txt-utf8");
    tree.dump();
    tree.writeConvertCode ("../workspace/code.gop");
    return 0;
}