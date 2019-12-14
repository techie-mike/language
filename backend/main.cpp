#include "libraries/MyTree.h"
int main() {
    Tree tree;
    tree.readTreeFromFile("../frontend/treee.txt");
    tree.dump ();
    return 0;
}