#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

void move_forward(int size, int begin, vector<int> &cache);
int in_set(int size, vector<int> &cache, int address);

int main(int argc, char **argv){
    string input;
    ifstream trace(argv[1]);
    ofstream output;
    output.open(argv[2]);
    long long cache_word = 0;
    long long block_word = 0;
    long long associativity = 0; // 0:directed map、1:four_way set associative、2:fully associative
    long long replace = 0; // 0: FIFO、1:LRU    
    getline(trace, input);
    cache_word = stoi(input);
    getline(trace, input);
    block_word = stoi(input);
    getline(trace, input);
    associativity = stoi(input);
    getline(trace, input);
    replace = stoi(input);    
    long long blocks = cache_word / block_word;
    long long temp = blocks;
    long long offset = block_word * 4;
    vector<int> directed_cache;
    vector<vector<int>> asso_cache;
    double miss_num = 0, hit_num = 0, miss_rate = 0;
    long long tag = 0, index = 0;
    long long index_bit = 0, offset_bit = 0;
    long long set_num = 0;
    while(offset != 1){
        offset_bit++;
        offset = offset >> 1;
    }
    while(temp != 1){
        index_bit++;
        temp = temp >> 1;
    }
    long long address = 0;    
    switch(associativity){
        case 0:
            directed_cache.resize(blocks);
            while(getline(trace, input)){
                address = stoi(input);
                address = address << 2;
                // offset
                address = address >> offset_bit;
                // tag
                tag = address >> index_bit;
                // index
                if(tag)
                    index = address % blocks;
                else
                    index = address;                
                if(directed_cache[index] == 0){
                    output << "-1\n";
                    miss_num++;
                    directed_cache[index] = tag;
                } else if(directed_cache[index] && directed_cache[index] != tag){
                    output << directed_cache[index] << "\n";
                    miss_num++;
                    directed_cache[index] = tag;
                } else {
                    output << "-1\n";
                    hit_num++;
                }                
            }
                break;
        case 1:
            set_num = blocks / 4;
            asso_cache.resize(set_num);
            index_bit -= 2;
            while(getline(trace, input)){
                address = stoi(input);
                address = address << 2;
                // offset
                address = address >> offset_bit;
                // tag                
                tag = address >> index_bit;
                // index
                if(tag)
                    index = address % set_num;
                else
                    index = address;                
                if(asso_cache[index].size() != 4 && in_set(asso_cache[index].size(), asso_cache[index], tag) == -1){
                    output << "-1\n";
                    asso_cache[index].push_back(tag);
                    miss_num++;             
                } else if(in_set(asso_cache[index].size(), asso_cache[index], tag) != -1){
                    output << "-1\n";
                    move_forward(asso_cache[index].size(), in_set(asso_cache[index].size(), asso_cache[index], tag), asso_cache[index]);
                    hit_num++;
                } else {
                    output << asso_cache[index][0] << "\n";
                    asso_cache[index][0] = tag;
                    move_forward(4, 0, asso_cache[index]);
                    miss_num++;
                }
            }
            break;
        case 2:
            set_num = blocks / blocks;
            asso_cache.resize(set_num);            
            index = 0;
            while(getline(trace, input)){
                address = stoi(input);
                address = address << 2;
                // offset
                address = address >> offset_bit;
                // tag                
                tag = address;                
                if(asso_cache[index].size() != blocks && in_set(asso_cache[index].size(), asso_cache[index], tag) == -1){
                    output << "-1\n";
                    asso_cache[index].push_back(tag);
                    miss_num++;             
                } else if(in_set(asso_cache[index].size(), asso_cache[index], tag) != -1){
                    output << "-1\n";
                    move_forward(asso_cache[index].size(), in_set(asso_cache[index].size(), asso_cache[index], tag), asso_cache[index]);
                    hit_num++;
                } else {
                    output << asso_cache[index][0] << "\n";
                    asso_cache[index][0] = tag;
                    move_forward(4, 0, asso_cache[index]);
                    miss_num++;
                }
            }
            break;
            
    }    
    output << fixed << setprecision(6) << "Miss rate = " << miss_num / (miss_num + hit_num);
    return 0;
}

void move_forward(int size, int begin, vector<int> &cache){   
    int temp = cache[begin]; 
    for(int i = begin; i < size - 1; i++){
        cache[i] = cache[i+1];
    }
    cache[size-1] = temp;    
}

int in_set(int size, vector<int> &cache, int address){
    for(int i = 0; i < size; i++){
        if(cache[i] == address)
            return i;
    }
    return -1;
}