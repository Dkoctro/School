#include <stdio.h>

#define MAX 1000000 // The max range of the input.

struct Lines{
    int firstVertex;
    int secondVertex;
    int distance;
}; // The struct store the inputs.

struct Lines lines[2 * MAX + 1]; // The struct array store the inputs.

int findParent(int i, int parent[]); // Find the parent of the number.
int uni(int i, int j, int parent[]); // Compare if the parent is the same.

int main(){
    char filename[100]; // Store the filename.
    printf("Please enter the input file name: ");
    scanf("%s", &filename);
    getchar(); // Get the newline.
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Enter failed"); // If the file doesn't find, the message will appear.
    } else {
        int vertices = 0, edge = 0; 
        int min = MAX + 1, minIndex = 0; // The variable which store the min value and the index.
        int stop = 0; // The variable which control the loop.
        int wholedistance = 0; // The variable store the whole distance.
        int u, v; // Store the parent of the path.
        fscanf(file, "%d %d", &vertices, &edge);
        getc(file); // Get the newline from the file.
        int parent[edge]; // Store the parent of each vertex.
        for(int i = 0; i < edge; i++)
            parent[i] = 0; // Initial all the parent.
        for(int i = 1; i <= edge; i++){
            fscanf(file, "%d %d %d", &lines[i].firstVertex, &lines[i].secondVertex, &lines[i].distance);
            getc(file);
        }    
        while(stop < edge){
            for(int i = 1; i <= edge; i++){  // Get the min distance and two vertex.          
                if(lines[i].distance < min){                
                    min = lines[i].distance;
                    u = lines[i].firstVertex;
                    v = lines[i].secondVertex;       
                    minIndex= i; // Store the min index.
                }
            }                
            u = findParent(u, parent);        
            v = findParent(v, parent);
            int tmp = 0;
            if(v > u){ // Because the uni function need u always greater than v, if v is greater than u, change two position.
                tmp = v;
                v = u;
                u = tmp;
            }         
            if(uni(u, v, parent)){                         
                wholedistance+=min; // If the uni function return True, it means the path doesn't become cycle.
            }
            stop++; // Increase the stop.
            min = MAX + 1; // Initial the min
            lines[minIndex].distance = min; // Set the min lines to the number which will never greater than min, so it won't be checked again.      
        }
        for(int i = 99; i >= 0; i--)
            filename[i + 1] = filename[i]; // Move the characteristics in the filename to make the filename from input to output.
        filename[0] = 'o';
        filename[1] = 'u';
        filename[2] = 't';
        FILE *output = fopen(filename, "w+"); // Create the file.
        fprintf(output, "%d", wholedistance); // Put the result to the file.
    }
    return 0;
}

int findParent(int i, int parent[]){
    while(parent[i]) // If the number isn't the parent, the number will keep finding.
        i = parent[i];
    return i;
}

int uni(int i, int j, int parent[]){
    if(i != j){ // If the cycle doesn't appear, set the parent.
        parent[j] = i;
        return 1;
    }
    return 0;
}