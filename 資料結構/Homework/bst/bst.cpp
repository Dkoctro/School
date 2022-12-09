#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Mode1{
    int input_Number;
    Mode1 *left_Node;
    Mode1 *right_Node;  
}; // Use linked-list to store the BST. 

struct Queue{
    Mode1 mode1;
    Queue *Node;
}; // Use Queue to store the level order.

struct Stack{    
    Mode1 mode1;
    Stack *Node;
}; // Use Stack to store the path.

void insertBST(int input); // Insert number to BST.
void deleteBST(int input); // Delete number from BST.
void searchBST(int input); // Search number from BST.
void printBST(void); // Print number in BST.
void prefixOrder(Mode1 *temp_ptr); // Print the prefix order.
void infixOrder(Mode1 *temp_ptr); // Print the infix order.
void postfixOrder(Mode1 *temp_ptr); // Print the postfix order.
void levelOrder(Mode1 *temp_ptr); // Print the level order.
void enqueue(Mode1 temp); // Enqueue the number to Queue.
void searchRoad(int sword, int meaty); // Find the road of the meaty.
void push(Mode1 temp); // Push the number to Stack.
void printRoad(int path, int sword, int meaty); // Print the path that capoo walk.
void reset(); // Reset the variables.
Mode1 dequeue(); // Dequeue the number from Queue.
Mode1 pop(); // Pop the number from Stack.

Queue *front = NULL; // Front pointer of Queue.
Queue *rear = NULL; // Rear pointer of Queue.

Stack *top = NULL; // Top pointer of Stack.
Stack *bottom = NULL;
Mode1 *first_ptr = NULL; // The pointer which always point to the root of BST.

bool first = false; // Store whether the input to BST is first time.
bool haveSword = false; // Check if the sword is found.
bool findMeaty = false; // Check if meaty is found.
int mode = 3; // Choosing mode.

int road[2000];
int *road_ptr = road;

int main(){    
    while(1){
        reset(); // Before next action, reset the variables first.
        printf("(1)Binary searching Tree.\n"
               "(2)Finding Meaty.\n"
               "(0)Escape and face to music next year.\n"); // Print the main menu    
        scanf("%d", &mode); // Read the mode.
        getchar(); // Absorb the new-line after enter the number into mode.
        if(mode == 0){
            exit(0);
        } else if(mode == 1){
            char action; // Store the action.
            int input[20000]; // Store the input of numbers.
            int *ptr; // The pointer point to the empty place of array.
            do{
                printf("\n(I)nsert a number.\n"    
                       "(D)elete a number.\n"
                       "(S)earch a number.\n"
                       "(P)rint 4 kinds of orders.\n"
                       "(R)eturn\n"); // Action menu.
                scanf("%c", &action); // Read the action.
                getchar(); // Absorb the new-line after enter the character into action.        
                ptr = input; // Set the pointer to the first place of array.
                switch(action){
                    case 'i':
                    case 'I':                             
                        printf("Enter numbers: ");
                        while(1){
                            scanf("%d", ptr); // Read the number one by one.
                            if(*ptr == -1){                                
                                getchar(); // Absorb the new-line from input.
                                break;
                            } else {
                                insertBST(*ptr);                        
                                ptr++; // Point to next number.
                            }
                        }                
                        ptr = input; // Point to first place of array.
                        for(int i = 0; input[i] != '\0' || i < 2000; i++){
                            input[i] = 0;
                        } // Reset the array of input.
                        break;
                    case 'd':
                    case 'D':                             
                        printf("Enter numbers: ");
                        while(1){
                            scanf("%d", ptr); // Read the number one by one.
                            if(*ptr == -1){                                
                                getchar(); // Absorb the new-line from input.
                                break;
                            } else {                        
                                deleteBST(*ptr);
                                ptr++; // Point to next number.
                            }
                        }                    
                        ptr = input; // Point to first place of array.
                        for(int i = 0; input[i] != '\0' || i < 2000; i++){
                            input[i] = 0;
                        } // Reset the array of input.
                        break;
                    case 's':
                    case 'S':                               
                        printf("Enter numbers: ");
                        while(1){
                            scanf("%d", ptr); // Read the number one by one.              
                            if(*ptr == -1){                                
                                getchar(); // Absorb the new-line from input.
                                break;
                            } else {
                                searchBST(*ptr);
                                getchar();
                                ptr++; // Point to next number.
                            }
                        }                     
                        ptr = input; // Point to first place of array.
                        for(int i = 0; input[i] != '\0' || i < 2000; i++){
                            input[i] = 0;
                        } // Reset the array of input.
                        break;
                    case 'p':
                    case 'P':
                        printBST();                        
                        break;
                    case 'r':
                    case 'R':
                        reset();
                        break;                        
                }
                if(action == 'r' || action == 'R')
                    break; // Break  the while loop and return.
            } while(action != 'r' || action != 'R');
        } else if(mode == 2){
            char name[100]; // Store the string of file.
            printf("Please input the map file: ");
            scanf("%s", name);
            FILE *file = fopen(name, "r");
            if(file == NULL)
                printf("Load file failed.\n\n");
            else{
                printf("Load file success.\n\n");
                int number;
                fscanf(file, "%d", &number); // Read the number from file.
                while(!feof(file)){
                    insertBST(number);
                    fscanf(file, "%d", &number);
                }
                insertBST(number); // Because after read the last number, the file will be EOF, but the last number doesn't insert to BST.
                fclose(file); // Close the file.
                int sword_location = 0, meaty = 0, broccoli = 0;
                printf("Please input the sword location: ");
                scanf("%d", &sword_location); // Read the sword position.
                getchar(); // Absorb the new-line from input the sword location.
                printf("Please input the Meaty's location: ");
                scanf("%d", &meaty); // Read the meaty position.
                getchar(); // Absorb the new-line from input the meaty's location.
                printf("Please input the broccoli traps' index (0~9): ");
                scanf("%d", &broccoli); // Read the broccoli position.
                getchar(); // Absorb the new-line from input the broccoli traps' index.    
                searchBST(broccoli); // Find  and delete the broccolis.
                searchRoad(sword_location, meaty); // Find the path.
            }

        }
    }
    return 0;
}

