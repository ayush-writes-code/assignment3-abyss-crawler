#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>  // compile with: gcc abyss_crawler.c -lncurses -o abyss

// ---------- CONFIG ----------
#define MAP_W 120   // width of map (make it big)
#define MAP_H 30    // height of map (taller area)


#define OFFSET_X 1        // left margin in terminal
#define OFFSET_Y 1        // top margin in terminal

#define MAX_ENEMIES 20
#define MAX_POTIONS 40
#define MAX_TREASURE 50


 // --- sound function declarations ---
// --- sound function declarations (prototypes) ---
void play_intro_music(void);
void stop_all_sounds(void);
void play_respawn_sound(void);
void play_death_music(void);
void play_heal_sound(void);
void play_money_sound(void);
void play_step_sound(void);


typedef struct {
    int x, y;
    int hp;
    int max_hp;
    int score;
} Player;

typedef struct {
    int x, y;
    int alive;
} Enemy;

char mapData[MAP_H][MAP_W];
Enemy enemies[MAX_ENEMIES];
int enemyCount = 0;

// ---------- TITLE (your ASCII art) ----------
void draw_title() {
    clear();

   attron(COLOR_PAIR(5)); mvprintw(1, 2, " (`-')  _ <-.(`-')             (`-').-> (`-').->                 (`-')  (`-')  _      .->             (`-')  _   (`-')  ");attroff(COLOR_PAIR(5));
    attron(COLOR_PAIR(5)); mvprintw(2, 2, " (OO ).-/  __( OO)      .->    ( OO)_   ( OO)_       _        <-.(OO )  (OO ).-/  (`(`-')/`)   <-.    ( OO).-/<-.(OO )  ");attroff(COLOR_PAIR(5));
   attron(COLOR_PAIR(5)); mvprintw(3, 2, " / ,---.  '-'---.\\  ,--.'  ,-.(_)--\\_) (_)--\\_)      \\-,-----.,------,) / ,---.  ,-`( OO).', ,--. )  (,------.,------,) ");attroff(COLOR_PAIR(5));
   attron(COLOR_PAIR(5)); mvprintw(4, 2, " | \\ /`\\. | .-. (/ (`-')'.'  //    _ / /    _ /       |  .--./|   /`. ' | \\ /`\\. |  |\\  |  | |  (`-') |  .---'|   /`. ' ");attroff(COLOR_PAIR(5));
   attron(COLOR_PAIR(5)); mvprintw(5, 2, " '-'|_.' || '-' `.)(OO \\    / \\_..`--. \\_..`--.      /_) (`-')|  |_.' | '-'|_.' ||  | '.|  | |  |OO )(|  '--. |  |_.' | ");attroff(COLOR_PAIR(5));
   attron(COLOR_PAIR(5)); mvprintw(6, 2, "(|  .-.  || /`'.  | |  /   /) .-._)   \\.-._)   \\     ||  |OO )|  .   .'(|  .-.  ||  |.'.|  |(|  '__ | |  .--' |  .   .' ");attroff(COLOR_PAIR(5));
   attron(COLOR_PAIR(5)); mvprintw(7, 2, " |  | |  || '--'  / `-/   /`  \\       /\\       /    (_'  '--'\\|  |\\  \\  |  | |  ||   ,'.   | |     |' |  `---.|  |\\  \\  ");attroff(COLOR_PAIR(5));
   attron(COLOR_PAIR(5)); mvprintw(8, 2, " `--' `--'`------'    `--'     `-----'  `-----'        `-----'`--' '--' `--' `--'`--'   '--' `-----'  `------'`--' '--'  ");attroff(COLOR_PAIR(5));

    mvprintw(11, 20, "A B Y S S   C R A W L E R");
    mvprintw(13, 25, "Press [ENTER] to start");
    mvprintw(15, 18, "Use W A S D or Arrow Keys to Move");
    mvprintw(16, 20, "Collect '$' | Heal with '+' | Avoid 'm'");
    mvprintw(18, 26, "Press Q to Quit");

    refresh();

}


// ---------- MAP / WORLD ----------

void generate_arena() {
    // Big rectangle: border = '#', inside = '.'
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (y == 0 || y == MAP_H - 1 || x == 0 || x == MAP_W - 1)
                mapData[y][x] = '#';  // walls
            else
                mapData[y][x] = '.';  // open floor
        }
    }
}

