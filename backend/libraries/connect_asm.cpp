//
// Created by texnar on 16/12/2019.
//

#include "connect_asm.h"

nameTable::nameTable():
    var (nullptr),
    length_ (0),
    size_ (0),
    size_names_ (0),
    free_ (0),
    all_names_ (nullptr)

{
    length_names_ = DEFAULT_LENGTH_NAMES;
    length_ = DEFAULT_LENGTH;
    var = (variable*) calloc (sizeof (variable), DEFAULT_LENGTH);
    if (!var)
        printf ("Error in calloc nameTable!");

    all_names_ = (char*) calloc (sizeof (char), DEFAULT_LENGTH_NAMES);
    if (!var)
        printf ("Error in calloc nameTable!");
}

nameTable::~nameTable() {
    free (var);
    free (all_names_);
}

void nameTable::autoLengthIncrease(int factor) {
    if (size_ + 2 >= length_) {
        length_ *= factor;
        var = (variable*) realloc (var, length_ * sizeof(variable));
        if (var){
            fillingPoisonousValues();

        } else
            printf("Error in new_adress\n");
    }

}

void nameTable::autoLengthNamesIncrease(int factor) {
    if (size_ + 50 >= length_names_) {
        ntable_t last_length_names = length_names_;
        length_names_ *= factor;
//        all_names_ = (char*) realloc(all_names_, length_names_*sizeof(char));
        char* new_names = (char*) calloc (length_names_, sizeof(char));

        if (new_names) {
            for (int i = 0; i < size_names_; i++)
                new_names[i] = all_names_[i];

            for (int i = 0; i < size_; i++) {
                if (var[i].name != nullptr)
                    var[i].name = var[i].name - all_names_ + new_names;
            }

            free (all_names_);
            all_names_ = new_names;

        } else
            printf("Error in new_adress\n");
    }

}

void nameTable::fillingPoisonousValues()
{
    for (ntable_t i = size_ + 1; i < length_; i++){
        var[i].state = false;
        var[i].name = nullptr;
    }
}

ntable_t nameTable::searchNameInTable(char* name) {
    for (ntable_t i = 0; i < size_; i++) {
        if (!strcmp (name, var[i].name))
            return i;
    }
    return -1;
}

void nameTable::createNameInTable(char* name) {
    autoLengthNamesIncrease ();
    autoLengthIncrease ();
    var[free_].name = all_names_ + size_names_;
    strcpy (all_names_ + size_names_, name);
    size_names_ += strlen (name) + 1;

    size_++;
    free_++;
}

void nameTable::createNameInTable(const char* name) {
    createNameInTable ((char*) name);
}
