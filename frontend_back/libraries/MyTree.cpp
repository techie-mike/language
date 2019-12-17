//
// Created by texnar on 11/11/2019.
//
#include "MyTree.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include <assert.h>
//#include <string.h>

#define LEVEL_VERIFIC 1
#include "my_stack.h"

Tree::Tree(size_tree_t DEFAULT_LENGTH, size_tree_t DEFAULT_LENGTH_NAMES) :
    free_ (0),
    one_element ((elem*) calloc(DEFAULT_LENGTH, sizeof(one_element[0]) )),
    all_names ((char *) calloc(DEFAULT_LENGTH_NAMES, sizeof(char))),
    size_ (0),
    size_names_ (0),
    root_ (0),
    length_names_ (DEFAULT_LENGTH_NAMES),
    length_ (DEFAULT_LENGTH),
    write_text_ (nullptr),
    num_write_text_ (0),
    last_str_have_n_ (false),
    num_tabs_ (0)
{
    fillingPoisonousValues();
}

void Tree::fillingPoisonousValues()
{
    for (size_tree_t i = size_ + 1; i < length_; i++){
//        data_[i] = 0;
        one_element[i].right_ = i + 1;
        one_element[i].parent_ = -1;
        one_element[i].type_ = -1;
        one_element[i].value_ = -1;
        one_element[i].name_ = nullptr;
    }
    one_element[length_ - 1].right_ = 0;
    free_ = size_ + 1;
}

Tree::~Tree()
{
    free(one_element);
    free(all_names);
    size_ = 0;
    length_ = 0;
    size_names_ = 0;
    root_ = 0;
    free_ = 0;
}

long ItLength(FILE* file)
{
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    long result = ftell(file);
    fseek(file, 0, SEEK_SET);

    return result;
}

/*bool Tree::readTreeFromFile(const char* name_file) {
    FILE* file = fopen(name_file, "r+");
    long length_of_file = ItLength(file) + 1;
    char* text = (char*) calloc(length_of_file, sizeof(char));
    fread(text, sizeof(char), length_of_file - 1, file);
    text[length_of_file - 1] = '\0';

//    getG(text);
//    loadingTree(text);

    free(text);
}*/

void Tree::loadingTree(char* text)
{
    assert(text != nullptr);

    if (text[0] == 0xEF && text[1] == 0xBB && text[2] == 0xBF)
        text += 3;
    skipSymbols(&text);
    readTextTree(text);
}

void Tree::readTextTree(char *read_now)
{
    if (*read_now == '{'){
        read_now ++;
        readNewObject(&read_now);
    }
    else
        printf("Error in read!\n");
}

void Tree::skipSymbols(char **read_now)
{
  /*  assert(read_now != nullptr);
    while (isspace(**read_now))
        (*read_now)++;
*/
//    while (**read_now != ')' && **read_now != '('
//            && **read_now != '\"' && **read_now != '\0') (*read_now)++;
}

size_tree_t Tree::readNewObject(char **read_now)
{
    size_tree_t left_temp = 0, right_temp = 0;
    char name[100] = {};
    readName(read_now, name);
    skipSymbols(read_now);

//-- read left branch --//
    if (**read_now == '@'){
        left_temp = 0;
        (*read_now)++;
    }
    else
    if (**read_now == '{') {
        (*read_now)++;
        left_temp = readNewObject(read_now);

        skipSymbols(read_now);
        assert(**read_now == '}');
        (*read_now)++;
    }
//    if (**read_now == '{')

//-- read left branch --//

    skipSymbols(read_now);

//-- read right branch --//
    if (**read_now == '@'){
        right_temp = 0;
        (*read_now)++;
    }
    else
    if (**read_now == '{') {
        (*read_now)++;
        right_temp = readNewObject(read_now);

        skipSymbols(read_now);
        assert(**read_now == '}');
        (*read_now)++;
    }
//-- read right branch --//

    skipSymbols(read_now);

    if (**read_now == '}') {
        root_ = createNewObject(name, left_temp, right_temp);
//        dump();
        return root_;
    }

    if (**read_now != '}'/* && (left_temp == 0 || right_temp == 0)*/){
        printf("Error, don't found end of object!\n");
        abort();
    }
}

void Tree::readName(char **read_now, char *name)
{
//    (*read_now)++;
    skipSymbols(read_now);

    int read_symbol = 0;
    char num_read = 0;

    if (haveQuotes(read_now))
        num_read = (char) sscanf(*read_now, "\"%[^\"]\"%n", name, &read_symbol);
    else
        num_read = (char) sscanf(*read_now, " %[^{}@ \n\t]%n", name, &read_symbol);

    if (num_read == 0) printf("Can't read name\n"); // should be test this function
    *read_now += read_symbol;

    skipSymbols(read_now);
}

size_tree_t Tree::createNewObject(char name[], size_tree_t left, size_tree_t right, size_tree_t parent)
{
    autoLengthIncrease();
    autoLengthNamesIncrease();

    size_tree_t new_index = free_;
    free_ = one_element[free_].right_;

    whatItIs(name, new_index);

    one_element[new_index].right_ = right;
    one_element[new_index].left_ = left;
    one_element[new_index].name_ = all_names + size_names_;

    if (one_element[new_index].left_)
        one_element[one_element[new_index].left_].parent_ = new_index;
    if (one_element[new_index].right_)
        one_element[one_element[new_index].right_].parent_ = new_index;

    if (one_element[new_index].parent_ == -1)
        one_element[new_index].parent_ = parent;
//    if (parent)
    int length = (int) (strlen(name) + 1);
    strcpy((all_names + size_names_), name);
    size_names_ += length;

   /* if (size_ == 0) {
        one_element[new_index].parent_ = 0;
    }*/
    size_++;
//    root_ = new_index;
//dump();
    return new_index;
}

bool Tree::writeTreeInFile(const char *name_file)
{
    char *text = (char*) calloc (50000, sizeof(char));
    writeTree (text, root_);
    writeFulTreeInFile (text, name_file);

    free(text);
}

void Tree::writeFulTreeInFile (char* text, const char *name_file)
{
    FILE* file = fopen (name_file, "w");
    int length = strlen (text);
    fwrite (text, sizeof(char), length, file);
    fclose (file);
    char system_command[100] = "iconv -f windows-1251 -t utf-8 ";
    strcat (system_command, name_file);
    strcat (system_command, " -o ");
    strcat (system_command, name_file);
    strcat (system_command, "-utf8");

    system (system_command);
}

