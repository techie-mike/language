//
// Created by texnar on 16/12/2019.
//

#include "NameTable.h"

nameTable::nameTable():
        var            (nullptr),
        num_arguments_ (0),
        size_          (0),
        all_names_     (nullptr),
        size_names_    (0),
        length_        (0),
        length_names_  (0),
        free_          (0)

{
    length_names_ = DEFAULT_LENGTH_NAMES;
    length_       = DEFAULT_LENGTH;
    var           = (element*) calloc (sizeof (element), DEFAULT_LENGTH);
    all_names_    = (char*)    calloc (sizeof (char),    DEFAULT_LENGTH_NAMES);

    if (!var || !all_names_) {
        printf ("Error in calloc nameTable!");
    }
}

nameTable::~nameTable() {
    free (var);
    free (all_names_);
}

void nameTable::autoLengthIncrease (int factor) {
    if (size_ + 2 >= length_) {
        length_ *= factor;
        var = (element*) realloc (var, length_ * sizeof (element));
        if (var){
            fillingPoisonousValues();

        } else
            printf("Error in new_address\n");
    }

}

void nameTable::autoLengthNamesIncrease (int factor) {
    if (size_names_ + 20 >= length_names_) {
        length_names_ *= factor;
        char* new_names = (char*) calloc (length_names_, sizeof(char));

        if (new_names) {
            for (ntable_t i = 0; i < size_names_; i++)
                new_names[i] = all_names_[i];

            for (ntable_t i = 0; i < size_; i++) {
                if (var[i].name != nullptr)
                    var[i].name = var[i].name - all_names_ + new_names;
            }
            free (all_names_);
            all_names_ = new_names;
        } else
            printf("Error in new_address\n");
    }

}

void nameTable::fillingPoisonousValues()
{
    for (ntable_t i = size_ + 1; i < length_; i++){
        var[i].position_object = 0;
        var[i].free_places     = 0;
        var[i].state           = false;
        var[i].name            = nullptr;
    }
}

ntable_t nameTable::searchNameInTable (const char* name) {
    for (ntable_t i = 0; i < size_; i++) {
        if (!strcmp (name, var[i].name))
            return i;
    }
    return -1;
}

void nameTable::createNameInTable (char* name) {
    createNameInTable ((const char*) name);
}

void nameTable::createNameInTable (const char* name) {
    autoLengthNamesIncrease ();
    autoLengthIncrease ();
    var[free_].name = all_names_ + size_names_;
    strcpy (all_names_ + size_names_, name);
    size_names_ += strlen (name) + 1;

    size_++;
    free_++;
}

void element::loadNewDependedPosition (ntable_t new_position) {
    if (DEFAULT_LENGTH_OF_ARRAY_POINTER <= free_places + 1) {
        printf ("Error, not enough free space for position_depended for element in nameTable!\n");
        assert (DEFAULT_LENGTH_OF_ARRAY_POINTER >= free_places + 1);
    }
    position_depended[free_places] = new_position;
    free_places++;
}