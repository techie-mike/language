//
// Created by texnar on 10/12/2019.
//

#ifndef FRONTEND_DIFFUR_H
#define FRONTEND_DIFFUR_H


static bool isConstBranch (Tree* tree, size_tree_t index, const char* variable);
size_tree_t searchConstNode (Tree* tree, size_tree_t index);
void optimisationOfConstNode (size_tree_t index);
void clearNode (size_tree_t index);
void clearBranch (size_tree_t index);

size_tree_t diff (Tree *diff_tree,const size_tree_t index, const char* variable);
size_tree_t differentialOfAddSub (bool isAdd, Tree *diff_tree, size_tree_t left, size_tree_t right);
size_tree_t differentialOfMul (Tree *diff_tree, size_tree_t left, size_tree_t right);
static size_tree_t differentialOfDiv(Tree *diff_tree, size_tree_t left, size_tree_t right);
static size_tree_t differentialOfPow(Tree *diff_tree, size_tree_t left, size_tree_t right);


void searchVariables (size_tree_t index);

void recordExpression (char* text);
void writeFunExplanations (char* text, int num_action);

void allSimplifications (char* text);
int optimisationOfConstants (char* text);
int optimisationUnusedMembers(char* text);

void optimisationOfUnusedNodeAddSub (size_tree_t index);
void optimisationOfUnusedNodeMul (size_tree_t index);
void optimisationOfUnusedNodePow (size_tree_t index);

size_tree_t searchUnusedNode (Tree* tree , size_tree_t index);
bool checkUnusedNode (Tree* tree, size_tree_t index);
void optimisationOfUnusedNode (size_tree_t index);


static void startPrintLatex (FILE* file);
void writeTexInText (char * text, size_tree_t index);
static void endPrintLatex (FILE* file);

void priorityStrcat (char* text, const char* string, bool priority, size_tree_t index);
int priorityFunction (size_tree_t index);
bool comparePriority (size_tree_t first, size_tree_t second);
void writeNameInTexText (char* text, size_tree_t index);

bool exceptionOperators (char* text, size_tree_t index);

void whatItIs (char* name, size_tree_t index);
bool isVariable (char* name, size_tree_t index);
bool isOperator (char *name, size_tree_t index);
bool isNumber (char* name, size_tree_t index);

#endif //FRONTEND_DIFFUR_H
