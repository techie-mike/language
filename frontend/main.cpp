#include "libraries/token.h"
#include "libraries/base/MyTree.h"

int main(int num_arguments, char *strings[]) {
    Tokens tok;

    tok.readFile (num_arguments, strings);
    Tree tree;
    tree.readTreeFromTokens (&tok);
//    tree.writeTreeInFile("../treee.txt");
    tree.writeTreeInFile("tree.txt");
    return 0;
}