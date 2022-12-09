#include <stdio.h>
#include <stdlib.h>


struct Queue{
    int value;
    struct Queue* Node;
}queue; // The queue which use to do BFS.

struct Queue *front, *rear; // The front pointer and rear pointer of the queue.

void BFS(int matrix[][100], int vertices, int path); // BFS
void enqueue(int value); // Enqueue to the queue.
struct Queue dequeue(); // Dequeue from the queue.


int answer[100][100]; // The array which store the answer.
int count = -1; // The numbers of the matrix.
int time = 0; // The numbers of adding numbers to the matrix.

int main(){
    char filename[100]; // Store the filename.
    printf("Please enter the input file name: ");
    scanf("%s", &filename);
    getchar(); // Get the newline.
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Enter failed."); // If the file doesn't find, the message will appear.
    } else {
        for(int i = 0; i < 100; i++){ // Initial the answer array.
            for(int j = 0; j < 100; j++){
                answer[i][j] = -1;
            }
        }
        int number = 0;
        fscanf(file, "%d", &number);// Get the times that the BFS will do.
        getc(file); // Get the newline from the file.
        for(int i = 0; i < number; i++){
            count++; // Increase the time.
            time = 0; // Initial the times of adding numbers to the matrix.
            int matrix[100][100]; // Store the graph.
            int vertices, start; 
            fscanf(file, "%d %d", &vertices, &start); // The number of vertices and start.
            getc(file); // Get the newline from the file.
            queue.value = start; // Initial the first position of queue.
            queue.Node = NULL;
            front = &queue; // Point to the front of the queue.
            rear = &queue; // Point to the rear of the queue.
            for(int j = 0; j < vertices; j++){
                for(int k = 0; k < vertices; k++){
                    fscanf(file, "%d", &matrix[j][k]); // Get the graph.             
                }
                getc(file); // Get the newline from the file.
            }
            answer[count][time] = start; // Store the first point to the answer.
            time++; // Increase the time.
            BFS(matrix, vertices, start); // Do BFS
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

void BFS(int matrix[][100], int vertices, int path){
    int pass = 0; // The variable which decide if the pop from the stack can stop.
    while(1){
        for(int i = 0; i < vertices; i++){
            if(matrix[path][i] == 1){                
                enqueue(i); // If the road is exist, enqueue into the queue.
            }
        }
        path =  dequeue().value; // Dequeue the road from the stack.
        int under = time-1; // The index of the final of the answer.
        while(!pass){
            if(answer[count][under] == path){ // If the number dequeue from the queue is already exist in the answer, dequeue the new number again.
                path = dequeue().value;                
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

void enqueue(int value){ // Enqueue the number into the queue.
    struct Queue* tmp = (struct Queue*)malloc(sizeof(struct Queue));
    tmp->value = value;
    tmp->Node = NULL;
    rear->Node = tmp;
    rear = rear->Node;
}

struct Queue dequeue(){ // Dequeue the number from the queue.
    struct Queue* tmp = (struct Queue*)malloc(sizeof(struct Queue));
    tmp = front;
    front = front->Node;
    tmp->Node = NULL;
    return *tmp;
}