void Tree::writeTree (char* text, size_tree_t index)
{
    const char* open_bracket = "{";
    const char* close_bracket = "}";
//    char name[1000] = "{\"";
//    strcat(text, "(\"");
//    strcat(text, one_element[index].name_);
//    strcat(text, "\"");

    strcat(text, open_bracket);
    strcat(text, one_element[index].name_);
//    strcat(text, "\"");

    if (one_element[index].left_ == 0 && one_element[index].right_ == 0)
        strcat(text, close_bracket);
    else {
        if (one_element[index].left_ == 0 && one_element[index].right_ != 0) {
            strcat(text, "@");
            writeTree(text, one_element[index].right_);
        } else
        if (one_element[index].left_ != 0 && one_element[index].right_ == 0) {
            writeTree(text, one_element[index].left_);
            strcat(text, "@");
        } else
        {
            writeTree(text, one_element[index].left_);
            writeTree(text, one_element[index].right_);
        }
        strcat(text, "}");
    }
    /*if (one_element[index].left_ == 0 && one_element[index].right_ == 0){
        strcat(text, open_bracket);
//        strcat(text, one_element[index].name_);
        writeNameInTextFromTree(text, one_element[index].name_);
        strcat(text, close_bracket);
    }
    else
    if (one_element[index].left_ == 0 && one_element[index].right_ != 0){
        strcat(text, open_bracket);
//        strcat(text, one_element[index].name_);
        writeNameInTextFromTree(text, one_element[index].name_);
        writeTree(text, one_element[index].right_);
        strcat(text, close_bracket);
    }
    else
    if (one_element[index].left_ != 0 && one_element[index].right_ == 0){
        strcat(text, open_bracket);
//        strcat(text, one_element[index].name_);
        writeTree(text, one_element[index].left_);
        writeNameInTextFromTree(text, one_element[index].name_);
        strcat(text, close_bracket);
    }

    else {
        strcat(text, open_bracket);

        writeTree(text, one_element[index].left_);
        writeNameInTextFromTree(text, one_element[index].name_);
        writeTree(text, one_element[index].right_);
        strcat(text, close_bracket);
    }*/
}

void Tree::dump()
{
    FILE* file = fopen("text_picture.dot", "wb");
    fprintf(file, "digraph structs {\n");
    fprintf(file, "rankdir=HR;\n");


    for (size_tree_t i = 1; i < length_; i++){
        if (one_element[i].parent_ != -1){
            if (*(one_element[i].name_) == '>' || *(one_element[i].name_) == '<')
                fprintf(file, "Index%d [shape=record, label=\" <left>  %d | {'\\%s' | Par: %d} | {Index: %d | Type: %d | Value: %lg} | <right> %d \",",
                        i, one_element[i].left_, one_element[i].name_,one_element[i].parent_, i, one_element[i].type_, one_element[i].value_, one_element[i].right_);
            else
                fprintf(file, "Index%d [shape=record, label=\" <left>  %d | {'%s' | Par: %d} | {Index: %d | Type: %d | Value: %lg} | <right> %d \",",
                i, one_element[i].left_, one_element[i].name_,one_element[i].parent_, i, one_element[i].type_, one_element[i].value_, one_element[i].right_);

            switch (one_element[i].type_){
                case TYPE_OPERATOR:
                    fprintf(file, "style=\"filled\", fillcolor=\"lightgrey\" ");
                    break;
                case TYPE_NUMBER:
                    fprintf(file, "style=\"filled\", fillcolor=\"yellow\" ");
                    break;
                case TYPE_VARIABLE:
                    fprintf(file, "style=\"filled\", fillcolor=\"lightblue\" ");
                    break;
                default:
                    break;
            }
            fprintf(file, "];\n");

        }
    }
    fprintf(file, "System [shape=record, label=\" <root> Root\" ];\n");
//    fprintf(file, "System [shape=record, label=\" <head> Head | <tail> Tail | <free> Free\" ];\n");
//    fprintf(file, "System: <head> -> Index%d : <index>;\n", head_);
//    fprintf(file, "System: <tail> -> Index%d : <index>;\n", tail_);
//    fprintf(file, "System: <free> -> Index%d : <index>;\n", free_);
    fprintf(file, "System: <root> -> Index%d;\n", root_);

    for (size_tree_t i = 1; i < length_; i++) {
        if (one_element[i].parent_ != -1) {
            if (one_element[i].left_ != 0)
                fprintf(file, "Index%d: <left> -> Index%d ;\n", i, one_element[i].left_);
            if (one_element[i].right_ != 0)
                fprintf(file, "Index%d: <right> -> Index%d ;\n", i, one_element[i].right_);
//            fprintf(file, "Index%d: <parent> -> Index%d ;\n", i, one_element[i].parent_);
        }
    }

    fprintf(file, "}\n");
    fclose(file);

    system("iconv -f windows-1251 -t utf-8 text_picture.dot -o text_picture_utf8.dot");
    system("dot text_picture_utf8.dot -T png -o test.png");
}

size_tree_t Tree::checkName(char *name)
{
    assert(name);
    return seeBranch(name, root_);
}


size_tree_t Tree::seeBranch(char* name, size_tree_t index)
{
    if (!strcmp(one_element[index].name_, name)) return index;

    size_tree_t new_index = 0;

    if (one_element[index].left_ != 0){
        new_index = seeBranch(name, one_element[index].left_);
        if (new_index) return new_index;
    }
    if (one_element[index].right_ != 0){
        new_index = seeBranch(name, one_element[index].right_);
        if (new_index) return new_index;
    }
    return 0;
}



void Tree::searchParents(Stack_t *stack, size_tree_t index)
{
    while (true){
        StackPush(stack, index);
        if (index == root_) break;
        index = one_element[index].parent_;
    }
}

void Tree::whatItIs(char *name, size_tree_t index)
{
   isOperator(name, index);
   isNumber(name, index);
   isVariable(name, index);
}

bool Tree::isOperator (char *name, size_tree_t index)
{
#define OPER(str, num) if (!strcmp( #str , name)) {\
    one_element[index].type_ = TYPE_OPERATOR;\
    one_element[index].value_ = num;\
    return true;\
}

    OPER(+  , OPERATOR_ADD)
    OPER(-  , OPERATOR_SUB)
    OPER(*  , OPERATOR_MUL)
    OPER(/  , OPERATOR_DIV)
    OPER(^  , OPERATOR_POW)
    OPER(sin, OPERATOR_SIN)
    OPER(cos, OPERATOR_COS)
    OPER(ln , OPERATOR_LN )
    OPER(=  , 0)
    OPER(;  , 0)

#undef OPER
}

bool Tree::haveQuotes(char **read_now) {
    char symbol[3] = {};
    return sscanf (*read_now, "%[\"]", symbol) != 0;
}

bool Tree::isNumber(char *name, size_tree_t index) {
    value_t number = 0;
    int num_read = 0;
    sscanf(name, "%lg%n", &number, &num_read);
    if (strlen(name) == num_read){
        one_element[index].type_ = TYPE_NUMBER;
        one_element[index].value_ = number;
        return true;
    }
    return false;
}

bool Tree::isVariable(char *name, size_tree_t index) {
#define VAR(str, num) if (!strcmp( #str , name)) {\
    one_element[index].type_ = TYPE_VARIABLE;\
    one_element[index].value_ = num;\
    return true;\
}

    VAR(x , VARIABLE_X)
    VAR(y , VARIABLE_Y)
    VAR(z , VARIABLE_Z)
    VAR(t , VARIABLE_T)

#undef VAR
    return false;
}

