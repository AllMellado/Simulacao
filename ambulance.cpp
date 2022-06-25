#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <list>

std::random_device rd; 
std::mt19937 gen(rd()); 
std::uniform_real_distribution<> dis(0, 1.0);

#define pop_size 100
#define map_size 100
#define hospt_x 25
#define hospt_y 25
#define num_cars 5
#define num_events 100
#define best_num 25

#define FOR(i,x) for( int i = 0; i < x; i++)
#define print std::cout <<

struct event{
    int x, y, hour, type;
};

struct car{
    int st_x, st_y;
    int x, y;
    char key;
    char status;
    event event;
};

struct set{
    int fit;
    car cars[num_cars];

    bool operator<( set &val ){ 
        return fit < val.fit; 
    }
};

int minute = 0;
int mins_taken = 0;
int ev_counter = 0;
bool ev_end = false;
bool running = true;

double alpha = 5;

std::vector<std::string> str;

char map[map_size][map_size];
char mapAux[map_size][map_size];
std::vector<set> pop;
car *car_list; 
event ev_list[num_events];

void clearMap();
void createPop();
void printMap();
void printPop();

void insertCars(car cars[num_cars]);
void check_coord(car *car);
void move_function(car *car);
void simulatePop();
void repopulate();
void getEvents();
void turn();

int main(){
    clearMap();
    createPop();
    //printPop();
    getEvents();
    print "Iterations: ";
    FOR(i,50){
        print i << " ";
        simulatePop();
        repopulate();
        alpha *= 0.99;
    }
    std::sort( pop.begin(), pop.end() );
    print "\nbest coords ";
    for( auto &car : pop.at(0).cars ){
        print "[" << car.x << "," << car.y << "] ";
    }
    print "Best fitness: " << pop.at(0).fit;
}

void clearMap(){
    FOR(i,map_size)
        FOR(j,map_size)
            map[i][j] = ' ';
    map [hospt_x][hospt_y] = 'H';
}

void createPop(){
    int x, y;
    FOR(i,pop_size){
        set set;
        FOR(j,num_cars){
            bool again = true;
            set.cars[j].key = 'A';
            set.cars[j].status = 'F';
            do{
                x = map_size*dis(gen);
                y = map_size*dis(gen);
                set.cars[j].st_x = x;
                set.cars[j].st_y = y;
                set.cars[j].x = x;
                set.cars[j].y = y;
                set.cars[j].event.x = x;
                set.cars[j].event.y = y;
                if( j == 0 ) again = false;
                FOR(ii,j) 
                    if( (set.cars[ii].x != x || set.cars[ii].y != y) && map[x][y] != 'H')
                        again = false;
            }while(again);
        }
        pop.push_back(set);
    }
}

void getEvents(){
    FOR(i,num_events){
        ev_list[i].x = map_size*dis(gen);
        ev_list[i].y = map_size*dis(gen);
        if( i != 0 ) 
            ev_list[i].hour = ev_list[i-1].hour + 5*dis(gen);
        else
            ev_list[i].hour = 5*dis(gen);
        ev_list[i].type = 2*dis(gen);
    }
}        

void simulatePop(){
    int i =0;
    for(auto &set: pop){
        car_list = set.cars;
        insertCars(car_list);
        while(running){
            turn();
        }
        i++;
        clearMap();
        set.fit = mins_taken;
        if(mins_taken <= 0){
            for(auto &s: str){
                print s;
            }
        }
        str.clear();
        mins_taken = 1;
        ev_counter = 0;
        minute = 0;
        ev_end = false;
        running = true;
    }
}

