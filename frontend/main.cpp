#include "libraries/token.h"
#include "libraries/base/MyTree.h"

int main() {
    Tokens tok;

    tok.readFile ("../workspace/programs/quad.gop");
    Tree tree;
    tree.readTreeFromTokens (&tok);
//    tree.writeTreeInFile("../treee.txt");
    tree.writeTreeInFile("../workspace/treee.txt");
    return 0;
}