size_tree_t Tree::diff(Tree *diff_tree, const size_tree_t index, const char* variable) {
    assert(index!= 0);
//    diff_tree->dump();

    switch (one_element[index].type_)
    {
        case TYPE_NUMBER:
            return createNewNode(diff_tree, TYPE_NUMBER, 0);
            break;
        case TYPE_VARIABLE:
            if (!strcmp(variable, one_element[index].name_))
            // it is place, where i should add check what is varialble we get diff //
                return createNewNode(diff_tree, TYPE_NUMBER, 1);
            else
                return createNewNode(diff_tree, TYPE_NUMBER, 0);
        case TYPE_OPERATOR:

#define DIV(left, right)   differentialOfDiv           (diff_tree, left, right)
#define POW(left, right)   differentialOfPow           (diff_tree, left, right)
#define PLUS(left, right)  differentialOfAddSub (true,  diff_tree, left, right)
#define MINUS(left, right) differentialOfAddSub (false, diff_tree, left, right)
#define MUL(left,right)    differentialOfMul           (diff_tree, left, right)
#define LN(right)          diff_tree->createNewObject ((char*) "ln", 0, right)


#define dL diff(diff_tree, one_element[index].left_, variable)
#define dR diff(diff_tree, one_element[index].right_, variable)
#define cL copyBranch(diff_tree, one_element[index].left_)
#define cR copyBranch(diff_tree, one_element[index].right_)

#define crNum(number) createNumber(diff_tree, number)
#define crCos(right) diff_tree->createNewObject((char*) "cos", 0, right)
#define crSin(right) diff_tree->createNewObject((char*) "sin", 0, right)

#define constLEFT isConstBranch(this, one_element[index].left_, variable)
#define constRIGHT isConstBranch(this, one_element[index].right_, variable)


            switch ((int) round(one_element[index].value_))
            {
                case OPERATOR_ADD:
                    return PLUS(dL, dR);
                    break;
                case OPERATOR_SUB:
                    return MINUS(dL, dR);
                    break;
                case OPERATOR_MUL:
                    return PLUS(MUL(dL,cR),MUL(cL,dR));
                    break;
                case OPERATOR_DIV:
                    return DIV(MINUS(MUL(dL,cR), MUL(cL,dR)), POW(cR,crNum(2)));
                    break;
                case OPERATOR_POW:
                    if (constLEFT && constRIGHT)
                        return crNum(0);
//                        return POW(cL,cR);
                    if (constLEFT && !constRIGHT)
                        return MUL(LN(cL),MUL(dR, POW(cL,cR)));
                    if (!constLEFT && constRIGHT){
                        return MUL(cR, MUL(dL, POW(cL, MINUS(cR, crNum(1)))));
                    }

                case OPERATOR_LN:
                    return MUL(dR, DIV(crNum(1), cR));
                case OPERATOR_SIN:
                    return MUL(dR, crCos(cR));
                case OPERATOR_COS:
                    return MINUS(crNum(0), MUL(dR, crCos(cR)));
            }
            break;

    }

#undef constLEFT
#undef constRIGHT

#undef crNum
#undef cL
#undef cR

#undef dR
#undef dL

#undef PLUS
#undef MINUS
#undef MUL
#undef POW
#undef DIV
    return 0;
}

size_tree_t Tree::createNewNode(Tree *diff_tree, size_tree_t type, value_t value) {
    char name[100] = {};
    if (type == TYPE_NUMBER) {
        sprintf(name, "%lg", value);
    }
    size_tree_t index = diff_tree->createNewObject(name,0,0);
    diff_tree->one_element[index].type_ = type;
    diff_tree->one_element[index].value_ = value;
    return index;
}

void Tree::fullDifferential(Tree *diff_tree) {
    var_found = {};
    searchVariables(root_);
    size_tree_t x_index = 0, y_index = 0, z_index = 0, t_index = 0, new_index = 0;

    if (var_found.x)
        x_index = diff(diff_tree, root_,"x");
    if (x_index)
        new_index = x_index;
    diff_tree->dump();

    if (var_found.y)
        y_index = diff(diff_tree, root_,"y");
    if (y_index) {
        if (new_index)
            new_index = diff_tree->createNewObject((char*) "+", new_index, y_index);
    }
    diff_tree->dump();

    if (var_found.z)
        z_index = diff(diff_tree, root_,"z");
    if (z_index) {
        if (new_index)
            new_index = diff_tree->createNewObject((char*) "+", new_index, z_index);
    }

    if (var_found.t)
        t_index = diff(diff_tree, root_,"t");
    if (t_index) {
        if (new_index)
            new_index = diff_tree->createNewObject((char*) "+", new_index, t_index);
    }

    diff_tree->root_ = new_index;
}

void Tree::copyTree(Tree &copy) {
    copy.size_ = this->size_;
    copy.length_ = this->length_;

    free(copy.one_element);
    copy.one_element = (elem*) calloc(copy.length_, sizeof(one_element[0]));
    copy.size_names_ = this->size_names_;
    copy.length_names_ = this->length_names_;

    free(copy.all_names);
    copy.all_names = (char*) calloc(copy.length_names_, sizeof(char));
    copy.root_ = this->root_;
    copy.free_ = this->free_;
}

size_tree_t Tree::differentialOfAddSub(bool isAdd, Tree* diff_tree, size_tree_t left, size_tree_t right) {
    if(isAdd)
        return diff_tree->createNewObject((char*) "+", left, right);
    else
        return diff_tree->createNewObject((char*) "-", left, right);
}

size_tree_t Tree::differentialOfMul(Tree *diff_tree, size_tree_t left, size_tree_t right) {
    return diff_tree->createNewObject((char*) "*", left, right);
}

size_tree_t Tree::copyNode(Tree *diff_tree, size_tree_t index) {
    diff_tree->createNewNode(diff_tree, this->one_element[index].type_, this->one_element[index].value_);

    return 0;
}

size_tree_t Tree::copyBranch(Tree *diff_tree, size_tree_t index) {
    size_tree_t left = 0, right = 0;
    if (this->one_element[index].left_)
        left = copyBranch(diff_tree, this->one_element[index].left_);
    if (this->one_element[index].right_)
        right = copyBranch(diff_tree, this->one_element[index].right_);

    size_tree_t new_index = diff_tree->createNewObject(this->one_element[index].name_, left, right);
//    diff_tree->one_element[new_index].type_ = this->one_element[index].type_;
//    diff_tree->one_element[new_index].value_ = this->one_element[index].value_;

    return new_index;
}

size_tree_t Tree::differentialOfDiv(Tree *diff_tree, size_tree_t left, size_tree_t right) {
    return diff_tree->createNewObject((char*) "/", left, right);
}

size_tree_t Tree::differentialOfPow(Tree *diff_tree, size_tree_t left, size_tree_t right) {

    return diff_tree->createNewObject((char*) "^", left, right);
}

