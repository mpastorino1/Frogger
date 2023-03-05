#include "libs.h"

bool range(int min, int max, int value) {
    return (min <= value && value <= max);
}

void initScreen(){
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    start_color();
    defineColors();
}

entity_s initEntity(int type, char extra, int x, int y, int health) {
    entity_s temp;
    temp.health = health;
    temp.extra = extra;
    temp.pos.x = x;
    temp.pos.y = y;
    temp.type = type;
    return temp;
}

void clearZone(int endX, int endY) {
    for (int y = 0; y < endY; y++) {
        for (int x = 0; x < endX + 10; x++) {
            mvaddch(y, x, ' ');
        }
    }
}

void printEnd(char * msg) {
    flash();
    flash();
    flash();

    int msgSize = strlen(msg);

    mvprintw(S_HEIGHT / 2, S_WIDTH / 2 - msgSize / 2, msg);
    mvprintw(S_HEIGHT / 2 + 1, S_WIDTH / 2 - 12, "Premi \"ENTER\" per uscire");
    //  refresh();
    char c;
    do {
        c = getch();
        cbreak();
    } while (c != '\n');
}

void initCars(entity_s * cars) {

    int screenWidth = getmaxx(stdscr);
    int street[5] = { S_HEIGHT - 16, S_HEIGHT - 18, S_HEIGHT - 20, S_HEIGHT - 22, S_HEIGHT - 24};
    int dir[5] = { -1, 1, -1, 1, -1};

    for (int i = 0; i < CAR_N; i++) {
        int lane = rnd(5, 0);
        cars[i].health = 99;
        cars[i].pos.x = rnd(S_WIDTH - CAR_WIDTH + 1, 0 + CAR_WIDTH); //distanza dal bordo
        cars[i].pos.y = street[lane];
        cars[i].extra = dir[lane];
    }
    
    for (int i = 0; i < CAR_N; i++){
        for (int j = 0; j < CAR_N; j++){
            for (int k = 0; k < CAR_WIDTH; k++){
                if (cars[i].pos.x + k == cars[j].pos.x + k){
                    cars[i].pos.x += CAR_WIDTH;
                }
            }
        }
    }
}

void initCar(entity_s car) {

    int screenWidth = getmaxx(stdscr);
    int street[5] = { S_HEIGHT - 16, S_HEIGHT - 18, S_HEIGHT - 20, S_HEIGHT - 22, S_HEIGHT - 24};
    int dir[5] = { -1, 1, -1, 1, -1};
    int lane = rnd(5, 0);
    car.pos.y = street[lane];
    car.extra = dir[lane];
    if(car.extra == 1){
        car.pos.x = 0 - CAR_WIDTH;
    } else {
        car.pos.x = S_WIDTH + CAR_WIDTH;
    }
}

void initLogs (entity_s * logs){
	int screenWidth = getmaxx(stdscr);
    int space_occupied = LOG_WIDTH * (LOG_N + 2 * LOG_N - 1) + 2;
    int border = (screenWidth - space_occupied) / 2;
	int river[5] = { S_HEIGHT - 28, S_HEIGHT -30, S_HEIGHT - 32, S_HEIGHT - 34, S_HEIGHT -36};
	int dir[5] = { 1, -1, 1, -1, 1};

	for (int i = 0; i < LOG_N; i++) {
        logs[i].health = 99;
        logs[i].pos.x = rnd(S_WIDTH - (LOG_WIDTH + 1), LOG_WIDTH + 1); //distanza dal bordo
        logs[i].pos.y = river[i];
		logs[i].extra = dir[i];
    }
}

entity_s initPlayer();

void playerP(int pipe);

void clearPlayer(entity_s player) {
    int startX = player.pos.x, startY = player.pos.y;

    mvaddch(startY, startX + 2, ' ');
    for (int i = 0; i < 5; i++) {
        mvaddch(startY + 1, startX + i, ' ');
    }
    mvaddch(startY + 2, startX, ' ');
    mvaddch(startY + 2, startX + 2, ' ');
    mvaddch(startY + 2, startX + 4, ' ');
    refresh();
}