void random_floor_pos(int *rx, int *ry) {
    while (1) {
        int x = rand() % MAP_W;
        int y = rand() % MAP_H;
        if (mapData[y][x] == '.') {
            *rx = x;
            *ry = y;
            return;
        }
    }
}

void place_enemies(int count) {
    if (count > MAX_ENEMIES) count = MAX_ENEMIES;
    enemyCount = count;
    for (int i = 0; i < enemyCount; i++) {
        enemies[i].alive = 1;
        random_floor_pos(&enemies[i].x, &enemies[i].y);
    }
}

void place_items() {
    // potions '+'
    for (int i = 0; i < MAX_POTIONS; i++) {
        int x, y;
        random_floor_pos(&x, &y);
        if (mapData[y][x] == '.')
            mapData[y][x] = '+';
    }
    // treasure '$'
    for (int i = 0; i < MAX_TREASURE; i++) {
        int x, y;
        random_floor_pos(&x, &y);
        if (mapData[y][x] == '.')
            mapData[y][x] = '$';
    }
}

int enemy_at(int x, int y) {
    for (int i = 0; i < enemyCount; i++) {
        if (enemies[i].alive && enemies[i].x == x && enemies[i].y == y)
            return i;
    }
    return -1;
}

// ---------- DRAW ----------

void draw_game(Player *p) {
    clear();

    mvprintw(0, OFFSET_X, "HP: %d/%d   Score: %d   (WASD / Arrows to move, Q to quit)",
             p->hp, p->max_hp, p->score);

    // Draw border + inside
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
        char ch = mapData[y][x];

if (ch == '+') {
    attron(COLOR_PAIR(3));
    mvaddch(OFFSET_Y + y, OFFSET_X + x, '+');
    attroff(COLOR_PAIR(3));
    continue;
}

if (ch == '$') {
    attron(COLOR_PAIR(4));
    mvaddch(OFFSET_Y + y, OFFSET_X + x, '$');
    attroff(COLOR_PAIR(4));
    continue;
}


            int ei = enemy_at(x, y);
if (ei != -1) {
    attron(COLOR_PAIR(2));
    mvaddch(OFFSET_Y + y, OFFSET_X + x, 'm');
    attroff(COLOR_PAIR(2));
    continue;
}


            if (p->x == x && p->y == y) {
    attron(COLOR_PAIR(1));
    mvaddch(OFFSET_Y + y, OFFSET_X + x, '@');
    attroff(COLOR_PAIR(1));
    continue;
}

            mvaddch(OFFSET_Y + y, OFFSET_X + x, ch);
        }
    }

    refresh();
}

// ---------- GAME LOGIC ----------
// find a random floor tile '.' that is not too close to enemies
void respawn_player_safe(Player *p) {
        play_respawn_sound();   // 🔊 play when respawning
    while (1) {
        int x = rand() % MAP_W;
        int y = rand() % MAP_H;

        // must be floor
        if (mapData[y][x] != '.') continue;

        int safe = 1;
        for (int i = 0; i < enemyCount; i++) {
            if (!enemies[i].alive) continue;
            int dx = enemies[i].x - x;
            int dy = enemies[i].y - y;
            // distance^2 < 25 means roughly within 5 tiles
            if (dx*dx + dy*dy < 25) {
                safe = 0;
                break;
            }
        }

        if (safe) {
            p->x = x;
            p->y = y;
            return;
        }
    }
}


void move_player(Player *p, int dx, int dy) {
    int nx = p->x + dx;
    int ny = p->y + dy;

    if (nx < 0 || nx >= MAP_W || ny < 0 || ny >= MAP_H) return;
    if (mapData[ny][nx] == '#') return; // wall

    play_step_sound();


    p->x = nx;
    p->y = ny;

    // items
    if (mapData[ny][nx] == '+') {
        p->hp += 4;
            play_heal_sound();   // 🔊 heal sound
        if (p->hp > p->max_hp) p->hp = p->max_hp;
        mapData[ny][nx] = '.';
    } else if (mapData[ny][nx] == '$') {
            play_money_sound();   // 🔊 coin sound
        p->score += 10;
        mapData[ny][nx] = '.';
    }

    // enemy collision -> lose HP and respawn somewhere safer
int ei = enemy_at(nx, ny);
if (ei != -1) {
    p->hp -= 3;
    if (p->hp < 0) p->hp = 0;
    respawn_player_safe(p);
}

}