bool Tree::isConstBranch (Tree *tree, size_tree_t index, const char* variable){
    bool found = false;
    if (tree->one_element[index].type_ == TYPE_NUMBER
    || (tree->one_element[index].type_ == TYPE_VARIABLE && strcmp(tree->one_element[index].name_, variable)))
        return true;

    if (tree->one_element[index].left_)
        found = isConstBranch(tree, tree->one_element[index].left_, variable);

    if (found && tree->one_element[index].right_)
        found = isConstBranch(tree, tree->one_element[index].right_, variable);

    return found;
}

/*size_tree_t Tree::isConstBranch (Tree *tree, size_tree_t index){
    size_tree_t found = 0;
    if (tree->one_element[index].type_ == TYPE_NUMBER)
        return index;

    if (tree->one_element[index].left_)
        found = isConstBranch(tree, tree->one_element[index].left_);

    if (found && tree->one_element[index].right_)
        found = isConstBranch(tree, tree->one_element[index].right_);

    return found;
}*/

void Tree::latex(Tree* origin, const char* name)
{
    FILE* file = fopen(name, "wb");
    startPrintLatex(file);
    char* text = (char*) calloc(40000, sizeof(char));

    origin->recordExpression(text);
    allSimplifications(text);
//    writeTexInText(text, root_);
//    deleteLastBracket(text);

    fwrite(text, sizeof(char), strlen(text), file);

    endPrintLatex(file);
    fclose(file);
    free(text);
    char name_file[100] = "pdflatex ";
    strcat(name_file, name);
    system(name_file);
    system("gvfs-open dump_tex.pdf");

}

void Tree::startPrintLatex (FILE* file)
{
    fprintf(file, "\\documentclass{article}\n\\usepackage[utf8]{inputenc}\n\\usepackage[russian]{babel}\n \\begin{document}\n\\section{Очень важное взятие ДИФФЕРЕНЦИАЛА этой хрени}\n\n");
}

void Tree::endPrintLatex (FILE* file)
{
    fprintf(file, "\\end{document}");
}

void Tree::writeNameInTextFromTree(char *text, char *name) {
    if (strchr(name, ' ')) {
        strcat(text, "\"");
        strcat(text, name);
        strcat(text, "\"");
    }
    else {
        strcat(text, " ");
        strcat(text, name);
    }
}

void Tree::writeTexInText(char *text, size_tree_t index) {
#define Strcat(text, string) priorityStrcat(text, string, priority, index)

    if (one_element[index].left_ == 0 && one_element[index].right_ == 0){
//        writeNameInTextFromTree(text, one_element[index].name_);
        writeNameInTexText(text, index);
    }
    else
    if (one_element[index].left_ != 0 && one_element[index].right_ == 0){

        bool priority = comparePriority(index, one_element[index].parent_);

        Strcat(text, "(");
        writeTexInText(text, one_element[index].left_);
//        writeNameInTextFromTree(text, one_element[index].name_);
        writeNameInTexText(text, index);
        Strcat(text, ")");
    }
    else
    if (one_element[index].left_ == 0 && one_element[index].right_ != 0){
        bool priority = comparePriority(index, one_element[index].parent_);

        Strcat(text, "(");
//        writeNameInTextFromTree(text, one_element[index].name_);
        writeNameInTexText(text, index);
        writeTexInText(text, one_element[index].right_);
        Strcat(text, ")");
    }

    else {
        bool priority = comparePriority(index, one_element[index].parent_);

        Strcat(text, "(");

        if (!exceptionOperators(text, index)) {
            writeTexInText(text, one_element[index].left_);
        //        writeNameInTextFromTree(text, one_element[index].name_);
            writeNameInTexText(text, index);
            writeTexInText(text, one_element[index].right_);
        }

        Strcat(text, ")");
    }

//    if (index == root_)
//        *last_text = ' ';
#undef Strcat
}

int Tree::priorityFunction(size_tree_t index) {
    if (one_element[index].type_ == 0)
        return 1;
    if (one_element[index].type_ == TYPE_NUMBER ||
        one_element[index].type_ == TYPE_VARIABLE)
        return 1;

    if (one_element[index].type_ == TYPE_OPERATOR &&
        ((int) round(one_element[index].value_) == OPERATOR_ADD ||
         (int) round(one_element[index].value_) == OPERATOR_SUB))
        return 2;

    if (one_element[index].type_ == TYPE_OPERATOR &&
        ((int) round(one_element[index].value_) == OPERATOR_MUL ||
         (int) round(one_element[index].value_) == OPERATOR_DIV))
        return 3;

    if (one_element[index].type_ == TYPE_OPERATOR &&
        ((int) round(one_element[index].value_) == OPERATOR_POW))
        return 4;

    if (one_element[index].type_ == TYPE_OPERATOR &&
        ((int) round(one_element[index].value_) == OPERATOR_LN
        ||(int) round(one_element[index].value_) == OPERATOR_SIN
        ||(int) round(one_element[index].value_) == OPERATOR_COS))
        return 5;


    printf("Error in priority function %d\n", one_element[index].type_);
}

void Tree::deleteLastBracket(char *text) {
    char* point = strchr(text, '\0');
    if (point)
        *(point - 1) = '\0';
    else
        printf("Error in deleteLastBracket\n");
}

void Tree::priorityStrcat(char *text, const char *string, bool priority, size_tree_t index) {
    if (index == root_)
        return;
    if (priority) {
        strcat(text, string);
    }
}

bool Tree::comparePriority(size_tree_t first, size_tree_t second) {
    if (one_element[second].type_ == TYPE_OPERATOR
    && (int) (one_element[second].value_) == OPERATOR_DIV
    && one_element[first].type_ == TYPE_OPERATOR)
        return false;
    if (priorityFunction(first) == 4 && priorityFunction(second) == 4)
        return true;
    if (priorityFunction(first) < priorityFunction(second))
        return true;
    return false;
}

void Tree::writeNameInTexText(char *text, size_tree_t index) {
    char name[100] = {};
    if (one_element[index].type_ == TYPE_VARIABLE) {
//        strcat(text, "$");
        strcat(text, one_element[index].name_);
//        strcat(text, "$");
    } else
        strcat(text, one_element[index].name_);

    writeNameInTextFromTree(text, name);
}

bool Tree::exceptionOperators(char *text, size_tree_t index) {
    if (one_element[index].type_ == TYPE_OPERATOR
        && (int) (one_element[index].value_ + 1e-12) == OPERATOR_DIV) {
        strcat(text, "\\frac");
        strcat(text, "{");
        writeTexInText(text, one_element[index].left_);
        strcat(text, "}");
        //        writeNameInTextFromTree(text, one_element[index].name_);
//        writeNameInTexText(text, index);
        strcat(text, "{");
        writeTexInText(text, one_element[index].right_);
        strcat(text, "}");
        return true;
    }
    if (one_element[index].type_ == TYPE_OPERATOR
        && (int) (one_element[index].value_ + 1e-12) == OPERATOR_MUL) {
        writeTexInText(text, one_element[index].left_);
        strcat(text, "\\cdot ");
        writeTexInText(text, one_element[index].right_);
        return true;

    }

    if (one_element[index].type_ == TYPE_OPERATOR
        && (int) (one_element[index].value_ + 1e-12) == OPERATOR_POW) {
//        strcat(text, "\\frac");
//        strcat(text, "{");
        writeTexInText(text, one_element[index].left_);
//        strcat(text, "}");
        //        writeNameInTextFromTree(text, one_element[index].name_);
        writeNameInTexText(text, index);
        strcat(text, "{");
        writeTexInText(text, one_element[index].right_);
        strcat(text, "}");
        return true;
    }
    return false;
}