void printPlayer(entity_s player) {

    /*
     
    Aspetto del player :
     
            ##
            ##
     
    */
    
    int startX = player.pos.x, startY = player.pos.y;
    for (int i = 0; i < 2; i++) {
        attron(COLOR_PAIR(6));
        mvaddch(startY, startX + i, 'O');
        mvaddch(startY + 1, startX + i, 'O');
        attroff(COLOR_PAIR(6));
    }
    refresh();
}

bool checkPlayerLimits(int x, int y) {
    if (x <= S_WIDTH - 2 && x >= 0 && y >= S_HEIGHT - 36 && y <= S_HEIGHT - 14) { // limiti
        return true;
    } else if (y <= S_HEIGHT - 36 && y >= S_WIDTH -38) { // limite superiore se mi trovo all'interno delle tane
        if ((x >= 2 && x < 6) || (x >= 10 && x < 14) || (x >= 18 && x < 22) || (x >= 26 && x < 30) || (x >= 34 && x < 38)) {
            return true;
        }
    }
    return false;
}


void printCars(entity_s cars) {

    int startX = cars.pos.x, startY = cars.pos.y;
    
    if(startX < 0 - CAR_WIDTH + 2 || startX + CAR_WIDTH >= S_WIDTH || startY < S_HEIGHT - 24 || startY + 2 > S_HEIGHT)
        return;
    
    if(cars.extra == 0){
        attron(COLOR_PAIR(2));
        for (int i = 0; i < CAR_WIDTH; i++) {
            mvaddch(startY, startX + i, 'O');
            mvaddch(startY + 1, startX + i, 'O');
        }
        attroff(COLOR_PAIR(2));
        refresh();
    } else {
        attron(COLOR_PAIR(1));
        for (int i = 0; i < CAR_WIDTH; i++) {
            mvaddch(startY, startX + i, 'O');
            mvaddch(startY + 1, startX + i, 'O');
        }
        attroff(COLOR_PAIR(1));
        refresh();
    }
    
}

void printLogs(entity_s logs) {

    int startX = logs.pos.x, startY = logs.pos.y;
    
   	if(startX < 0 || startX + LOG_WIDTH >= S_WIDTH || startY < 0 || startY + 2 > S_HEIGHT)
        return;
    
    attron(COLOR_PAIR(7));
    for (int i = 0; i < LOG_WIDTH; i++) {
        mvaddch(startY, startX + i, 'O');
        mvaddch(startY + 1, startX + i, 'O');
    }
    attroff(COLOR_PAIR(7));
    refresh();
}

void updateLogs(entity_s *logs) {
 
}

entity_s bullet(position_s position) {
    entity_s temp;
    temp.pos = position;
    temp.health = 100;
    temp.pos.y -= 1;

    return temp;
}

bool checkStreetCollision(entity_s cars[], entity_s player) {
    for (int i = 0; i < CAR_N; i++){
        for(int j = 0; j < CAR_WIDTH; j++){
            if((cars[i].pos.x + j == player.pos.x || cars[i].pos.x + j == player.pos.x + 1) && (cars[i].pos.y + 1 == player.pos.y || cars[i].pos.y + 1 == player.pos.y + 1) )
                return true;
        }
    }
    return false;
}

bool checkRiverCollision(entity_s player) {
    if( player.pos.y == S_HEIGHT - 28 || player.pos.y == S_HEIGHT - 30 || player.pos.y == S_HEIGHT - 32 || player.pos.y == S_HEIGHT - 34 || player.pos.y == S_HEIGHT - 36){
        return true;
    }
    return false;
}

int enemiesAlive(entity_s enemies[]) {
    int count = 0;
    for (int i = 0; i < ENEMY_N; i++)
        if (enemies[i].health > 0)
            count++;
    return count;
}

bool checkNearby(bool right, entity_s current, entity_s cars[]) {
    //direction == true = dx, else sx
    int whereCheck = 0;
    int i = 0;

    if (right)
        whereCheck = CAR_WIDTH;
    else
        whereCheck = S_WIDTH - 5;
    
    //navi di primo tipo contro navi di primo tipo
    for (i = 0; i < CAR_N; i++)
        if (cars[i].pos.x == current.pos.x + whereCheck)
            return true;
    
    return false;
}

