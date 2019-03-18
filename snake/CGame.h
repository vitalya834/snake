#pragma once
#ifndef __CGAME_H__
#define __CGAME_H__

#include <time.h>
#include <fstream>
#include <utility>

#include "CScreen.h"
#include "CSnake.h"
#include "SCoord.h"

using namespace std;

const int NAMELENGTH = 16;      // размер буфера для имени игрока

// Структура для хранения результата игры

struct SRecord {
	char name[NAMELENGTH];      // имя игрока
	double rating;              // рейтинг
	int length;                 // длина змеи
	double game_time;           // время игры
	time_t date;                // дата и время окончания игры

	SRecord();
	void as_string(char *buffer);   // отформатированная строка результата
};


class CGame {
public:
	CGame(CScreen& _scr, int _width = 80, int _height = 24, int _latency = 100);
	void game_loop();           // основной цикл игры
	void top10(bool after_game); // работа с таблицей 10 лучших результатов
	bool once_more();           // вывод запроса и приём ответа от игрока
	void pak(int y);            // "Press any key for continue..."
	void read_top10();          // чтение из файла таблицы 10 лучших результатов
	void write_top10();         // запись в файл таблицы 10 лучших результатов
	void logo();                // вывод заставки игры
	void goodbye();             // вывод копирайта по окончании игры

private:
	enum Command { CMD_NOCOMMAND = 0, CMD_EXIT, CMD_LEFT, CMD_RIGHT, CMD_UP, CMD_DOWN };
	enum State { STATE_OK, STATE_EXIT, STATE_DIED };

	typedef pair<int, Command> CmdPair;

	int width, height;      
	int latency;            
	CScreen scr;            
	CSnake snake;           
	double duration_game;   
	double rating, rating_i; 

	SRecord ttop10[10];     

	CmdPair cmd_table[5];   

	void draw_field();      
	SCoord make_food();     
	void print_stat();      
	Command get_command();  
	void top10_table();     
};



#endif // __CGAME_H__