void Tree::recordExpression(char *text) {
    strcat(text, "\\begin{displaymath}\n");
    writeTexInText(text, root_);
    strcat(text, "\n\\end{displaymath}\n");
}

void Tree::allSimplifications(char *text) {
    strcat(text, "Немного преобразуем полученную штуку, чтобы выражение выглядело ещё менее понятным.\n");
    recordExpression(text);
    while (true){
        int num_action = 0;
        num_action += optimisationOfConstants(text);
        num_action += optimisationUnusedMembers(text);
        if (!num_action)
            break;
    }
}

int Tree::optimisationOfConstants(char *text) {
    int num_optimisation = 0;
    dump();
    while (true) {
        size_tree_t branch = searchConstNode(this, root_);
        if (!branch) break;
        optimisationOfConstNode(branch);
//        writeTexInText(text, index);
        strcat(text, "\n");
//        dump();
        num_optimisation++;
        writeFunExplanations(text, num_optimisation);
        if (num_optimisation)
            recordExpression(text);
    }
//    recordExpression(text);
    return num_optimisation;
}

size_tree_t Tree::searchConstNode(Tree *tree, size_tree_t index) {
    size_tree_t found = 0;
    if (tree->one_element[one_element[index].left_].type_ == TYPE_NUMBER
        && tree->one_element[one_element[index].right_].type_ == TYPE_NUMBER)
        return index;

    if (tree->one_element[index].left_)
        found = searchConstNode(tree, tree->one_element[index].left_);

    if (!found && tree->one_element[index].right_)
        found = searchConstNode(tree, tree->one_element[index].right_);

    return found;
}

void Tree::optimisationOfConstNode(size_tree_t index) {
    switch ((int) (one_element[index].value_ + 1e-12)) {
        case OPERATOR_ADD:
            one_element[index].type_ = TYPE_NUMBER;
            one_element[index].value_ = one_element[one_element[index].left_].value_ +
                    one_element[one_element[index].right_].value_;
            clearNode(one_element[index].left_);
            clearNode(one_element[index].right_);
            break;
        case OPERATOR_SUB:
            one_element[index].type_ = TYPE_NUMBER;
            one_element[index].value_ = one_element[one_element[index].left_].value_ -
                                        one_element[one_element[index].right_].value_;
            clearNode(one_element[index].left_);
            clearNode(one_element[index].right_);
            break;

        case OPERATOR_MUL:
            one_element[index].type_ = TYPE_NUMBER;
            one_element[index].value_ = one_element[one_element[index].left_].value_ *
                                        one_element[one_element[index].right_].value_;
            clearNode(one_element[index].left_);
            clearNode(one_element[index].right_);
            break;

        case OPERATOR_DIV:
            one_element[index].type_ = TYPE_NUMBER;
            one_element[index].value_ = one_element[one_element[index].left_].value_ /
                                        one_element[one_element[index].right_].value_;
            clearNode(one_element[index].left_);
            clearNode(one_element[index].right_);
            break;
        case OPERATOR_POW:
            one_element[index].type_ = TYPE_NUMBER;
            one_element[index].value_ = pow (one_element[one_element[index].left_].value_, one_element[one_element[index].right_].value_);
            clearNode(one_element[index].left_);
            clearNode(one_element[index].right_);
            break;
        default:
            printf("Unknow operator\n");
            break;
    }

    char name[100] = {};
    sprintf(name, "%lg", one_element[index].value_);
    int length = (int) (strlen(name) + 1);
    one_element[index].name_ = strcpy((all_names + size_names_), name);
    if (one_element[index].name_ == nullptr)
        printf("Error in strcpy\n");
    size_names_ += length;

//    one_element[index].name_ = name;

}

void Tree::clearNode(size_tree_t index) {
// i don't know why i write this "if" last night //
// ohh, may be i understand

    if (one_element[one_element[index].parent_].left_ == index)
        {
        one_element[one_element[index].parent_].left_ = 0;

        }else
    if (one_element[one_element[index].parent_].right_ == index)
        one_element[one_element[index].parent_].right_ = 0;


    one_element[index].parent_ = -1;
    one_element[index].type_ = -1;
    one_element[index].value_ = -1;
    one_element[index].right_ = free_;
    free_ = index;

    one_element[index].name_ = nullptr;

}

size_tree_t Tree::createNumber(Tree* tree, value_t value) {
    char name[100] = {};
    sprintf(name, "%lg", value);

    size_tree_t new_index = tree->createNewObject(name, 0, 0);
    tree->one_element[new_index].type_ = TYPE_NUMBER;
    tree->one_element[new_index].value_ = value;
    return new_index;
}

int Tree::optimisationUnusedMembers(char *text) {
    int num_optimisation = 0;
    dump();
    while (true) {
        size_tree_t branch = searchUnusedNode(this, root_);
        if (!branch) break;
        optimisationOfUnusedNode(branch);
//        recordExpression(text);
//        writeTexInText(text, index);
        strcat(text, "\n");
//        dump();
        num_optimisation++;
    }
    writeFunExplanations(text, num_optimisation);
    if (num_optimisation)
        recordExpression(text);
    return num_optimisation;
}

size_tree_t Tree::searchUnusedNode(Tree* tree, size_tree_t index) {
    size_tree_t found = 0;
    if (checkUnusedNode(tree, index))
        return index;

    if (tree->one_element[index].left_)
        found = searchUnusedNode(tree, tree->one_element[index].left_);

    if (!found && tree->one_element[index].right_)
        found = searchUnusedNode(tree, tree->one_element[index].right_);

    return found;
}

bool Tree::checkUnusedNode(Tree* tree, size_tree_t index) {
//    size_tree_t found = 0;

#define LV tree->one_element[one_element[index].left_].value_
#define RV tree->one_element[one_element[index].right_].value_
#define LT tree->one_element[one_element[index].left_].type_
#define RT tree->one_element[one_element[index].right_].type_
#define TYPE tree->one_element[index].type_
#define VALUE tree->one_element[index].value_

    if (RT == TYPE_NUMBER
        && (RV == 0 || RV == 1)

        && TYPE == TYPE_OPERATOR && VALUE == OPERATOR_POW)
        return true;

    if (LT == TYPE_NUMBER
        && (LV == 1 || RV > 0)

        && TYPE == TYPE_OPERATOR && VALUE == OPERATOR_POW)
        return true;

    if (LT == TYPE_NUMBER
        && (LV == 1 || RV > 0)

        && TYPE == TYPE_OPERATOR && VALUE == OPERATOR_POW)
        return true;

    if (((LT == TYPE_NUMBER && (LV == 1 || LV == 0))
      || (RT == TYPE_NUMBER && (RV == 1 || RV == 0)) )

        && TYPE == TYPE_OPERATOR && VALUE == OPERATOR_MUL)
        return true;

    if ((RT == TYPE_NUMBER && RV == 1)

        && TYPE == TYPE_OPERATOR && VALUE == OPERATOR_DIV)
        return true;

    if (((LT == TYPE_NUMBER && LV == 0)
      || (RT == TYPE_NUMBER && RV == 0))

        && TYPE == TYPE_OPERATOR
        && (VALUE == OPERATOR_ADD))
        return true;

    if (((RT == TYPE_NUMBER && RV == 0))

        && TYPE == TYPE_OPERATOR
        && (VALUE == OPERATOR_SUB))
        return true;

#undef TYPE
#undef VALUE
#undef LT
#undef RT
#undef LV
#undef RV
    return false;
}