bool checkCollision(entity_s bullet, entity_s enemies[], int damage) {
    //se il proiettile è "morto" non può esserci collisione
    if (!bullet.health)
        return false;

    /*
     * controlliamo per ogni nemico vivo se è avvenuta una collisione
     * il proiettile può collidere solo con un nemico alla volta
     * variabili.. "varie", l'intero contatore, le coordinate del proiettile e la dimensione dei nemici
     */
    int i = 0;
    int x = bullet.pos.x;
    int y = bullet.pos.y;
    int enemySize = 0;

    for (i = 0; i < ENEMY_N; i++) {
        entity_s enemy = enemies[i];
        //dobbiamo controllare se il proiettile è compreso negli estremi delle coordinate
        //solo se il nemico è vivo
        if (enemy.health > 0) {
            position_s ePos = enemy.pos;
            //controlliamo che il proiettile colpisca solo ciò che è rimasto della nave
            // cambiando il range dove viene controllata la posizione
            if (enemy.health > 0) enemySize = 1;
            else if (enemy.health > 40) enemySize = 2;
            else if (enemy.health > 70) enemySize = 3;
            //il controllo della collisione vero e proprio
            if (range(ePos.x, ePos.x + enemySize, x) && range(ePos.y, ePos.y + enemySize, y)) {
                enemies[i].health -= damage;
                return true;
            }
        }
    }
    return false; //-1;}
}