void insertBST(int input){
    Mode1 *temp_ptr = NULL, *input_ptr = NULL; // Two pointer point to the number.
    Mode1 *ptr = (Mode1 *)malloc(sizeof(Mode1)); // New memory space of input.
    
    ptr->input_Number = input; // Set the number equals to input number.
    ptr->left_Node = NULL; // Set the pointer point to NULL
    ptr->right_Node = NULL; // Set the pointer point to NULL

    if(!first){ // Judge if this is the first input of BST.
        if(mode == 1) // If the mode is one, the string will be print.
            printf("Number %d is inserted\n", input);
        first = true;
        first_ptr = ptr; // Set first pointer.
    } else {
        temp_ptr = first_ptr; // Set temp pointer point equals to first pointer.
        input_ptr = first_ptr; // Set input pointer point equals to first pointer.
        while(1){
            if(input < input_ptr->input_Number){ // If input is less than the number which is pointed by input pointer.
                input_ptr = input_ptr->left_Node; // Move the pointer to left node.
                if(input_ptr == NULL){
                    temp_ptr->left_Node = ptr; // If left node is empty, the input will be pointed by it. Which means the value insert into BST.
                    if(mode == 1)
                        printf("Number %d is inserted\n", input); // Print the message.
                    break;
                } else {
                    temp_ptr = input_ptr; // If left node isn't empty, the temp pointer will point equal to input pointer.
                }
            } else if(input > input_ptr->input_Number){ // If input is larger than the number which is pointed by input pointer.
                input_ptr = input_ptr->right_Node; // Move the pointer to right node.
                if(input_ptr == NULL){
                    temp_ptr->right_Node = ptr; // If right node is empty, the input will be pointed by it. Which measn the value insert into BST.
                    if(mode == 1)
                        printf("Number %d is inserted\n", input); // Pirnt the message.
                    break;
                } else {
                   temp_ptr = input_ptr; // If right node isn't empty, the temp pointer will point equal to input pointer.
                }
            } else {
                if(mode == 1)
                    printf("Error. Number %d exists.\n", input); // If the value already exist, the error message will print.
                break;
            }
        }                    
    }
}

