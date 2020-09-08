//
// Created by Texnar on 21.05.2020.
//

#include "MyTree.h"

long Tree::itLength(FILE* file)
{
    assert (file != nullptr);

    fseek (file, 0, SEEK_END);
    long result = ftell (file);
    fseek (file, 0, SEEK_SET);

    return result;
}

void Tree::readTreeFromFile(char *name_file) {
    if (name_file == nullptr) {
        printf ("File name not received!\n");
        abort ();
    }

    char* text = readTextFromFile (name_file);
    loadingTree (text);
}

char* Tree::readTextFromFile (char* name_file) {
    assert (name_file != nullptr);

    FILE* file = fopen (name_file, "rb");
    assert (file != nullptr);

    long length_of_file = itLength (file) + 1;

    char* text = (char*) calloc (length_of_file, sizeof (char));
    fread (text, sizeof (char), length_of_file - 1, file);
    text[length_of_file - 1] = '\0';

    fclose (file);
    return text;
}

void Tree::loadingTree (char* text) {
    assert(text != nullptr);

    if (((unsigned char) text[0] == 0xEF)       //  '0xEF 0xBB 0xBF' - it is special character
     && ((unsigned char) text[1] == 0xBB)       //  in start file of encoding UTF-8
     && ((unsigned char) text[2] == 0xBF))
        text += 3;

    readTextTree (text);
}

void Tree::readTextTree (char *read_now)
{
    if (*read_now == '{') {
        read_now ++;
        readNewObject (&read_now);
    }
    else
        printf("Error in read!\n");
}

tree_st Tree::readNewObject (char **read_now)
{
    tree_st left_temp  =  0,
            right_temp =  0;
    char    name[100]  = {};

    readName (read_now, name);
    left_temp  = readNewObjectBranch (read_now);
    right_temp = readNewObjectBranch (read_now);

    if (**read_now == '}') {
        root_ = createNewObject (name, left_temp, right_temp);
        return root_;
    }
    else {
        printf("Error, don't found end of object!\n");
        abort();
    }
}

tree_st Tree::readNewObjectBranch (char** read_now) {
    if (**read_now == '@'){
        (*read_now)++;
        return 0;
    }
    else if (**read_now == '{') {
        (*read_now)++;
        tree_st temp = readNewObject (read_now);

        assert (**read_now == '}');
        (*read_now)++;
        return temp;
    } else
        return 0;
}


void Tree::readName (char **read_now, char *name) {
    int  read_symbol = 0;
    char num_read    = 0;

    if (haveQuotes (read_now))
        num_read = (char) sscanf (*read_now, "\"%[^\"]\"%n",   name,  &read_symbol);    // write name if use text quotes
    else
        num_read = (char) sscanf (*read_now, "%[^{}@ \n\t]%n", name,  &read_symbol);    // write name

    if (num_read == 0)
        printf ("Can't read name\n");
    *read_now += read_symbol;
}

bool Tree::haveQuotes(char **read_now) {
    char symbol[3] = {};
    assert (*read_now > (void*) 0x1000);
    return sscanf_s (*read_now, "%[\"]", symbol, 3) != 0;
}

tree_st Tree::createNewObject (char    name[],
                         tree_st left  ,
                         tree_st right ,
                         tree_st parent) {
    autoLengthIncrease();
    autoLengthNamesIncrease();

    tree_st new_index = free_;
    free_ = node_[free_].right;

    node_[new_index].right = right;
    node_[new_index].left = left;
    node_[new_index].name = all_names_ + size_names_;

    if (node_[new_index].left)
        node_[node_[new_index].left].parent = new_index;
    if (node_[new_index].right)
        node_[node_[new_index].right].parent = new_index;

    if (node_[new_index].parent == -1)
        node_[new_index].parent = parent;


    int length = (int) (strlen(name) + 1);
    strcpy((all_names_ + size_names_), name);
    size_names_ += length;

    size_++;
    return new_index;
}

void Tree::autoLengthIncrease(int factor) {
    if (size_ + 2 >= length_) {
        length_ *= factor;
        node_ = (Node*) realloc (node_, length_ * sizeof(node_[0]));
        if (node_){
            fillingPoisonousValues();

        } else
            printf("Error in new_address\n");
    }

}

void Tree::fillingPoisonousValues() {
    for (tree_st i = size_ + 1; i < length_; i++){
        node_[i].right = i + 1;
        node_[i].parent = -1;
        node_[i].type = -1;
        node_[i].value = -1;
        node_[i].name = nullptr;
    }
    node_[length_ - 1].right = 0;
    free_ = size_ + 1;
}

