#include <stdio.h>
#include <stdlib.h>

struct Stack{
    int number;
    struct Stack* Node;
}stack; // The stack which use to do DFS.

struct Stack* top; // The top pointer of the stack.

void DFS(int matrix[][100], int vertices, int path); // DFS
void push(int value); // Push to the stack.
struct Stack pop(); // Pop from the stack.


int answer[100][100]; // The array which store the answer.
int count = -1; // The numbers of the matrix.
int time = 0; // The numbers of adding numbers to the matrix.
int number = 0; // The times that the DFS will do.

 int main(){     
     char filename[100]; // Store the filename.
     printf("Please enter the input file name: ");
     scanf("%s", &filename); 
     getchar(); // Get the newline.
     FILE *file = fopen(filename, "r");
     if(file == NULL){
         printf("Enter failed."); // If the file doesn't find, the message will appear.
     } else {
     for(int i = 0; i < 100; i++)
         for(int j = 0; j < 100; j++)
            answer[i][j] = -1; // Initial the answer array.
     fscanf(file, "%d", &number); // Get the times that the DFS will do.
     getc(file); // Get the newline from the file.
     for(int i = 0; i < number; i++){
         count++; // Increase the time.
         time = 0; // Initial the times of adding numbers to the matrix.
         int vertices, index;
         fscanf(file, "%d %d", &vertices, &index); // The number of vertices and start.
         getc(file); // Get the newline from the file.
         stack.number = index; // Initial the first position of stack.
         stack.Node = NULL;
         top = &stack; // Point the top of the stack.
         int matrix[100][100]; // Store the graph.
         for(int j = 0; j < vertices; j++){
             for(int k = 0; k < vertices; k++){
                 fscanf(file, "%d", &matrix[j][k]); // Get the graph.
             }
             getc(file); // Get the newline from the file.
         }
         answer[count][time] = index; // Store the first point to the answer.
         time++; // Increase the time.
         DFS(matrix, vertices, index); // Do DFS         
        }
        for(int i = 99; i >= 0; i--)
            filename[i + 1] = filename[i]; // Move the characteristics in the filename to make the filename from input to output.
         filename[0] = 'o';
         filename[1] = 'u';
         filename[2] = 't';
         FILE *output = fopen(filename, "w+"); // Create the file.
         for(int j = 0; j <= count; j++){
             for(int k = 0; k < time; k++){
                 if(answer[j][k] != -1)
                    fprintf(output, "%d ", answer[j][k]); // Put the result to the file.
             }
             fprintf(output, "\n");             
         }
     }
 }

 void DFS(int matrix[][100], int vertices, int path){     
     int pass = 0; // The variable which decide if the pop from the stack can stop.
     while(1){         
        for(int i = vertices - 1; i >= 0; i--){
            if(matrix[path][i] == 1){
                push(i); // If the road is exist, push into the stack.
            }
        }
        path = pop().number; // Pop the road from the stack.
        int under = time - 1; // The index of the final of the answer.
        while(!pass){                        
            if(answer[count][under] == path){ // If the number pop from the stack is already exist in the answer, pop the new number again.
                path = pop().number;                
                under = time - 1;
                continue;
            }
            under--;
            if(under == -1) // If all the number is checked, leave the loop.
                pass = 1;
        }
        pass = 0; // Initial the pass.
        answer[count][time] = path; // Store the path into answer.     
        time++;
        if(time == vertices) // If all of the number have been checked, leave the function.
            break;
     }
 }

 void push(int value){ // Push the number into the stack.
     struct Stack *temp = (struct Stack*)malloc(sizeof(struct Stack));
     temp->number = value;
     temp->Node = top;
     top = temp;
 }

 struct Stack pop(){ // Pop the number from the stack.
     struct Stack *tmp = (struct Stack*)malloc(sizeof(struct Stack));
     tmp = top;
     top = top->Node;
     tmp->Node = NULL;
     return *tmp;
 }