void turn(){
    memcpy(mapAux, map, map_size*map_size*sizeof(char));
    if( ev_counter < num_events && minute >= ev_list[ev_counter].hour*60 ){
        double dist, min_dist = 10000;
        int car = -1;
        FOR(i,num_cars){
            dist = sqrt(pow(ev_list[ev_counter].x - car_list[i].x,2) + 
                        pow(ev_list[ev_counter].y - car_list[i].y,2)); 
            if( dist < min_dist && car_list[i].status == 'F' ){
                min_dist = dist;
                car = i;
            }
        }
        if( car != -1 ){
            car_list[car].event = ev_list[ev_counter];
            car_list[car].status = 'P';
            ev_counter++;
        }
    }
    if( ev_counter >= num_events ){
        ev_end = true;
    }
    FOR(i,num_cars){
        mapAux[car_list[i].x][car_list[i].y] = ' ';
        move_function(&car_list[i]);
        check_coord(&car_list[i]);
        mapAux[car_list[i].x][car_list[i].y] = car_list[i].key;
    }
    minute++;
    memcpy(map, mapAux, map_size*map_size*sizeof(char));
}


void insertCars(car cars[num_cars]){
    FOR(i,num_cars){
        map[cars[i].st_x][cars[i].st_y] = cars[i].key;
    }
}

void repopulate(){
    //printPop();
    std::sort( pop.begin(), pop.end() );
    //printPop();
    int x, y;
    bool again;
    int pos = best_num;
    FOR(i,best_num){
        FOR(w,pop_size/best_num-1){
            set set;
            FOR(j,num_cars){
                pop.at(i).cars[j].status = 'F';
                again = true;
                do{
                    x = pop.at(i).cars[j].st_x + alpha*(-10+20*dis(gen));
                    y = pop.at(i).cars[j].st_y + alpha*(-10+20*dis(gen));
                    if( x >= map_size ) x = map_size - 1;
                    if( y >= map_size ) y = map_size - 1;
                    if( x < 0 ) x = 0;
                    if( y < 0 ) y = 0;
                    set.cars[j].st_x = x;
                    set.cars[j].st_y = y;
                    set.cars[j].x = x;
                    set.cars[j].y = y;
                    set.cars[j].event.x = x;
                    set.cars[j].event.y = y;
                    set.cars[j].status = 'F';
                    if( j == 0 ) again = false;
                    FOR(ii,j) 
                        if( (set.cars[ii].x != x || set.cars[ii].y != y) && map[x][y] != 'H')
                            again = false;
                }while(again);
            }
            //print pos << " " << pop.at(pos).cars[0].st_x << "," << pop.at(pos).cars[0].st_y << " ";
            pop.at(pos) = set;
            //print pop.at(pos).cars[0].st_x << "," << pop.at(pos).cars[0].st_y << "\n";
            pos++;
        }
    }
    //printPop();
}

void check_coord(car *car){
    if( car->x == car->event.x && car->y == car->event.y  ){
        if( car->status == 'P' ){
            mins_taken += minute - car->event.hour*60;
            std::stringstream a;
            a << "(" << minute << " " << car->event.hour*60 << ")\n";
            str.push_back(a.str());
        }
        if( car->event.type == 1 ){
            car->event.x = hospt_x;
            car->event.y = hospt_y;
            car->event.type = 0;
            car->status = 'H';
        }
        if( car->event.type == 0 ){
            car->event.x = car->st_x;
            car->event.y = car->st_y;
            car->event.type = -1;
            car->status = 'F';
            if( ev_end ){
                running = false;
            }
        }
    }
}

void move_function(car *car){
    if( car->x < car->event.x){
        car->x++;
    }else if( car->x > car->event.x ){
        car->x--;
    }else if( car->y < car->event.y ){
        car->y++;
    }else if( car->y > car->event.y){
        car->y--;
    }
}

void printMap(){
    for( auto &row : map ){
        std::cout << "|";
        for( auto &x : row )
            std::cout << "  " <<  x;
        std::cout << " |\n";
    }
    std::cout << " \n";
}

void printPop(){
    for( auto &row : pop ){
        for( auto &car : row.cars )
            std::cout << " [" << car.st_x << "," << car.st_y <<"]";
        std::cout << " \n";
    }
    std::cout << " \n";
}