void Tree::optimisationOfUnusedNode(size_tree_t index) {
    switch ((int) (one_element[index].value_ + 1e-12)) {
        case OPERATOR_ADD: case OPERATOR_SUB: {
            optimisationOfUnusedNodeAddSub(index);
            break;
        }

        case OPERATOR_MUL:
            optimisationOfUnusedNodeMul(index);
            break;

        case OPERATOR_DIV: {

            size_tree_t temp_index = one_element[index].left_;
            one_element[temp_index].parent_ = one_element[index].parent_;

            if (!one_element[index].parent_) {
                if (one_element[one_element[index].parent_].left_ == index)
                    one_element[one_element[index].parent_].left_ = temp_index;
                else
                    one_element[one_element[index].parent_].right_ = temp_index;
            }
            if (index == root_)
                root_ = temp_index;

            clearNode(one_element[index].right_);
            clearNode(index);
            break;
        }

        case OPERATOR_POW:
            optimisationOfUnusedNodePow(index);
            break;

        /*case OPERATOR_POW:
            break;*/
    }

    char name[100] = {};
    sprintf(name, "%lg", one_element[index].value_);
    int length = (int) (strlen(name) + 1);
    one_element[index].name_ = strcpy((all_names + size_names_), name);
    if (one_element[index].name_ == nullptr)
        printf("Error in strcpy\n");
    size_names_ += length;

//    one_element[index].name_ = name;

}

void Tree::optimisationOfUnusedNodeAddSub(size_tree_t index) {
    size_tree_t temp_index = 0, zero_num = 0;
    if (one_element[one_element[index].left_].type_ == TYPE_NUMBER
        && one_element[one_element[index].left_].value_ == 0) {
        temp_index = one_element[index].right_;
        zero_num = one_element[index].left_;
    } else{
        zero_num = one_element[index].right_;
        temp_index = one_element[index].left_;
    }
    one_element[temp_index].parent_ = one_element[index].parent_;

    if (one_element[one_element[index].parent_].left_ == index)
        one_element[one_element[index].parent_].left_ = temp_index;
    else
        one_element[one_element[index].parent_].right_ = temp_index;

    if (index == root_)
        root_ = temp_index;

    clearNode(index);
    clearNode(zero_num);

//    dump();
}

void Tree::optimisationOfUnusedNodeMul(size_tree_t index) {
    size_tree_t temp_index = 0, unused_index = 0;
    if ((one_element[one_element[index].left_].type_ == TYPE_NUMBER
     && one_element[one_element[index].left_].value_ == 0)
     || (one_element[one_element[index].right_].type_ == TYPE_NUMBER
        && one_element[one_element[index].right_].value_ == 0))
    {
        one_element[index].type_ = TYPE_NUMBER;
        one_element[index].value_ = 0;
        clearBranch(one_element[index].left_);
        clearBranch(one_element[index].right_);
        return;
    }

    {
    if (one_element[one_element[index].left_].type_ == TYPE_NUMBER
         && one_element[one_element[index].left_].value_ == 1) {
        temp_index = one_element[index].right_;
        unused_index = one_element[index].left_;
    } else {
        temp_index = one_element[index].left_;
        unused_index = one_element[index].right_;
    }

    one_element[temp_index].parent_ = one_element[index].parent_;

    if (one_element[one_element[index].parent_].left_ == index)
        one_element[one_element[index].parent_].left_ = temp_index;
    else
        one_element[one_element[index].parent_].right_ = temp_index;

    if (index == root_)
        root_ = temp_index;

    clearNode(index);
    clearNode(unused_index);
    return;
    }
}

void Tree::optimisationOfUnusedNodePow(size_tree_t index) {
    size_tree_t temp_index = 0, unused_index = 0;
    if ((one_element[one_element[index].right_].type_ == TYPE_NUMBER
         && one_element[one_element[index].right_].value_ == 0))
    {
        one_element[index].type_ = TYPE_NUMBER;
        one_element[index].value_ = 1;
        clearNode(one_element[index].left_);
        clearNode(one_element[index].right_);
        return;
    }

    if ((one_element[one_element[index].right_].type_ == TYPE_NUMBER
         && one_element[one_element[index].right_].value_ == 1))
    {
//        temp_index = one_element[one_element[index].parent_].left_;
//        unused_index = one_element[one_element[index].parent_].right_;
//        one_element[temp_index].parent_ = one_element[index].parent_;

        temp_index = one_element[index].left_;
        unused_index = one_element[index].right_;
        one_element[temp_index].parent_ = one_element[index].parent_;


        if (index == root_)
            root_ = temp_index;

        clearNode(index);
        clearNode(unused_index);
        return;
    }

    if ((one_element[one_element[index].left_].type_ == TYPE_NUMBER
         && one_element[one_element[index].left_].value_ == 1))
    {
        one_element[index].type_ = TYPE_NUMBER;
        one_element[index].value_ = 1;
        clearNode(one_element[index].left_);
        clearNode(one_element[index].right_);
        return;
    }
}

void Tree::writeFunExplanations(char *text, int num_action) {
    int num_phrase = 0;
    if (num_action >= 10){
        num_phrase = rand()%3;
        switch (num_phrase){
            case 0:
                strcat(text, "Очевидно, что\n");
                break;
            case 1:
                strcat(text, "Легко видеть, что\n");
                break;
            case 2:
                strcat(text, "Я сам не понял как так получилось, но вот\n");
                break;
            default:
                break;
        }
    }

    if (num_action < 10 && num_action >= 5) {
        num_phrase = rand() % 3;
        switch (num_phrase){
            case 0:
                strcat(text, "Если найденная $n$-я производная нигде не используется в дальнейшем,"
                             "то упрощать полученное выражение вряд ли имеет смысл, но мы это зачем-то делаем\n");
                break;
            case 1:
                strcat(text, "Аналогично получаем\n");
                break;
            case 2:
                strcat(text, "Из предыдущего преобразования закономерно вытекает следующее\n");
                break;
            default:
                break;
        }
    }

    if (num_action < 5 && num_action > 0) {
        num_phrase = rand() %3;
        switch (num_phrase){
            case 0:
                strcat(text, "Дальнейшие преобразования намного сложнее, так как появлется неопределённость"
                             ", решаемая с помощью Лагранжа\n");
                break;
            case 1:
                strcat(text, "Следующие преобразования от нас потребуют 3 высших образования"
                             " и наличие, как минимум, 2 красных дипломов\n");
                break;
            case 2:
                strcat(text, "Исходя из теормы 2.15 в 1 части Петровича получаем следующее\n");
                break;
            default:
                break;
        }
    }

}