void deleteBST(int input){
    Mode1 *temp_ptr = NULL, *input_ptr = NULL, *behind_ptr = NULL; // Three pointer point to the value.
    temp_ptr = first_ptr; // Set temp pointer point equals to first pointer.
    input_ptr = first_ptr; // Set input pointer point equals to first pointer.
    while(1){        
        if(input < input_ptr->input_Number){ // If input is less than the number which is pointed by input pointer.
            input_ptr = input_ptr->left_Node; // Move pointer to left node.
            if(input_ptr == NULL){
                if(mode == 1)
                    printf("Number %d is not exist.\n", input); // If the input pointer point to null, it means the number isn't in BST, so it can't be found, and print the message.
                break;
            } else {
                behind_ptr = temp_ptr; // Let behind pointer always behind input pointer.
                temp_ptr = input_ptr; // Let temp pointer point equal to input pointer.
            }
        } else if(input > input_ptr->input_Number){ // If input is larger than the number which is pointed by input pointer.
            input_ptr = input_ptr->right_Node; // Move pointer to right node.
            if(input_ptr == NULL){
                if(mode == 1)
                    printf("Number %d is not exist.\n", input); // If the input pointer point to null, it means the number isn't in BST, so it can't be found, and print the message.
                break;
            } else {
                behind_ptr = temp_ptr; // Let behind pointer always behind input pointer.
                temp_ptr = input_ptr; // Let temp pointer point equal to input pointer.
            }
        } else { // Find the value, and have different action according to the amount of children.
            if(input_ptr->left_Node == NULL && input_ptr->right_Node == NULL){ // If the amount is zero, the node will be delete without other actions.
                printf("Number %d is deleted.\n", input);
                if(behind_ptr->left_Node == input_ptr) // Set the previous node point to null.
                    behind_ptr->left_Node = NULL; 
                else   
                    behind_ptr->right_Node = NULL;
                free(input_ptr); // Free the memory space of deleted node.
                break;
            } else if(input_ptr->left_Node != NULL && input_ptr->right_Node != NULL){ // If the amount is two, the node will be delete with a lot of actions.
                behind_ptr = input_ptr; // The pointer behind the input pointer.
                input_ptr = input_ptr->right_Node; // Input pointer point to the right children.
                while(input_ptr->left_Node != NULL){ // If the left children exist, input pointer will point to the left children, and behind point will always behind it.
                    behind_ptr = input_ptr;
                    input_ptr = input_ptr->left_Node;
                }
                temp_ptr->input_Number = input_ptr->input_Number; // Let theh value in temp pointer equals to the value in input pointer.
                if(temp_ptr == behind_ptr){
                    behind_ptr->right_Node = input_ptr->right_Node;
                }
                else if(input_ptr->right_Node != NULL){
                    behind_ptr->left_Node = input_ptr->right_Node; // If there exist right children in the input pointer, the left node of behind pointer will point to the right children of input pointer.
                }                
                printf("Number %d is deleted.\n", input);       
                break;
            } else if(input_ptr->left_Node != NULL){ // If the children only one, it will need less action.
                input_ptr = input_ptr->left_Node; // Move input pointer to the node which is the left node of the deleted node.
                temp_ptr->input_Number = input_ptr->input_Number; // Move the value from the node which pointed by input pointer to the temp pointer.
                temp_ptr->left_Node = input_ptr->left_Node; // Move the left node from the node which pointed by input pointer to the temp pointer.
                temp_ptr->right_Node = input_ptr->right_Node; // Move the right node from the node which pointed by input pointer to the temp pointer.
                printf("Number %d is deleted.\n", input);
                free(input_ptr); // Free the memeory space of deleted node.
                break;
            } else if(input_ptr->right_Node != NULL){
                behind_ptr =input_ptr; // The pointer behind the input pointer.
                input_ptr = input_ptr->right_Node; // Move input pointer to the node which is the right node of the deleted node.
                while(input_ptr->left_Node != NULL){
                    behind_ptr = input_ptr; // The behind pointer will always point behind the input pointer.
                    input_ptr = input_ptr->left_Node;
                }
                temp_ptr->input_Number = input_ptr->input_Number; // Move the value from the node which pointed by input pointer to the temp pointer.
                temp_ptr->left_Node = input_ptr->left_Node;  // Move the left node from the node which pointed by input pointer to the temp pointer.
                temp_ptr->right_Node = input_ptr->right_Node; // Move the right node from the node which pointed by input pointer to the temp pointer.        
                printf("Number %d is deleted.\n", input);
                free(input_ptr); // Free the memeory space of deleted node.
                break;
            }
        }  
    }
}

