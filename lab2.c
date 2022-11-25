/**
 * This program reads a text file and prints the top 5 most frequent letters (upper or lower case)
 * and the top 5 most frequent digits that are in the file. If the file has less than 5 unique letters
 * and/or digits the top list is adjusted accordingly. The file name is provided using command line 
 * arguments. If the file name is not provided or the file is not readable, the program will exit
 * and provide an error message.
 *
 * @author Armon White {@literal <whitac19@wfu.edu>}
 * @date Sept. 5, 2021
 * @assignment Lab 2
 * @course CSC 250
 **/

#include <ctype.h>
#include <stdio.h>
#define MAX_STRING_SIZE 20
#define MAX_LIST_SIZE 36

/* a useful struct, if you choose to use it */  
struct Char_Freq {
    char ch;
    int count;
};

/* function declarations*/
int process_characters (char filename[], struct Char_Freq aList[], int *aPtr, struct Char_Freq dList[], int *dPtr);
int search(char c, struct Char_Freq[], int a);
int sort(struct Char_Freq *a, int size);
void print(struct Char_Freq aList[], int aNum, struct Char_Freq dNum[], int dList);

int main(int argc, char* argv[]){
    struct Char_Freq aList[MAX_LIST_SIZE];//initializing structs and variables
    struct Char_Freq dList[MAX_LIST_SIZE];
    int aNum = 0;
    int dNum = 0;
    int fileOK = 1;

    if(argc < 2){
        printf("Usage: %s filename \n", argv[0]);
        return 1;
    }

    fileOK = process_characters(argv[1], aList, &aNum, dList, &dNum);//retrieves file

if (!fileOK){//exception catcher
    printf("%s could not open file %s \n", argv[0], argv[1]);
    return 2;
}

print(aList, aNum, dList, dNum);

    return 0;
}


int process_characters (char filename[], struct Char_Freq aList[], int *aPtr, struct Char_Freq dList[], int *dPtr){
    char ch; /*every character out of the file*/
    FILE* file_ptr = fopen(filename, "rie"); /*references the file*/
    int aNum = 0;
    int dNum = 0;
    int loc;

    if (file_ptr == 0){
        return 0;
    }
    ch = fgetc(file_ptr);//gets file
    while (ch != EOF){//goes through loop until end of the file
        if(isalpha(ch)){//checks if character is a letter and accodingly counts and puts letter into arraylist
            ch = tolower(ch);
            loc = search(ch, aList, aNum);
            if(loc != -1){
                aList[loc].count++;
            }else if(loc == -1) {
                aList[aNum].ch = ch;
                aList[aNum].count = 1;
                aNum++;
            }
        }
        else if (isdigit(ch)){//checks if character is a number and accodingly counts and puts number into arraylist
            loc = search(ch, dList, dNum);
            if(loc != -1){
                dList[loc].count++;
            }else if(loc == -1) {
                dList[dNum].ch = ch;
                dList[dNum].count = 1;
                dNum++;
        }
        }
        ch = fgetc(file_ptr);
    }
    fclose(file_ptr);//closes file
    *aPtr = aNum;
    *dPtr = dNum;


    sort(aList, aNum);
    sort(dList, dNum);
    
 return -1;
}



int search(char c, struct Char_Freq aList[], int a){//searches arraylist
 for (int i = 0; i < a; i++){
        if (aList[i].ch == c){
            return i;
        }
 }
 return -1;
}

int sort (struct Char_Freq *list, int s){//sorts arraylist
    struct Char_Freq temp;
    for (int i = 0; i < s-1; i++){
        for (int j = 0; j < s-1-i; j++){
            if (list[j].count < list[j+1].count){
                temp = list[j];
                list[j] = list[j+1];
                list[j+1] = temp;
            }
        }
    }
    return 0;
}

void print(struct Char_Freq aList[], int aNum, struct Char_Freq dList[], int dNum){//prints an formats output
    printf("alpha top 5 \n");
    printf("------------------- \n");
    for (int i = 0; i < aNum && i < 5; i++){
    printf("[%c], %d \n", aList[i].ch, aList[i].count);
}

    printf("\n");
    printf("digit top 5 \n");
    printf("------------------- \n");
for (int i = 0; i < dNum && i < 5; i++){
    printf("[%c], %d \n", dList[i].ch, dList[i].count);
}
}