void Tree::autoLengthIncrease(int factor) {
    if (size_ + 2 >= length_) {
        length_ *= factor;
        one_element = (elem*) realloc(one_element, length_*sizeof(one_element[0]));
        if (one_element){
            fillingPoisonousValues();

        } else
            printf("Error in new_adress\n");
    }

}

void Tree::autoLengthNamesIncrease(int factor) {
    if (size_names_ + 100 >= length_names_) {
        size_tree_t last_length_names = length_names_;
//        elem* last_address = one_element;
        length_names_ *= factor;
//        all_names = (char*) realloc(all_names, length_names_*sizeof(char));
        char* new_names = (char*) calloc(length_names_, sizeof(char));

        if (new_names){
            for (int i = 0; i < last_length_names; i++)
                new_names[i] = all_names[i];

//            memset(all_names + last_length_names, '\0', length_names_ - last_length_names);
            for (int i = 0; i < length_; i++) {
                if (one_element[i].name_ != nullptr)
                    one_element[i].name_ = one_element[i].name_ - all_names + new_names;
            }

            free(all_names);
            all_names = new_names;

        } else
            printf("Error in new_adress\n");
    }

}

void Tree::clearBranch(size_tree_t index) {
    if (one_element[index].left_)
        clearBranch(one_element[index].left_);
    if (one_element[index].right_)
        clearBranch(one_element[index].right_);
    clearNode(index);
}


void Tree::searchVariables(size_tree_t index) {
    if (!strcmp("x", one_element[index].name_))
        var_found.x = true;
    if (!strcmp("y", one_element[index].name_))
        var_found.y = true;
    if (!strcmp("z", one_element[index].name_))
        var_found.z = true;
    if (!strcmp("t", one_element[index].name_))
        var_found.t = true;

    if (var_found.x && var_found.y && var_found.z && var_found.t)
        return;

    if (one_element[index].left_)
        searchVariables(one_element[index].left_);

    if (one_element[index].right_)
        searchVariables(one_element[index].right_);
}

bool Tree::readTreeFromFile(const char* name_file) {
    char new_name_file[100] = {};
    strcat (new_name_file, name_file);
    strcat (new_name_file, "-win1251");

    char system_command[200] = {"iconv -f utf-8 -t windows-1251 "};
    strcat (system_command, name_file);
    strcat (system_command, " -o ");
    strcat (system_command, new_name_file);

    system(system_command);

    FILE* file = fopen(new_name_file, "r+");
    long length_of_file = ItLength(file) + 1;
    char* text = (char*) calloc(length_of_file, sizeof(char));
    fread(text, sizeof(char), length_of_file - 1, file);
    text[length_of_file - 1] = '\0';

    loadingTree(text);

    char remove_temp_file[100]= {};
    strcat (remove_temp_file, "rm ");
    strcat (remove_temp_file, new_name_file);
    strcat (remove_temp_file, "\n");
    system (remove_temp_file);
}

void Tree::mainLineView (size_tree_t index) {
    if (!strcmp(one_element[index].name_, ";")) {
            secondLineView (one_element[index].left_);
        if (one_element[index].right_)
            mainLineView (one_element[index].right_);
    }
}


void Tree::treeView (size_tree_t index) {

}

void Tree::secondLineView (size_tree_t index) {
     if (index != 0 && !functionsView (index) && !assignmentView (index))
         printf ("Error, unknown branch from main line!\n");
}

bool Tree::assignmentView (size_tree_t index) {
    if (!strcmp (one_element[index].name_, "=") && *one_element[one_element[index].right_].name_ != '$') {
        writeNameInTextCode (one_element[one_element[index].left_].name_);

        assignmentWriteInTextCode ();

        mathOperatorsView (one_element[index].right_);
//        writeNameInTextCode (one_element[one_element[index].right_].name_);
        writeNameInTextCode ("\n");
        return true;
    }

    return false;
}

bool Tree::functionsView (size_tree_t index, bool is_call) {
    if (*one_element[index].name_ == '$') {
        if (!strcmp (one_element[index].name_ + 1, "main"))
            writeNameInTextCode (name_main_function);
        else
            writeNameInTextCode (one_element[index].name_ + 1);

        writeNameInTextCode (" (");
        argumentsOfFunctionsView (one_element[index].left_);
        writeNameInTextCode (") ");

        if (!is_call){
            writeNameInTextCode (name_begin);
            writeNameInTextCode ("\n");

            num_tabs_++;
            lineOfFunctionsView (one_element[index].right_);
            num_tabs_--;

            writeNameInTextCode (name_end);
            writeNameInTextCode ("\n");
        }

//        lineOfFunctionsView (one_element[index].right_);
        return true;
    }

    return false;
}

void Tree::writeConvertCode (const char* name_file) {
    char *text = (char*) calloc (50000, sizeof(char));
    write_text_ = text;
    num_write_text_ = 0;
    mainLineView (root_);

    FILE* file_code = fopen (name_file, "wb");
    fwrite (text, sizeof (char), num_write_text_, file_code);
    fclose (file_code);
}

void Tree::writeNameInTextCode (char* name) {
    int num_write = 0;

    if (last_str_have_n_)
        for (int i = 0; i < num_tabs_; i++) {
            sprintf (write_text_ + num_write_text_, "%c", '\t');
            num_write_text_++;
        }

    sprintf (write_text_ + num_write_text_, "%s%n", name, &num_write);
    num_write_text_ += num_write;
    num_write = 0;
    last_str_have_n_ = (bool) strchr (name, '\n');
}

void Tree::writeNameInTextCode (const char* name) {
    writeNameInTextCode ((char*) name);
}

void Tree::argumentsOfFunctionsView(size_tree_t index, bool first) {
    if (index != 0) {
        if (!first)
            writeNameInTextCode (" ");
        writeNameInTextCode (one_element[one_element[index].right_].name_);

        argumentsOfFunctionsView (one_element[index].left_, false);
    }
}

void Tree::lineOfFunctionsView(size_tree_t index) {
    if (index) {
        if (strcmp(one_element[index].name_, "op") != 0)
            printf ("Error in operators of functions!\n");

        operatorsView (one_element[index].left_);

        lineOfFunctionsView (one_element[index].right_);
    }
}

void Tree::operatorsView(size_tree_t index) {
    if (index == 0)
        printf ("Error, \"op\" haven't left knot!\n");
    else {
        if (callFunctionsView (index))
            return;
        if (assignmentView (index))
            return;
        if (operatorIfView (index))
            return;
        if (operatorPutView (index))
            return;
        if (operatorGetView (index))
            return;
        if (operatorWhileView (index))
            return;
        if (operatorReturnView (index))
            return;
    }
}

