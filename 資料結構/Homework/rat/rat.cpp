#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

// Define the lengths of the array.
#define LENGTH 17

// Define the structure of the Stack.
typedef struct {
    int Po_X;
    int Po_Y;
} Stack;

// Declare the Stack array
Stack stack[LENGTH*LENGTH]; // Store the data which will be pushed into the stack.
Stack receive; // Store the data which will be poped from the stack.

// Declare the functions that will be used in the program.
void push(int x, int y); // Push the data into the stack.
Stack pop(); // Pop the data from the stack.
bool isFull(); // Check if the stack is full.
bool isEmpty(); // Check if the stack if empty.
void findRoad(int position_Y, int position_X, char maze[LENGTH][LENGTH]); // The function which will be used to find the road.
void Reset(); // The function which will reset some data when the user start to find the new road.

//Declare some variable which will be used in the program.
int count = 0; // The variable which will record the frequency of the step count.
int level = -1; // The variable which will record the top of the stack.
int path[LENGTH][LENGTH]; // The array which store the road that has been walked.
int start[2], end[2]; // The array which store the start position and end position.

int main(){
    char input_Name[100]; // The array which store the input of the file name.
    
    printf("enter filename: "); 
    scanf("%s", input_Name); 
        
    FILE *file = fopen(input_Name, "r"); 
    char input_Ch; // The variable which will store the characteristic from the file.
    char maze[LENGTH][LENGTH]; // The array which store the maze data.

    if(file == NULL){ // If the file cannot be found, the program will print "The file cannot be found." and stop the program. (Although it isn't required, I still put it in the program.)
        printf("The file cannot be found.");
        exit(EXIT_FAILURE);
    }
    else{
        // Get the characteristic from the file and put into the maze array.
        for(int i = 0; i <  LENGTH; i++){
            for(int j = 0; j < LENGTH + 1; j++){
                input_Ch = fgetc(file); // Get the characteristic from the file.
                if(input_Ch != '\n'){                    
                    maze[i][j] = input_Ch; // Because it will have new-line in the file (However, we can't see it.), it need to ignore the new-line in the file.
                }                
            }            
        }        
    }

    // Print the maze
    for(int i = 0; i < LENGTH; i++){
        for(int j = 0; j < LENGTH; j++){
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }

    // Start to find the road.
    while(true){ // Because of using while loop, the program will keep running until the start position is -1 -1.
        printf("\nenter start position: ");
        scanf("%d %d", &start[0], &start[1]); 
        if(start[0] != -1 && start[1] != -1){ // If the start position is -1 -1, the program will stop.
            printf("enter exit position: ");
            scanf("%d %d", &end[0], &end[1]);
            Reset(); // Reset the variable which will be used in the program.
            level++; // Add the start position to the stack.
            printf("%d:%d,%d\n", count, start[0], start[1]);
            count++; // Add one to the times that the rat run.
            findRoad(start[0], start[1], maze); // Start to find the road.
        }else{
            printf("end the code.");
            exit(EXIT_SUCCESS); // If the start position is -1 -1, the program will exit.
        }
    }
    return 0;
}

//The order that the rat will run is: right -> down -> up -> left.
void findRoad(int position_Y, int position_X, char maze[LENGTH][LENGTH]){
    for(count = 1; (position_X != end[1])||(position_Y != end[0]); count++){ // The for loop will keep to run until the end has been found or the position return to the start point and there are no path to go.
        if(position_X == start[1] && position_Y == start[0] && (path[position_Y][position_X+1] == 1||maze[position_Y][position_X+1] == '1') && (path[position_Y][position_X-1] == 1||maze[position_Y][position_X-1] == '1') && (path[position_Y+1][position_X] == 1 || maze[position_Y+1][position_X] == '1') && (path[position_Y-1][position_X] == 1 || maze[position_Y-1][position_X] == '1')){ // If there are no path to go, the loop will stop.
            printf("Failed to escape.\n");
            break;
        }else{
            if(position_X + 1 < LENGTH && maze[position_Y][position_X + 1] == '0' && path[position_Y][position_X + 1] != 1) { // Find the right road.
                printf("%d:%d,%d\n", count, position_Y, position_X + 1);
                path[position_Y][position_X] = 1; // It means the path has been walked, so it couldn't walk again.
                push(position_X, position_Y); // It will push the position into the stack.
                position_X++; // It will change the position to the right hand side.
            }
            else if(position_Y + 1 < LENGTH && maze[position_Y + 1][position_X] == '0'&& path[position_Y + 1][position_X] != 1){ // Find the road below the position.
                printf("%d:%d,%d\n", count, position_Y + 1, position_X); 
                path[position_Y][position_X] = 1; // It means the path has been walked, so it couldn't walk again.
                push(position_X, position_Y); // It will push the position into the stack.
                position_Y++; // It will change the position below the original position.                               
            }
            else if(position_Y - 1 >= 0 && maze[position_Y - 1][position_X] == '0'&& path[position_Y - 1][position_X] != 1){ // Find the road above the position.
                printf("%d:%d,%d\n", count, position_Y - 1, position_X); 
                path[position_Y][position_X] = 1; // It means the path has been walked, so it couldn't walk again.
                push(position_X, position_Y); // It will push the position into the stack.
                position_Y--; // It will change the position above the original position.
            }
            else if(position_X - 1 >= 0 && maze[position_Y][position_X - 1] == '0'&& path[position_Y][position_X - 1] != 1){ // Find the left road.
                printf("%d:%d,%d\n", count, position_Y, position_X - 1); 
                path[position_Y][position_X] = 1; // It means the path has been walked, so it couldn't walk again.
                push(position_X, position_Y); // It will push the position into the stack.
                position_X--; // It will change the position to the left hand side.
            }
            else{                
                path[position_Y][position_X] = 1; // It means the path has been walked, so it couldn't walk again.     
                receive = pop(); // Pop the data from the stack and store into receive.
                position_Y = receive.Po_Y; // Store the data which pop from the stack.
                position_X = receive.Po_X; // Store the data which pop from the stack.
                printf("%d:%d,%d\n", count, receive.Po_Y, receive.Po_X);    
            }       
        }         
    }
    if(position_X == end[1] && position_Y == end[0]){ // If the rat walk to the end successfully, it will print the message.
            printf("successfully escape!!\n");            
    } 
    
}

bool isFull(){ // Check if the stack is full.
    if(level == LENGTH * LENGTH - 1)
        return true;
    else 
        return false;
}

bool isEmpty(){ // Check if the stack if empty.
    if(level == 0)
        return true;
    else
        return false;
}

void push(int x, int y){ // Push the data into stack.
    if(!isFull()){
        stack[level].Po_X = x;
        stack[level].Po_Y = y;
    }
    level++;
}

Stack pop(){ // Pop the data from the stack.
    if(!isEmpty()){       
        level--;     
        return stack[level];
    }
    return stack[level--];
}

void Reset(){ // Reset the variable which will be used in the program when the new road begin to find.
    count = 0;
    level = 0;
    for(int i = 0; i < LENGTH; i++){
        for(int j = 0; j < LENGTH; j++){
            path[i][j] = 0;
        }
    }
}