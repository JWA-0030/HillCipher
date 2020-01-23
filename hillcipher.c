// Jesse Alsing
// 6/5/2017
// CIS 3360 Homework 1
// Hill Cipher

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <strings.h>

//just some constants to use for ease of time on my part and we were given a max array length of 10000
#define MAX_FILENAME_LENGTH 20
#define MAX_ARRAY_SIZE 10000

int **initArray(int size);
void file_to_array(FILE *ifp_input, int keyGridSize);
void printarray(char myArray[]);
void printMatrix(int **matrix, int rows, int columns);
int** matrixMultiply(int** a, int keyGridSize, int** b, int r2, int c2);
void encrypt(int** matrix, int keyGridSize);


char myArray[10000];

int main(int argc, char** argv){

    //Encryption key file and file to be encrypted, respectfully
    FILE *ifp_key, *ifp_input;

     //Error and return if it doesn't open the files from the cmd line
    if(argc != 3){
        printf("Error!\n");
        printf("%d\n", argc);
        return 0;
    }
    //read in from cmd line and open in read-only mode
    ifp_key = fopen(argv[1], "r");
    ifp_input = fopen(argv[2], "r");

    //Takes care of the case where the file could not be opened.
    if(ifp_key == NULL || ifp_input == NULL){
        printf("Error! Could not open file!");
        return 0;
    }

    int keyGridSize = 0;
    //first we read in the size of the grid matrix
    fscanf(ifp_key, "%d", &keyGridSize);
    //initializes the keymatrix
    int **keyMatrix = initArray(keyGridSize);
    int i, j;
    //now we scan in the key grid
    for(i = 0; i < keyGridSize; i++){
        for(j = 0; j < keyGridSize; j++){
            fscanf(ifp_key, "%d", &keyMatrix[i][j]);
        }
    }
    //here we output the key matrix to the cmd line
    printf("Key Matrix:\n\n");
    printMatrix(keyMatrix, keyGridSize, keyGridSize);
    printf("\n\n");
    //here we read in the file t oencrypt and then encrypt it
    file_to_array(ifp_input, keyGridSize);
    encrypt(keyMatrix, keyGridSize);

    //Cleanup
    fclose(ifp_key);
    fclose(ifp_input);
    return 0;
}

void file_to_array(FILE *ifp_input, int keyGridSize){
    int temp = 0;
    int c;
    //this makes sure that we only read in valid characters and read them into an array of characters
    while((c = fgetc(ifp_input)) != EOF){
        if(isalpha(c)){
            myArray[temp] = tolower(c);
            temp++;
        }
    }

    //Appends an "x" onto the end to ensure a valid matrix multiplication
    while((strlen(myArray) % keyGridSize) != 0){
        strcat(myArray, "x");
    }
    //here we print the resulting array of chars with no spaces and all lower case
    printf("Plaintext:\n\n");
    printarray(myArray);
    printf("\n\n");
}

void printarray(char *myArray){

    int i;

    for(i = 0; i < strlen(myArray); i++){
        printf("%c", myArray[i]);
        //this ensures that only 80 chars are printed on a line
        if(((i + 1) % 80) == 0)
            printf("\n");
    }
}

void printMatrix(int **matrix, int rows, int columns){
    int i, j;

     for(i = 0; i < rows; i++){
        for(j = 0; j < columns; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **initArray(int size){

    int i;
    int **newArray = malloc(size*sizeof(int*));

    for(i = 0; i < size; i++)
        newArray[i] = malloc(size*sizeof(int));

    return newArray;
}

void encrypt(int** matrix, int keyGridSize){

    int** plainTextMatrix;
    int** resultMatrix;

    int len = strlen(myArray);
    int i, j, k = 0;
   //here we allocate space to create a matrix for our array of chars
    plainTextMatrix = malloc(sizeof(int*)*keyGridSize);

    for(i = 0; i < keyGridSize; i++)
        plainTextMatrix[i] = malloc(sizeof(int)*(len / keyGridSize));


    //Copy the all lowercase plaintext into the matrix
    for(j = 0; j < len / keyGridSize; j++)
    {
        for(i = 0; i < keyGridSize; i++)
        {
            //we subtract the ASCII value of 'a' to make our multiplication easier to debug
            plainTextMatrix[i][j] = myArray[k] - 'a';
            k++;
        }
    }
    //here we peerfrom the matrix multiplication and send the results to our resultMatrix
    resultMatrix = matrixMultiply(matrix, keyGridSize, plainTextMatrix, keyGridSize, (len / keyGridSize));

    //here we need to ensure that our resulting matrix will be in the alphabet of chars from 0-26
    for(j = 0; j < len / keyGridSize; j++)
    {
        for(i = 0; i < keyGridSize; i++)
        {
            resultMatrix[i][j] = resultMatrix[i][j] % 26;
        }
    }

    k = 0;

    printf("\n\nCiphertext:\n\n");
    //we need to add back the value of int 'a' from earlier to put the correct ASCII value in
    for(j = 0; j < len / keyGridSize; j++)
    {
        for(i = 0; i < keyGridSize; i++)
        {
            printf("%c", resultMatrix[i][j] + 'a');
            //this ensure we only print 80 chars a line
            if((k + 1) % 80 == 0)
                printf("\n");
            k++;
        }
    }
}

int** matrixMultiply(int** keyMatrix, int keyGridSize, int** plainTextMatrix, int r2, int c2){

    int** resultMatrix;
    int i, j, k, sum = 0;
    //here we allocate space for our resulting matrix after multiplication
    resultMatrix = malloc(sizeof(int*)*keyGridSize);

    for(i = 0; i < keyGridSize; i++)
        resultMatrix[i] = malloc(sizeof(int)*(c2));
    //here we perform the matrix multiplication
    for (i = 0; i < keyGridSize; i++)
    {
        for (j = 0; j < c2; j++)
        {
            for (k = 0; k < keyGridSize; k++)
                sum = sum + keyMatrix[i][k]*plainTextMatrix[k][j];

            resultMatrix[i][j] = sum;
            //we set sum to zero to ensure integrity for the next run through
            sum = 0;
        }
    }

    return resultMatrix;
}
