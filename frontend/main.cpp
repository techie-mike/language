#include "libraries/token.h"
#include "libraries/base/MyTree.h"

int main() {
    Tokens tok;

    tok.readFile ("test.gop");
    Tree tree;
    tree.readTreeFromTokens (&tok);
    return 0;
}