void searchBST(int input){
    Mode1 *temp_ptr = NULL, *input_ptr = NULL;
    temp_ptr = first_ptr; // Set temp pointer point equals to first pointer.
    input_ptr = first_ptr; // Set input pointer point equals to first pointer.
    if(mode == 1){
        while(1){
            if(input < input_ptr->input_Number){ // If input is less than the number which is pointed by input pointer.
                input_ptr = input_ptr->left_Node; // Move pointer to left node.
                if(input_ptr == NULL){ // If the input pointer point to null, it means the number isn't in BST, so it can't be found, and print the message.
                    printf("SORRY. %d is not found.\n", input);
                    break;
                }
            } else if(input > input_ptr->input_Number){ // If input is less than the number which is pointed by input pointer.
                input_ptr = input_ptr->right_Node; // Move pointer to right node.
                if(input_ptr == NULL){ // If the input pointer point to null, it means the number isn't in BST, so it can't be found, and print the message.
                    printf("SORRY. %d is not found.\n", input);
                    break;
                }
            } else { // Find the number.
                printf("Bingo! %d is found.\n", input);
                break;
            }                    
        }
    } else if(mode == 2){ // If the mode is two , the search will change.
        int max = 0;
        while(1){ // Find the max value of the BST.
            if(temp_ptr != NULL){
                input_ptr = temp_ptr;
                temp_ptr = temp_ptr->right_Node; // Because the max always be the most right position, so keep find the right node.
            } else{
                max = input_ptr->input_Number;
                input_ptr = first_ptr;         
                break;
            }
        }
        for(int i = 1; i <= max; i++){ // Use loop to check if the value is in the BST and need to delete or not.
            while(1){
                if(i < input_ptr->input_Number){ // If input is less than the number which is pointed by input pointer.
                    input_ptr = input_ptr->left_Node; // Move pointer to left node.
                    if(input_ptr == NULL){                
                        input_ptr = first_ptr; // Reset the input pointer to the first pointer.     
                        break;
                    }
                } else if(i > input_ptr->input_Number){ //If input is larger than number which is pointed by input pointer.
                    input_ptr = input_ptr->right_Node; // Move pointer to right node.
                    if(input_ptr == NULL){    
                        input_ptr = first_ptr; // Reset the input pointer to the first pointer.          
                        break;
                    }
                } else {                
                    // Calculate the digit to check if the number is needed to delete.
                    input_ptr = first_ptr;
                    int number = i, number_ten, judge;
                    while(1){                        
                        if(number < 10 && number == input){ // If the number is equal to input, delete the number.               
                                deleteBST(i);
                                break;
                            } else if(number<10){ // If the number isn't equal to input and less than 10, it will never equal to input.
                                break;
                        }
                        number_ten = number / 10 * 10; // The number which use to do abstraction.
                        judge = number - number_ten; // The number which use to check if equal to input.
                        if(judge == input){                            
                            deleteBST(i);
                            break;
                        } else {
                            number /= 10; // If the number is larger than 10, the number will do division.
                        }
                    }
                    break;
                }
            }
        }
    }

}

void printBST(){
    printf("The tree in prefix order : "); // Print prefix order.
    prefixOrder(first_ptr);    
    printf("\nThe tree in infix order : "); // Print infix order.
    infixOrder(first_ptr);
    printf("\nThe tree in post order : "); // Print postfix order.
    postfixOrder(first_ptr);
    printf("\nThe tree in level order : "); // Print level order.
    levelOrder(first_ptr);
    printf("\n");
}

void prefixOrder(Mode1 *temp_ptr){
      if(temp_ptr != NULL){ // Use recursive to print the number from root to left node to right node.
          printf("%d ", temp_ptr->input_Number);
          prefixOrder(temp_ptr->left_Node);
          prefixOrder(temp_ptr->right_Node);
      }
}

void infixOrder(Mode1 *temp_ptr){
    if(temp_ptr != NULL){ // Use recursive to print the value from left node to root to right node.
        infixOrder(temp_ptr->left_Node);
        printf("%d ", temp_ptr->input_Number);
        infixOrder(temp_ptr->right_Node);
    }
}

void postfixOrder(Mode1 *temp_ptr){
    if(temp_ptr != NULL){ // Use recursive to print the number from left node to right node to root.
        postfixOrder(temp_ptr->left_Node);
        postfixOrder(temp_ptr->right_Node);
        printf("%d ", temp_ptr->input_Number);
    }
}

void levelOrder(Mode1 *temp_ptr){    
    Queue queue;
    front = &queue; // Set the front pointer.
    rear = &queue; // Set the rear pointer.
    Mode1 print; // The value use to print the numbers.
    enqueue(*first_ptr);
    while(1){        
        print = dequeue(); // Dequeue the Queue.
        printf("%d ", print.input_Number); // Print the number.
        if(print.left_Node != NULL)
            enqueue(*(print.left_Node)); // Enqueue the left node first.
        if(print.right_Node != NULL)
            enqueue(*(print.right_Node));        
        if(print.right_Node == NULL && print.left_Node == NULL && front == rear) // It means all the numbers have been printed.
            break;
    }        
}

void enqueue(Mode1 temp){ //Enqueue the value to Queue.
    Queue *ptr = (Queue *)malloc(sizeof(Queue));    
    ptr->mode1 = temp;    
    ptr->Node = NULL;
    rear->Node = ptr;
    rear = rear->Node;    
}