void drawScene(int points, int countdown, int hp, entity_s player, entity_s cars[], entity_s logs[], entity_s * bullets, int bulletCount) {
    
	init_color(COLOR_MYGREEN, 0, 400, 0);
	clearZone(S_WIDTH, S_HEIGHT - 1);  //cancella schermata

    //il punteggio
	attron(COLOR_PAIR(0));
    mvprintw(S_HEIGHT - 12, 0, "PUNTI: %d", points);
    attroff(COLOR_PAIR(0));

	//barra del tempo
	attron(COLOR_PAIR(0));
	mvprintw(S_HEIGHT -12, 31, "TEMPO: %02d", countdown);
	attroff(COLOR_PAIR(0));	
	
	//vita
	attron(COLOR_PAIR(0));
	mvprintw(S_HEIGHT -12, 17, "HP: %d", hp);
	attroff(COLOR_PAIR(0)); 
    
    // marciapiede
    attron(COLOR_PAIR(4));
    for (int i = 0; i < S_WIDTH; i++){
        mvprintw(S_HEIGHT - 13, i, "O");
        mvprintw(S_HEIGHT - 14, i, "O");
        
        mvprintw(S_HEIGHT -25, i, "O");
        mvprintw(S_HEIGHT -26, i, "O");
    }
    attroff(COLOR_PAIR(4));

	//strada
	attron(COLOR_PAIR(0));
	for (int i = 0; i < S_WIDTH; i++){
		if(i % 3){
			mvprintw(S_HEIGHT -17, i, "_");
			mvprintw(S_HEIGHT -19, i, "_");
			mvprintw(S_HEIGHT -21, i, "_");
			mvprintw(S_HEIGHT -23, i, "_");
		}
	}
	

	//fiume
	attron(COLOR_PAIR(3));
	for (int i = 0; i < S_WIDTH; i++){
		mvprintw(S_HEIGHT -36, i, " ");
        mvprintw(S_HEIGHT -35, i, " ");
		mvprintw(S_HEIGHT -34, i, " ");
        mvprintw(S_HEIGHT -33, i, " ");
		mvprintw(S_HEIGHT -32, i, " ");
        mvprintw(S_HEIGHT -31, i, " ");
		mvprintw(S_HEIGHT -30, i, " ");
        mvprintw(S_HEIGHT -29, i, " ");
		mvprintw(S_HEIGHT -28, i, " ");
        mvprintw(S_HEIGHT -27, i, " ");
	}
	attroff(COLOR_PAIR(3));
	
    
    // tane
    // riga 40 E 39
    attron(COLOR_PAIR(5));
    for (int i = 0; i < S_WIDTH; i++) {
        mvprintw(S_HEIGHT - 40, i, "O");
		mvprintw(S_HEIGHT - 39, i, "O");
    }
    attroff(COLOR_PAIR(5));
    
    // riga 38-37
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 40; col++) {
            if (col < 2 || col > 37 ) {
                attron(COLOR_PAIR(5));
                mvprintw(S_HEIGHT - 38 + row, col, "O");
                attroff(COLOR_PAIR(5));
            } else if (col >= 2 & col < 6) {
                attron(COLOR_PAIR(3));
                mvprintw(S_HEIGHT - 38 + row, col, " ");
                attroff(COLOR_PAIR(3));
            } else if (col > 5 & col < 10) {
                attron(COLOR_PAIR(5));
                mvprintw(S_HEIGHT - 38 + row, col, "O");
                attroff(COLOR_PAIR(5));
            } else if (col > 9 & col < 14){
                attron(COLOR_PAIR(3));
                mvprintw(S_HEIGHT - 38 + row, col, " ");
                attroff(COLOR_PAIR(3));
            } else if ( col > 13 & col < 18){
                attron(COLOR_PAIR(5));
                mvprintw(S_HEIGHT - 38 + row, col, "O");
                attroff(COLOR_PAIR(5));
            } else if ( col > 17 & col < 22){
                attron(COLOR_PAIR(3));
                mvprintw(S_HEIGHT - 38 + row, col, " ");
                attroff(COLOR_PAIR(3));
            } else if ( col > 21 & col < 26){
                attron(COLOR_PAIR(5));
                mvprintw(S_HEIGHT - 38 + row, col, "O");
                attroff(COLOR_PAIR(5));
            } else if ( col > 25 & col < 30){
                attron(COLOR_PAIR(3));
                mvprintw(S_HEIGHT - 38 + row, col, " ");
                attroff(COLOR_PAIR(3));
            } else if ( col > 29 & col < 34){
                attron(COLOR_PAIR(5));
                mvprintw(S_HEIGHT - 38 + row, col, "O");
                attroff(COLOR_PAIR(5));
            } else {
                attron(COLOR_PAIR(3));
                mvprintw(S_HEIGHT - 38 + row, col, " ");
                attroff(COLOR_PAIR(3));
            }
            
        }
    }
    attroff(COLOR_PAIR(5));

	//tronchi
	for (int i = 0; i < LOG_N; i++) {
        printLogs(logs[i]);
    }

	//stampo il giocatore
    printPlayer(player);

	//le macchine
    for (int i = 0; i < CAR_N; i++) {
        printCars(cars[i]);
	}
    
    //i proiettili, per il colore fare riferimento alla funzione defineColors()
    attron(COLOR_PAIR(6));
    for (int i = 0; i < bulletCount; i++) {
        if (bullets[i].health > 0)
            mvaddch(bullets[i].pos.y, bullets[i].pos.x, '^');
        refresh();
    }
    attroff(COLOR_PAIR(6));

    //aggiorno il tutto
    refresh();
}

int rnd(int MAX, int MIN) {
    return MIN + rand() % (MAX - MIN + 1);
}

void defineColors() {
    //definisce le coppie di colori
    if (has_colors()) {
        init_pair(0, COLOR_WHITE, COLOR_BLUE);		//info, strada
        init_pair(1, COLOR_WHITE, COLOR_RED);		//car
    	init_pair(2, COLOR_RED, COLOR_WHITE);		//car
        init_pair(3, COLOR_WHITE, COLOR_BLUE);	 	//fiume
        init_pair(4, COLOR_CYAN, COLOR_MAGENTA);	//marciapiede
        init_pair(5, COLOR_MAGENTA, COLOR_GREEN);	//tane
        init_pair(6, COLOR_GREEN, COLOR_MYGREEN);	//player  
    	init_pair(7, COLOR_GREEN, COLOR_YELLOW);	//trochi	
    }
}