bool Tree::callFunctionsView (size_tree_t index) {
    if (!strcmp(one_element[index].name_, "=") && *one_element[one_element[index].right_].name_ == '$') {
        writeNameInTextCode (one_element[one_element[index].left_].name_);

        assignmentWriteInTextCode ();

        functionsView (one_element[index].right_, true);
        writeNameInTextCode ("\n");
        return true;
    }

    if (*one_element[index].name_ == '$') {
        functionsView (index, true);
        writeNameInTextCode ("\n");
        return true;
    }
    return false;
}

bool Tree::operatorIfView(size_tree_t index) {
    if (!strcmp (one_element[index].name_, "if")) {
        writeNameInTextCode (name_if);
        writeNameInTextCode (" ");
        compareView (one_element[index].left_);
        writeNameInTextCode (" ");

        num_tabs_++;
        allResultIfView (one_element[index].right_);
        num_tabs_--;

//        writeNameInTextCode ("\n");
    }
    return false;
}

void Tree::compareView(size_tree_t index) {
    writeNameInTextCode ("(");
    writeNameInTextCode (one_element[one_element[index].left_].name_);
    writeNameInTextCode (" ");
    if (!strcmp (one_element[index].name_, "=="))
        writeNameInTextCode (name_equaly);
    if (!strcmp (one_element[index].name_, "<"))
        writeNameInTextCode (name_less);
    if (!strcmp (one_element[index].name_, ">"))
        writeNameInTextCode (name_more);
    if (!strcmp (one_element[index].name_, ">="))
        writeNameInTextCode (name_more_equaly);
    if (!strcmp (one_element[index].name_, "<="))
        writeNameInTextCode (name_less_equaly);

    writeNameInTextCode (" ");
    writeNameInTextCode (one_element[one_element[index].right_].name_);
    writeNameInTextCode (")");

}

void Tree::assignmentWriteInTextCode() {
    writeNameInTextCode (" ");
    writeNameInTextCode (name_assignment);
    writeNameInTextCode (" ");
}

bool Tree::oneMathOperatorView (size_tree_t index, const char* name, size_tree_t type,
        value_t value, const char* real_name, int last_priority) {
    if (!strcmp (one_element[index].name_, name)) {
        one_element[index].type_  = type;
        one_element[index].value_ = value;
        int now_priority = priorityFunction (index);
        if (last_priority > now_priority) {
            writeNameInTextCode ("(");
            mathOperatorsView (one_element[index].left_, now_priority);
            writeNameInTextCode (" ");
            writeNameInTextCode (real_name);
            writeNameInTextCode (" ");
            mathOperatorsView (one_element[index].right_, now_priority);
            writeNameInTextCode (")");
        } else {
            mathOperatorsView (one_element[index].left_, now_priority);
            writeNameInTextCode (" ");
            writeNameInTextCode (real_name);
            writeNameInTextCode (" ");
            mathOperatorsView (one_element[index].right_, now_priority);
        }
        return true;
    }
    return false;
}

void Tree::mathOperatorsView (size_tree_t index, int last_priority) {
    if (!index)
        return;
    if (oneMathOperatorView (index, "+",   TYPE_OPERATOR, OPERATOR_ADD, name_addition,       last_priority))
        return;
    if (oneMathOperatorView (index, "-",   TYPE_OPERATOR, OPERATOR_SUB, name_subtraction,    last_priority))
        return;
    if (oneMathOperatorView (index, "*",   TYPE_OPERATOR, OPERATOR_MUL, name_multiplication, last_priority))
        return;
    if (oneMathOperatorView (index, "/",   TYPE_OPERATOR, OPERATOR_DIV, name_division,       last_priority))
        return;
    if (oneMathOperatorView (index, "^",   TYPE_OPERATOR, OPERATOR_POW, name_power,          last_priority))
        return;
    if (oneMathOperatorView (index, "sin", TYPE_OPERATOR, OPERATOR_SIN, "sin",               last_priority))
        return;
    if (oneMathOperatorView (index, "cos", TYPE_OPERATOR, OPERATOR_MUL, "cos",               last_priority))
        return;

    if (!strcmp (one_element[index].name_, "deriv")) {
        operatorDerivView (index);
        return;
    }

    if (*one_element[index].name_ == '$') {
        callFunctionsView (index);
        return;
    }
    writeNameInTextCode (one_element[index].name_);
}

void Tree::operatorDerivView(size_tree_t index) {
    writeNameInTextCode ("d(");
    mathOperatorsView (one_element[index].right_, 3);
    writeNameInTextCode (")/d ");
    writeNameInTextCode (one_element[one_element[index].left_].name_);

}

void Tree::allResultIfView(size_tree_t index) {
    if (!one_element[index].left_)
        printf ("Error, \"if\" haven't main result");

   resultIfView (one_element[index].left_);

    if (one_element[index].right_) {
        num_tabs_--;
        writeNameInTextCode (name_if_else);
        num_tabs_++;

        resultIfView (one_element[index].right_);

    }
}

void Tree::resultIfView (size_tree_t index) {
//    if (!index) return;
    if (one_element[index].right_ == 0){
        writeNameInTextCode ("\n");
        lineOfFunctionsView (index);
    } else {
        writeNameInTextCode (name_begin);
        writeNameInTextCode ("\n");

        lineOfFunctionsView (index);
        num_tabs_--;
        writeNameInTextCode (name_end);
        num_tabs_++;
        writeNameInTextCode ("\n");
    }
}

bool Tree::operatorPutView (size_tree_t index) {
    if (!strcmp (one_element[index].name_, "put")) {
        writeNameInTextCode (name_put);
        writeNameInTextCode (" ");
        writeNameInTextCode (one_element[one_element[index].left_].name_);
        writeNameInTextCode ("\n");

        return true;
    }
    return false;
}

bool Tree::operatorGetView (size_tree_t index) {
    if (!strcmp (one_element[index].name_, "get")) {
        writeNameInTextCode (name_get);
        writeNameInTextCode (" ");
        writeNameInTextCode (one_element[one_element[index].left_].name_);
        writeNameInTextCode ("\n");

        return true;
    }
    return false;
}

bool Tree::operatorWhileView (size_tree_t index) {
    if (!strcmp (one_element[index].name_, "while")) {
        writeNameInTextCode (name_while);
        writeNameInTextCode (" ");
        compareView (one_element[index].left_);
        writeNameInTextCode (" ");

        num_tabs_++;
        resultIfView (one_element[index].right_);
        num_tabs_--;

        writeNameInTextCode ("\n");
    }
    return false;
}

bool Tree::operatorReturnView(size_tree_t index) {
    if (!strcmp (one_element[index].name_, "ret")) {
        writeNameInTextCode (name_return);
        writeNameInTextCode (" ");
//        writeNameInTextCode (one_element[one_element[index].left_].name_);
        mathOperatorsView (one_element[index].left_);
        writeNameInTextCode ("\n");
        return true;
    }
    return false;
}



