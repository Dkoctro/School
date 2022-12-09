#include <stdio.h>

#define MAX 1000000 // The max range of the input.

struct Lines{
    int firstvertex;
    int secondvertex;
    int distance;    
}; // The struct store the input.

struct Vertex{
    int path;
    struct Vertex *Node;
}; // The struct store each vertex.

struct Lines lines[MAX + 1]; // The struct array store the inputs.

int main(){
    char filename[100]; // Store the filename.
    printf("Please enter the input file name: ");
    scanf("%s", &filename);
    getchar(); // Get the newline.
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Enter failed."); // If the file doesn't find, the message will appear.
    } else {
        int vertexNB = 0, root = 0;
        fscanf(file, "%d", &vertexNB);
        getc(file); // Get the newline from the file.
        for(int i = 0; i < vertexNB - 1; i++){
            fscanf(file, "%d %d %d", &lines[i].firstvertex, &lines[i].secondvertex, &lines[i].distance);
            getc(file); // Get the newline from the file.
        }
        fscanf(file, "%d", &root);
        getc(file); // Get the newline from the file.
        int wholedistance[vertexNB + 1]; // Store the whole distance of each vertex.
        struct Vertex v[vertexNB+1]; // Each vertex.
        for(int i = 0; i <= vertexNB;i++){ // Initial the vertexs.
                wholedistance[i] = 0;
                v[i].path = 0;
                v[i].Node = NULL;
        }
        for(int i = 0; i < vertexNB; i++){ // If the second vertex is the root, the first vertex will point to the root.
            if(lines[i].secondvertex != root){ // If the second vertex isn't the root, the second vertex will point to the first vertex.
                v[lines[i].secondvertex].Node = &(v[lines[i].firstvertex]);
                v[lines[i].secondvertex].path = lines[i].distance; // The distance will store in second vertex.
            } else {
                v[lines[i].firstvertex].Node = &(v[lines[i].secondvertex]);
                v[lines[i].firstvertex].path = lines[i].distance; // The distance will store in first vertex.
            }
        }
        for(int i = 1; i <= vertexNB; i++){
            struct Vertex *v_ptr = &v[i]; // The pointer point to the vertex.
            while(1){            
                if(v_ptr != NULL){ // If the pointer isn't point to NULL, the distance will keep adding.
                    wholedistance[i] += v_ptr->path;
                    v_ptr = v_ptr->Node;
                } else {
                    break;
                }
            }
        }
        for(int i = 99; i >= 0; i--)
            filename[i + 1] = filename[i]; // Move the characteristics in the filename to make the filename from input to output.
        filename[0] = 'o';
        filename[1] = 'u';
        filename[2] = 't';
        FILE *output = fopen(filename, "w+"); // Create the file.
        for(int i = 1; i <= vertexNB; i++){
            fprintf(output, "%d %d\n", i, wholedistance[i]); // Put the result to the file.
        }
    }
    return 0;
}