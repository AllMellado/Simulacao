#include <random>
#include <iostream>
#include <algorithm>
#include <list>

std::random_device rd; 
std::mt19937 gen(rd()); 
std::uniform_real_distribution<> dis(0, 1.0);

#define size 50
#define default_type 0
#define default_range 1
#define default_close_diff 3
#define default_close_same 3

struct cell {
    char key;
    int type;
    int range;
    int num_diff;
    int num_same;
    bool (*rule)();
};

cell map[size][size];
cell mapAux[size][size];

void turn();
int checkCloseDiff(int x, int y, char key);
int checkCloseSame(int x, int y, char key);
void changeLocation(int i, int j);
void printMap();
void startCells(double perc, char key, int type=default_type, int range=default_range, 
                int num_diff=default_close_diff, int num_same=default_close_same);

// //                            
// type 0: check only diff key           
// type 1: check only same key           
// type 2: check both diff and same key    
// type 3: check diff for empty space 
// type 4: check same for empty space 
// //      

int main(){
    // Encher o mapa com células de char vazio sem mais informacao 
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            map[i][j].key = ' ';

    // Iniciar duas celulas com chaves diferentes mas regras iguais
    startCells(0.4, 'O');
    startCells(0.4, 'X');

    // Exemplo de uma incialização diferente:
    // Cria 10% do tamanho total, key '@', regra tipo 2, range 2, 8 diff, 12 same;
    // startCells(0.1, '@', 2, 2, 8, 12)

    printMap();
    
    for(int i=0; i < 1000; i++){
        turn();
    }
    printMap();
}

void turn(){
    memcpy(mapAux, map, size*size*sizeof(cell));
    char key;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if( map[i][j].key != ' ' ){
                key = map[i][j].key;
                
            }
        }
    }
    memcpy(map, mapAux, size*size*sizeof(cell));
}

bool diff(){
    checkCloseDiff(i,j,key) >= map[i][j].num_diff
    return false;
}

bool same(){
    checkCloseSame(i,j,key) < map[i][j].num_same
    return false;
}

bool diff_same(){
    checkCloseDiff(i,j,key) >= map[i][j].num_diff || checkCloseSame(i,j,key) < map[i][j].num_same
    return false;
}

bool diff_empty(){
    checkCloseDiff(i,j,' ') >= map[i][j].num_diff
    return false;
}

bool same_empty(){
    checkCloseSame(i,j,' ') < map[i][j].num_same
    return false;
}

int checkCloseDiff(int x, int y, char key){
    int sumClose = 0;
    int range = map[x][y].range;
    for(int i = -range; i < range+1; i++){
        for(int j = -range; j < range+1; j++){
            if( x+i >= 0 && y+j >= 0 && x+i < size && y+j < size ){
                if( map[x+i][y+j].key != ' ' && map[x+i][y+j].key != key ){
                    sumClose++;
                }
            }
        }
    }
    return sumClose;
}

int checkCloseSame(int x, int y, char key){
    int sumClose = 0;
    int range = map[x][y].range;
    for(int i = -range; i < range+1; i++){
        for(int j = -range; j < range+1; j++){
            if( x+i >= 0 && y+j >= 0 && x+i < size && y+j < size ){
                if( map[x+i][y+j].key == key ){
                    sumClose++;
                }
            }
        }
    }
    return sumClose;
}

void changeLocation(int i, int j){
    int x, y;
    bool again = false;
    do{
        x = size*dis(gen);
        y = size*dis(gen);
        if( mapAux[x][y].key == ' ' ){
            memcpy( &mapAux[x][y], &map[i][j], sizeof(cell));
            mapAux[i][j].key = ' ';
            again = false;
        }else{
            again = true;
        }
    }while(again);
}

void printMap(){
    for( auto &row : map ){
        for( auto &x : row )
            std::cout << "  " <<  x.key ;
        std::cout << " \n";
    }
    std::cout << " \n";
}

void startCells(double perc, char key, int type, int range, int num_diff, int num_same){
    int length = perc*size*size;
    int x, y;
    for( int i = 0; i < length; i++){
        bool again = false;
        do{
            x = size*dis(gen);
            y = size*dis(gen);
            if( map[x][y].key == ' ' ){
                map[x][y].key = key;
                map[x][y].type = type;
                map[x][y].range = range;
                map[x][y].num_diff = num_diff;
                map[x][y].num_same = num_same;
                again = false;
            }else{
                again = true;
            }
        }while(again);
    }

}