Mode1 dequeue(){ // Dequeue the value to Queue.
    front = front->Node;
    Mode1 temp_ptr = front->mode1;    
    return temp_ptr;
}

void searchRoad(int sword, int meaty){ // Find the shorest road.
    Stack stack;
    Mode1 *front;
    Mode1 *road;
    bool go = true; // Determine if the left node can go or not.
    road = first_ptr;
    top = &stack;    
    top->mode1 = *first_ptr; // Push root to Stack.
    top->Node = NULL;    
    while(1){
        while(1){
        if(road->left_Node != NULL && go){  // Push left node.
            push(*(road->left_Node)); // Push the node into Stack.
            front = NULL;
            road = road->left_Node;
            if(road->input_Number == sword && !haveSword){ // If the sword is found.
                haveSword = true;
                while(1){
                    *road = pop();
                    printRoad(road->input_Number, sword, meaty); // Print the path.
                    if(road->input_Number == first_ptr->input_Number) // If the Stack is empty, the loop will break.
                        break;
                }                     
            } else if(haveSword == true && road->input_Number == meaty){ // IF the meaty is found.
                findMeaty = true;
                while(1){
                    *road = pop();
                    printRoad(road->input_Number, sword, meaty); // Print the path.
                    if(road->input_Number == first_ptr->input_Number) // If the Stack is empty, the loop will break.
                        break;
                }
            } else {
                if(road->left_Node == NULL && road->right_Node == NULL){ // If the pointer is at leaf, the Stack will pop.
                    *road = pop();                
                }
            }
        } else 
            break;
        }
        if(road->right_Node != NULL){  // Push the right node.
            go = true; // It means it can push left node again.
            push(*(road->right_Node));
            front = NULL;
            road = road->right_Node;
            if(road->input_Number == sword && !haveSword){ // If the sword is found.
                haveSword = true;                
                while(1){
                    *road = pop();
                    printRoad(road->input_Number, sword, meaty); // Print the path.
                    if(road->input_Number == first_ptr->input_Number) // If the Stack is empty, the loop wil break.
                        break;
                }                     
            } else if(haveSword == true && road->input_Number == meaty){ // IF the meaty is found.
                findMeaty = true;
                while(1){
                    *road = pop();
                    printRoad(road->input_Number, sword, meaty); // Print the path.
                    if(road->input_Number == first_ptr->input_Number) // If the Stack is emtpy, the loop will break.
                        break;
                }
            } else {
                if(road->left_Node == NULL && road->right_Node == NULL){ // If the pointer is pointing to leaf, the Stack will pop.
                    *road = pop();
                    front = road;
                    while(1){
                        *road = pop();
                        if(front == road->left_Node){ // Detect if the front pointer is pointing in front of input pointer.
                            go = false;
                            break;
                        }
                        else{
                            front = road; // Set the front pointer point equal to road pointer.
                        }
                    }
                }
            }
        } else {
            *road = pop(); // If many node are been found, the Stack will keep poping.
        }
        
    }
}

void push(Mode1 temp){ // Push the value to Stack.
    Stack *ptr = (Stack *)malloc(sizeof(Stack));
    ptr->mode1 = temp;
    ptr->Node = top;
    top = ptr;
}

Mode1 pop(){ // Pop the value from Stack.
    Mode1 temp = top->mode1;
    if(top->mode1.input_Number != first_ptr->input_Number)
        top = top->Node;    
    return temp;
}

void printRoad(int path, int sword, int meaty){    
    *road_ptr = path;
    road_ptr++;
    if(*road_ptr == first_ptr->input_Number && haveSword){ // If the sword is found, it will print the path.
        int *head = road, *head2 = road_ptr;
        while(1){ // Print the path from root to sword.
            printf("%d->", *road_ptr);
            road_ptr--;
            if(road_ptr == head)
                break;
        }
        while(1){ // Print the path frmo sword to root.
            printf("%d->", *road_ptr);
            road_ptr++;
            if(road_ptr == head2)
                break;
        }
        haveSword = false;
    } else if(*road_ptr == first_ptr->input_Number && findMeaty){ // If the meaty is found, it wil print the path.
        int *head = road;
        while(1){ // Print the path from root to meaty.
            printf("%d", *road_ptr);
            road_ptr--;
            if(head == road_ptr){
                printf("%d", *road_ptr);
                break;
            } else{
                printf("->");
            }
        }
    }
}

void reset(){ // Reset the variables.
    free(front);
    free(first_ptr);
    front = NULL;
    rear = NULL;    
    first_ptr = NULL;
    first = false;
}