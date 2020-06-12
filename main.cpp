#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include "Enums.h"
#include "Vector2D.h"
#include "Entity.h"
#include "Map.h"

// Globals

bool oldKeyDown[256];
bool keyDown[256];

// Functions

void Loop(void);
void Init(void);
void Display(void);
void Clear(void);
void Update(void);
void PollInput(void);

// Classes

class Player;
class Ghost;
class Item;
class Dot;
class Fruit;

// Enumerables

// Game Functions

void Init()
{
}

void Loop()
{
}

class Player : public Entity
{
private:
    Direction dir;
    int lives;
    int score;
    float speed;
    int animFrame;
    char sprite[4][2];

public:
    Player(Vector2D p)
    {
        pos = p;
        dir = DIR_RIGHT;
        animFrame = 0;
        lives = 3;
        score = 0;
        speed = 0.5f;
        sprite[DIR_UP][0] = 'u';
        sprite[DIR_UP][1] = 'o';
        sprite[DIR_DOWN][0] = 'n';
        sprite[DIR_DOWN][1] = 'o';
        sprite[DIR_LEFT][0] = ')';
        sprite[DIR_LEFT][1] = '0';
        sprite[DIR_RIGHT][0] = '(';
        sprite[DIR_RIGHT][1] = '0';
        image = sprite[dir][animFrame];
    }
    void update(Map *m)
    {
        Vector2D dest;
        Vector2D step;
        ObjectID currentBlock, destBlock;

        if (keyDown['w'])
        {
            // std::cout << "UP!";
            dir = DIR_UP;
        }
        if (keyDown['a'])
        {
            // std::cout << "LEFT!";
            dir = DIR_LEFT;
        }
        if (keyDown['s'])
        {
            // std::cout << "DOWN!";
            dir = DIR_DOWN;
        }
        if (keyDown['d'])
        {
            // std::cout << "RIGHT!";
            dir = DIR_RIGHT;
        }

        if (dir == DIR_DOWN)
        {
            // std::cout << "down";
            step = Vector2D(0, speed);
            // std::cout << step.x << " : " << step.y;
        }
        else if (dir == DIR_UP)
        {
            step = Vector2D(0, -speed);
        }
        else if (dir == DIR_LEFT)
        {
            step = Vector2D(-speed, 0);
        }
        else if (dir == DIR_RIGHT)
        {
            step = Vector2D(speed, 0);
        }
        else
            step = Vector2D(0, 0);

        dest = pos + step;
        // std::cout << speed << std::endl;
        // std::printf("(%f,%f) = (%f,%f) + (%f,%f)\n", dest.x, dest.y, pos.x, pos.y, step.x, step.y);
        /*
            Query map for items, or dots, or ghosts, on the current pos.
                -In case item, dot ...
                -In case ghost, ...
        */

        currentBlock = m->queryMap(pos);
        if (currentBlock == ID_DOT)
        {
            std::cout << "DOT___";
            m->setBlock(pos, ' ');
            score += 100;
        }
        else if (currentBlock == ID_FRUIT)
        {
            std::cout << "FRUIT_";
            m->setBlock(pos, ' ');
            score += 1000;
        }
        else if (currentBlock == ID_GHOST)
        {
            std::cout << "GHOST_";
            m->setBlock(pos, ' ');
            score += 500;
        }
        else 
        {
            std::cout << "UNK___";
        }

        /*
            Query map for the availability of destination for movation.
                - IN case of a collision, dont move, set colllided flag to 1.
                - Else move there, set collided flag to 0.    
        */

        destBlock = m->queryMap(dest);
        if (destBlock == ID_WALL)
        {
            dest = pos;
            // set collided flag to 1
        }
        else
        {
            // set collided flag to 0
        }

        pos = dest;
        if (++animFrame == 2)
            animFrame = 0;
        image = sprite[dir][animFrame];
    }
    int getScore()
    {
        return score;
    }
};

class Ghost : public Entity
{
    private:
    Direction dir;
    float speed;
    int animFrame;
    char sprite[1][2];

    public:
    Ghost (Vector2D p);
    void update(Map *m);
    
};

// Main Function

int main()
{
    Map map;
    Player player(Vector2D(30, 10));
    std::vector<std::vector<char>> board;

    map.fetchMap("default.map");
    board = map.getMap();

    std::thread inputThread(PollInput);

    while (true)
    {
        usleep(250000);
        player.update(&map);
        board = map.getMap();
        board[player.getPos().y][player.getPos().x] = player.getImage();

        system("clear");
        std::cout << "  [" << map.getMap()[10][30] << "] ";
        std::cout << "SCORE : " << player.getScore() << std::endl;

        for (auto i : board)
        {
            for (auto j : i)
                std::cout << j;
            std::cout << "\n";
        }
        std::cout << std::endl;
        // if (player.getPos().y == 8)
        // {
        //     // stop;
        // }
    }

    return 0;
}

void PollInput()
{
    termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~ICANON;
    new_tio.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    char newchar, oldchar;
    while (true)
    {

        std::cin.get(newchar);
        if (oldchar != newchar)
        {
            keyDown[newchar] = true;
            keyDown[oldchar] = false;
            oldchar = newchar;
        }
    }
}