/**
 * This program reads a text file and prints the top 5 most frequent letters (upper or lower case)
 * and the top 5 most frequent digits that are in the file. If the file has less than 5 unique letters
 * and/or digits the top list is adjusted accordingly. The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Armon White {@literal <whitac19@wfu.edu>}
 * @date Sept. 29, 2021
 * @assignment Lab 3
 * @course CSC 250
 **/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 20

/* a useful struct, if you choose to use it  */  
struct WordFreq {
    char* word;
    int count;
};

/* function declarations go here */

int process_characters(struct WordFreq *listPtr[], char filename[], int *wordListPtr);
void add_word(struct WordFreq *list_ptr[], int *listPtr, char buffer[], int bufferSize);
void processWord (struct WordFreq *listPtr[], char buffer[], int *bSize, int *wordListPtr);
int wordSearch(struct WordFreq wList[], char word[], int listSize);
void sortWords(struct WordFreq wList[], int listSize);
void swapWords(struct WordFreq wList[], int i1, int i2);
void print(char filename[], int listSize);
void outputToFile(struct WordFreq wList[], char filename[], int listSize);


int main(int argc, char* argv[]){
    struct WordFreq *list = NULL;
    int listSize = 0;
    int fileOK = 1;

    if(argc < 3){
        printf("Usage: %s filename frequency_file \n", argv[0]);
        return 1;
    }

    fileOK = process_characters(&list, argv[1], &listSize);

    if(!fileOK){
        printf("%s could not open file %s \n", argv[0], argv[1]);
        return 2;
    }
    sortWords(list, listSize);
    outputToFile(list, argv[2], listSize);
    print(argv[1], listSize);

    for(int i = 0; i < listSize; i++){
        if(list[i].word){
            free(list[i].word);
        }
    }
    if(list){
        free(list);
        }

    return 0;
}


int process_characters(struct WordFreq *listPtr[], char filename[], int *wordListPtr){
    char ch;
    char buffer[MAX_STRING_SIZE];
    FILE *filePtr = fopen(filename, "rie");
    int bSize = 0;
    buffer[0] = '\0';
    int loc;
   
    if(filePtr == 0) {
        printf("could not open %s \n", filename);
        return 0;
    }

    ch = fgetc(filePtr);
    while(ch != EOF) {
        if(isalpha(ch)) {
            if (!(bSize < 19)){            /*(!(bSize > 19))*/
                ch = tolower(ch);
                buffer[bSize] = ch;
                bSize++;
                buffer[bSize] = '\0';
            }
            else{
                processWord(listPtr, buffer, &bSize, wordListPtr);
            }         
            
        }
        else{
            /*printf("cString %s \n", buffer);*/
            /* store the word (C-string in buffer) in dynamic list */
            /*buffer[0] = '\0';*/
            /*n = 0;*/
            processWord(listPtr, buffer, &bSize, wordListPtr);
        }
        ch = fgetc(filePtr);
    }
    processWord(listPtr, buffer, &bSize, wordListPtr);

    fclose(filePtr);
    return 1;
}

void add_word(struct WordFreq *list_ptr[], int *listPtr, char buffer[], int bufferSize) {
    int i;
    /*char new_word[] = wordAdd;           just a word to add to the list */
    struct WordFreq *temp_list = (struct WordFreq *)malloc((*listPtr + 1) * sizeof(struct WordFreq)); /* creating temp list for copying */
    


    /* copy the old list to temp_list */
    for(i = 0; i < *listPtr; i++) {
        temp_list[i] = (*list_ptr)[i];
    }

    /* let's add a new word to the end of the new list */
    /* make space to store the new word */
    temp_list[i].word = (char *) malloc(strlen(buffer) + 1);
    /* copy the word over */
    strcpy(temp_list[i].word, buffer);
    temp_list[i].count = 1;

    /* let's free the old list (no memory leaks) */
    if(*list_ptr) free(*list_ptr);
    /* let's point to the new, larger list */
    *list_ptr = temp_list;
    /* added one more word, increase the count */
    (*listPtr)++;
}

void processWord (struct WordFreq *listPtr[], char buffer[], int *bSize, int *wordListPtr){
    int loc;

    if (*bSize == 0){
        return;
    }

    loc = wordSearch(*listPtr, buffer, *wordListPtr);

    if (loc >= 0){
        (*listPtr)[loc].count++;
    }
    else{
        add_word(listPtr, wordListPtr, buffer, *bSize);
    }
    buffer [0] = '\0';
    *bSize = 0;
}

int wordSearch(struct WordFreq wList[], char word[], int listSize){
    int loc;

    while(loc < listSize){
        if(strncmp(word, wList[loc].word, MAX_STRING_SIZE) == 0){
            return loc;
        }
        loc++;
    }
    return -1;
}

void sortWords(struct WordFreq wList[], int listSize){
    for (int i = 0; i < listSize; i++){
        for (int j = 0; j < listSize - 1; i++){
            if(wList[j].count < wList[j+1].count){
                swapWords(wList, j, j+1);
            }
        }
    }
}

void swapWords(struct WordFreq wList[], int i1, int i2){
    struct WordFreq temp = wList[i1];
    wList[i1] = wList[i2];
    wList[i2] = temp;
}

void print(char filename[], int listSize){
    printf("%s has %d unique words\n", filename, listSize);
}

void outputToFile(struct WordFreq wList[], char filename[], int listSize){
    int i;
    FILE *filePtr = fopen(filename, "w");

    for (int i = 0; i < listSize; i++){
        fprintf(filePtr, "%s %d\n", wList[i].word, wList[i].count);
    }

    fclose(filePtr);
}