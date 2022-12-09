#include <stdio.h>
#include <string.h>

int searchRoot(int in[], int data, int number, int mode); // Search the root of the tree.
void findPostorder(int in[], int pre[], int number); // Find the postorder.
void findPreorder(int in[], int post[], int number, int index); // Find the preorder.

int modeSelect = 0; // Store the mode which is either findPostorder or findPreorder.
int count = 0; // The index of answer array which store the answer of result.
int countline = -1; // The index of answer array which store the times of answer.
int answer[1000][1000]; // The array of answer.

int main(){
    int line = 0; // The times of finding.
    int nodeNumber = 0; // The max number of nodes.
    int nodeNB = 0; // The number of nodes.    
    char mode[18]; // Store the string of preorder-inorder or postinorder-inorder.

    char filename[100]; // The file name.
    printf("Please enter the input file name: ");
    scanf("%s", &filename); // Get the name of file.
    getchar(); // Get the newline.
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Enter failed"); // If the file doesn't open successfully, the message will appear.
    } else {
        fscanf(file, "%d", &line); // Get the times of finding.
        getc(file); // Get the newline in the file.
        for(int i = 0; i < line; i++){
            countline++; // Plus one to the countline.
            fgets(mode, 18, file); // Get the mode from the file.
            fscanf(file, "%d", &nodeNB); // Get the number of nodes.
            getc(file); // Get the newline from the file.
            if(nodeNB > nodeNumber)
                nodeNumber = nodeNB; // If the number of nodes is more than the max, the max will be replaced.
            int nodes[2][nodeNB]; // store the nodes from the file.
            int *nodePtr = nodes[0]; // Point to the first line of nodes.
            int *nodePtr2 = nodes[1]; // Point to the second line of nodes.
        
            for(int j = 0; j < 2; j++){
                for(int k = 0; k < nodeNB; k++){
                    fscanf(file, "%d", &nodes[j][k]); // Get the nodes from the file.                    
                }                   
                getc(file); // Get the newline from the file.
            }
            left = 0;
            if(!strncmp(mode, "preorder-inorder", 16)){                
                count = 0;
                modeSelect = 1;                
                findPostorder(nodePtr2, nodePtr, nodeNB); // Find the postorder
            } else if(!strncmp(mode, "postorder-inorder", 17)){                   
                count = nodeNB; // Because the result will be reversed, the index should be counted down.
                modeSelect = 2;                
                findPreorder(nodePtr2, nodePtr, nodeNB, nodeNB - 1); // Find the preorder
            }
        }
        for(int i = 99; i >= 0; i--)
            filename[i + 1] = filename[i]; // Move the characteristics in the filename to make the filename from input to output.
        filename[0] = 'o';
        filename[1] = 'u';
        filename[2] = 't';
        FILE *output = fopen(filename, "w+"); // Create the output file.
        for(int i = 0;  i < line; i++){
            for(int j = 0; j < nodeNumber; j++){
                if(answer[i][j] != 0){                
                    fprintf(output, "%d ", answer[i][j]); // Put the result to the file.
                }
                else
                    break;
            }
        fprintf(output, "\n");
        }
    }
    return 0;
}

int searchRoot(int in[], int data, int number, int mode){
    int root = 0;
    for(int i = 0; i < number; i++){ // Find the index of the preorder or postorder in the inorder because the numbers in the preorder or postorder (reversed) will always be the root.        
        if(data == in[i]){            
            root = i;            
            return root; // Return the index of the root.
         }
     }
    if(mode == 1) // If there isn't any root in inorder, the function will return error.
        return -1;
    else
        return root;
}

void findPostorder(int in[], int pre[], int number){
    int root = searchRoot(in, pre[0], number, modeSelect); // Find the root.
    if(root != 0)
        findPostorder(in, pre+1, root); // It means there exists the left child.
    if(root != number - 1)
        findPostorder(in + root + 1, pre + root + 1, number - root - 1); // It means there exists the right child.

    answer[countline][count] = pre[0]; // Add the answer into the array.
    count++; // Increase the index of the answer array.
}

void findPreorder(int in[], int post[], int number, int index){    
    int root[number]; // Store the root of the postorder.
    int tmp[number]; // Store the temp roots.
    int temp = index; // Store the index of postorder.
    int tmpindex = 0; // Store the number of index.
    int bottom = number; // Store the bottom of the roots.
    for(int i = 0; i < number; i++){        
        root[i] = searchRoot(in, post[temp], number, modeSelect); // Find the root.        
        temp--; // Decrease the index.
    }         
    for(int i = 0; i < number; i++){
        for(int j = i + 1, n = j; n < number;n++){
            if(root[j] > root[i]){ // If the back root is greater than the root, it will be moved back.
                tmp[tmpindex] = root[j]; // Store the roots.
                for(int k = j; k < number; k++){ // Move the root for one step.
                    root[k] = root[k+1];
                    root[k+1] = -1;
                }
                tmpindex++;
            } else
                j++; // If the back root isn't greater than the root, j increase to find the next root.
        }                
        bottom = number - tmpindex; // Bottom will be the position of moving roots.
        tmpindex = 0; // Initial the tmpindex;
        for(int j = bottom; j < number; j++){
            root[j] = tmp[tmpindex]; // Move the greater roots back to the array.
            tmpindex++;
        }                
        tmpindex = 0; // Initial the tmpindex;
    }
    for(int i = 0; i < number; i++)
        answer[countline][i] = in[root[i]]; // Add the answer into the array.
}