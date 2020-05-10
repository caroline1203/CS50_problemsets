// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

unsigned long hash(const char *word);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 15505;

// Hash table
node *table[N];

//words' number
int wordsSize = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char test[strlen(word) + 1];
    for (int i = 0; word[i]; i++)
    {
        test[i] = tolower(word[i]);
    }
    test[strlen(word)] = '\0';

    int index = hash(test) % N;
    node *temp = table[index];


    while (temp != NULL)
    {
        if (strcasecmp(temp -> word, word) == 0)
        {
            return true;
        }
        temp = temp -> next;
    }


    return false;
}

// Hashes word to a number
//use djb2 hash function, refer to http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open file
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        printf("could not load");
        return false;
    }

    //read the words from the file
    char buffer[LENGTH + 1];

    while (fscanf(file, "%s", buffer) != EOF)
    {
        //copy the word, make a node
        node *newNode = malloc(sizeof(node));
        strcpy(newNode -> word, buffer);

        int index = hash(buffer) % N;

        if (table[index] == NULL)
        {
            table[index] = newNode;
            newNode -> next = NULL;
        }
        else
        {
            newNode -> next = table[index];
            table[index] = newNode;
        }
        wordsSize++;
    }
    fclose(file);
    return true;

}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return wordsSize;
}



// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //make table empty
    for (int i = 0; i < N; i++)
    {
        node *cur = table[i];
        while (cur != NULL)
        {
            node *dele = cur;
            cur = cur -> next;
            free(dele);
        }
    }
    return true;
}