void Tree::autoLengthNamesIncrease (int factor) {
    if (size_names_ + 100 >= length_names_) {
        tree_st last_length_names = length_names_;
        length_names_ *= factor;
        char* new_names = (char*) calloc (length_names_, sizeof(char));

        if (new_names){
            for (int i = 0; i < last_length_names; i++)
                new_names[i] = all_names_[i];

            for (int i = 0; i < length_; i++) {
                if (node_[i].name != nullptr)
                    node_[i].name = node_[i].name - all_names_ + new_names;
            }

            free(all_names_);
            all_names_ = new_names;

        } else
            printf("Error in new_address\n");
    }

}

Tree::Tree (tree_st DEFAULT_LENGTH      ,
            tree_st DEFAULT_LENGTH_NAMES):
        node_         ((Node*) calloc (DEFAULT_LENGTH,       sizeof(node_[0]))),
        all_names_    ((char*) calloc (DEFAULT_LENGTH_NAMES, sizeof(char))    ),
        size_         (0),
        size_names_   (0),
        root_         (0),
        free_         (0),
        length_       (DEFAULT_LENGTH),
        length_names_ (DEFAULT_LENGTH_NAMES)
{
    fillingPoisonousValues();
}

Tree::~Tree() {
    free (node_);
    free (all_names_);
    size_       = 0;
    length_     = 0;
    size_names_ = 0;
    root_       = 0;
    free_       = 0;
}


void Tree::dump (const char* name_file, void (* colorFunction) (FILE*, Node*))
{
    FILE* file = fopen (name_file, "wb");
    if (!file) {
        printf ("Can't open file in function dump!\n");
        exit (2);
    }
    fprintf (file, "digraph structs {\n");
    fprintf (file, "rankdir=HR;\n");


    for (tree_st i = 1; i < length_; i++){
        if (node_[i].parent != -1){
            if (*(node_[i].name) == '>' || *(node_[i].name) == '<')
                fprintf(file, "Index%d [shape=record, label=\" <left>  %d | {'\\%s' | Par: %d} | {Index: %d | Type: %d | Value: %lld} | <right> %d \",",
                        i, node_[i].left, node_[i].name, node_[i].parent, i, node_[i].type, node_[i].value, node_[i].right);
            else
                fprintf(file, "Index%d [shape=record, label=\" <left>  %d | {'%s' | Par: %d} | {Index: %d | Type: %d | Value: %lld} | <right> %d \",",
                        i, node_[i].left, node_[i].name, node_[i].parent, i, node_[i].type, node_[i].value, node_[i].right);

            colorFunction (file, &(node_[i]));
            fprintf(file, "];\n");

        }
    }
    fprintf(file, "System [shape=record, label=\" <root> Root\" ];\n");
    fprintf(file, "System: <root> -> Index%d;\n", root_);

    for (tree_st i = 1; i < length_; i++) {
        if (node_[i].parent != -1) {
            if (node_[i].left != 0)
                fprintf(file, "Index%d: <left> -> Index%d ;\n", i, node_[i].left);
            if (node_[i].right != 0)
                fprintf(file, "Index%d: <right> -> Index%d ;\n", i, node_[i].right);
        }
    }

    fprintf(file, "}\n");
    fclose(file);

//    system("iconv -f windows-1251 -t utf-8 ../logs/text_picture.dot -o ../logs/text_picture_utf8.dot");

    char name_png_dump[20]  = {};
    strcat (name_png_dump, name_file);
    char* pointer_on_dot = strrchr (name_png_dump, '.');
    strcpy (pointer_on_dot, ".png");

    char system_command[100] = "dot text_picture_refactor.dot -T png -o ";
    strcat (system_command, name_png_dump);
    system (system_command);
}

void Tree::fullVisit (void (*func)(Node* node)) {
    visit (root_, func);
}

void Tree::visit (tree_st index, void (*func)(Node* node)) {
    if (node_[index].left )     visit (node_[index].left,  func);    // Visiting all node in tree_ and
    func (&(node_[index]));                                          // tree_ doesn't know what function we use.
    if (node_[index].right)     visit (node_[index].right, func);
}

void Tree::controlledExternalFunction (tree_st index, int (*func)(Node* node)) {
//#define CHECK_INDEX(index, branch) \
//    if (node_[index].branch <= 0) {\
//        printf ("Attempt to enter an empty node from node with index: %d in " #branch "\n", index);\
//        abort();\
//    }
    if (index <= 0) return;
    int return_value = func (&node_[index]);

    if (return_value == ONLY_LEFT_BRANCH ||
        return_value == BOTH_BRANCH)
    {
//        CHECK_INDEX (index, left)
        controlledExternalFunction (node_[index].left, func);
    }

    if (return_value == ONLY_RIGHT_BRANCH ||
        return_value == BOTH_BRANCH)
    {
//        CHECK_INDEX (index, right)
        controlledExternalFunction (node_[index].right, func);
    }

#undef CHECK_INDEX
}

void Tree::controlledExternalFunctionFromRoot (int (*func)(Node* node)) {
    controlledExternalFunction (root_, func);
}

tree_st Tree::getRoot() {
    return root_;
}

Node* Tree::getNodes() {
    return node_;
}