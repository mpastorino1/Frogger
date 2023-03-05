#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

//
entity_s bullet(position_s position);
//B
bool checkStreetCollision(entity_s cars[], entity_s player);
bool checkRiverCollision(entity_s player);
void clearZone(int endX, int endY);
bool checkNearby(bool right, entity_s current, entity_s cars[]);
bool checkCollision(entity_s bullet, entity_s cars[], int damage);
bool checkPlayerLimits(int x, int y);
//D
void drawScene(int points, int countdown, int hp, entity_s player, entity_s cars[], entity_s logs[],entity_s * bullets, int bulletCount);
void defineColors();
//E
int enemiesAlive(entity_s enemies[]);
//I
void initScreen();
void initCars(entity_s * cars);
void initCar(entity_s car);
void initLogs(entity_s * logs);
entity_s initEntity(int type, char extra, int x, int y, int health);
//P
void printEnd(char *msg);
void printCars(entity_s car);
void printLogs(entity_s log);
void printPlayer(entity_s player);
//R
bool range(int min, int max, int value);
int rnd(int MAX, int MIN);
//U
void updateLogs(entity_s * logs);

#endif