void move_enemies(Player *p) {
    for (int i = 0; i < enemyCount; i++) {
        if (!enemies[i].alive) continue;

        int dx = 0, dy = 0;

        // Very simple chase / random
        if (rand() % 2 == 0) {
            if (p->x < enemies[i].x) dx = -1;
            else if (p->x > enemies[i].x) dx = 1;
            if (p->y < enemies[i].y) dy = -1;
            else if (p->y > enemies[i].y) dy = 1;
        } else {
            int dir = rand() % 5;
            if (dir == 0) dx = -1;
            else if (dir == 1) dx = 1;
            else if (dir == 2) dy = -1;
            else if (dir == 3) dy = 1;
        }

        int nx = enemies[i].x + dx;
        int ny = enemies[i].y + dy;

        if (nx < 0 || nx >= MAP_W || ny < 0 || ny >= MAP_H) continue;
        if (mapData[ny][nx] == '#') continue;
        if (enemy_at(nx, ny) != -1) continue; // don't stack

        enemies[i].x = nx;
        enemies[i].y = ny;

        if (nx == p->x && ny == p->y) {
    p->hp -= 1;
    if (p->hp < 0) p->hp = 0;
    respawn_player_safe(p);
}

    }
}

//music
void stop_all_sounds(void) {
    system("killall afplay >/dev/null 2>&1");
}

void play_intro_music(void) {
    system("afplay intro.mp3 -v 0.2 &");      // background music, low volume
}

void play_respawn_sound(void) {
    system("afplay respawn.mp3 -v 0.9 &");
}

void play_death_music(void) {
    system("afplay death.mp3 -v 0.9 &");
}

void play_heal_sound(void) {
    system("afplay heal.mp3 -v 0.9 &");
}

void play_money_sound(void) {
    system("afplay coin.mp3 -v 0.9 &");
}

void play_step_sound(void) {
    system("afplay step.mp3 -v 0.4 &");
}

// ---------- MAIN ----------


int main() {
    srand((unsigned int)time(NULL));

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    play_intro_music();

   
    //color coding
    start_color();
use_default_colors();
init_pair(1, COLOR_CYAN, -1);     // Player color
init_pair(2, COLOR_RED, -1);      // Enemies
init_pair(3, COLOR_GREEN, -1);    // Healing
init_pair(4, COLOR_YELLOW, -1);   // Treasure
init_pair(5, COLOR_MAGENTA, -1); //Title color




    // TITLE SCREEN
    draw_title();
    int ch;
    while ((ch = getch()) != '\n' && ch != KEY_ENTER) {
        // wait for ENTER
    }


    // INIT GAME
    Player player;
    player.max_hp = 20;
    player.hp = player.max_hp;
    player.score = 0;

    generate_arena();
    place_items();
    place_enemies(25);

    // start in CENTER of arena
    player.x = MAP_W / 2;
    player.y = MAP_H / 2;

    int running = 1;
    while (running) {
        draw_game(&player);

        int key = getch();
        if (key == 'q' || key == 'Q') {
                stop_all_sounds();   // 🔇 stop background music
            running = 0;
            break;
        } else if (key == KEY_UP || key == 'w' || key == 'W') {
            move_player(&player, 0, -1);
        } else if (key == KEY_DOWN || key == 's' || key == 'S') {
            move_player(&player, 0, 1);
        } else if (key == KEY_LEFT || key == 'a' || key == 'A') {
            move_player(&player, -1, 0);
        } else if (key == KEY_RIGHT || key == 'd' || key == 'D') {
            move_player(&player, 1, 0);
        }

        move_enemies(&player);

        if (player.hp <= 0) {
                stop_all_sounds();   // stop any previous afplay

                play_death_music();   // 🔊 death music
            running = 0;
        }
    }

    // GAME OVER
    clear();
    mvprintw(5, 10, "You were consumed by the Abyss...");
    mvprintw(7, 10, "Final Score: %d", player.score);
    mvprintw(9, 10, "Press any key to exit.");
    refresh();
    getch();

    endwin();
    return 0;
}
