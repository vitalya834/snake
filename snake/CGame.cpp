#include "CGame.h"

#include <iostream>
#include <cstring>
#include <conio.h>

const char *recordFormatStr = "%-15s  %9.4f  %4u  %7.2f  %s";

SRecord::SRecord() {
	name[0] = '\0';
	rating = 0.0;
	length = 0;
	game_time = 0;
	date = static_cast<time_t>(0);
}

void SRecord::as_string(char *buffer) {
	sprintf(buffer, recordFormatStr, name, rating, length, game_time, ctime(&date));
}

ostream& operator << (ostream& os, const SRecord& rec) {
	os
		<< rec.rating << ' '
		<< rec.length << ' '
		<< rec.game_time << ' '
		<< rec.date << ' '
		<< rec.name << endl;
	return os;
}

istream& operator >> (istream& is, SRecord& rec) {
	is
		>> rec.rating
		>> rec.length
		>> rec.game_time
		>> rec.date;
	is.ignore(1);
	is.getline(&rec.name[0], 16);
	return is;
}

int rec_compare(const void *_op1, const void *_op2) {
	const SRecord *op1 = reinterpret_cast<const SRecord *>(_op1);
	const SRecord *op2 = reinterpret_cast<const SRecord *>(_op2);
	return static_cast<int>(op2->rating - op1->rating);
}





void clearkeys() {
	while (_kbhit())
		_getch();
}



CGame::CGame(CScreen& _scr, int _width, int _height, int _latency) :
	width(_width), height(_height), latency(_latency), scr(_scr) {

	srand(static_cast<unsigned int>(time(NULL)));

	duration_game = 0;
	rating = 0.0;


	cmd_table[0] = CmdPair(27, CMD_EXIT);  
	cmd_table[1] = CmdPair('K', CMD_LEFT);  
	cmd_table[2] = CmdPair('M', CMD_RIGHT); 
	cmd_table[3] = CmdPair('H', CMD_UP);   
	cmd_table[4] = CmdPair('P', CMD_DOWN);  
}

CGame::Command CGame::get_command() {
	int ch;

	ch = _getch();
	if (ch == 0 || ch == 0xe0) {
		ch = _getch();
	}

	for (int i = 0; i < 5; i++) {
		if (cmd_table[i].first == ch) {
			return cmd_table[i].second;
		}
	}
	return CMD_NOCOMMAND;
}


SCoord CGame::make_food() {
	SCoord food;
	do {
		food.x = rand() % (width - 2) + 1;
		food.y = rand() % (height - 2) + 1;
	} while (snake.into(food));

	return food;
}


const char BORDER = '|=|';    

void CGame::draw_field() {

	scr.cls();

	for (int y = 0; y < height; y++) {
		if (y == 0 || y == height - 1) {
			for (int x = 0; x < width; x++)
				scr.pos(x, y, BORDER);
		}
		else {
			scr.pos(0, y, BORDER);
			scr.pos(width - 1, y, BORDER);
		}
	}
	scr.pos(0, height);
	_cprintf("Длинна: ****  Рейтинг: ****.**** (****.****)  Время: ****.**");
}


void CGame::print_stat() {
	scr.pos(8, height);
	_cprintf("%04u", snake.size());
	scr.pos(22, height);
	_cprintf("%09.4f", rating);
	scr.pos(33, height);
	_cprintf("%09.4f", rating_i);
	scr.pos(51, height);
	_cprintf("%07.2f", duration_game);
}

void CGame::top10_table() {
	scr.cls();
	char buf[80];

	scr.pos_str(width / 2 - 12, 2, "***** Т О П    1 0 *****");
	scr.pos_str(5, 4, "Имя              Рейинг    Length  Время   Дата");


	for (int i = 0; i < 10; i++) {
		ttop10[i].as_string(buf);
		scr.pos_str(5, 5 + i, buf);
	}
}

void CGame::top10(bool after_game) {

	char buf[80];
	char buf_encoded[NAMELENGTH];

	top10_table();     
	time_t date = time(NULL);
	if (after_game) {
		
		scr.pos(5, 16);
		_cprintf(recordFormatStr, "Your result", rating, snake.size(), duration_game, ctime(&date));
	}

	if (rating > ttop10[9].rating) {   
		
		scr.pos_str(5, 20, "Your name: _");
		scr.pos(16, 20);
		cin.getline(&buf[0], NAMELENGTH);
		clearkeys();
		OemToCharBuff(buf, buf_encoded, static_cast<DWORD>(NAMELENGTH));
		
		strcpy_s(ttop10[9].name, buf_encoded);
		ttop10[9].date = date;
		ttop10[9].game_time = duration_game;
		ttop10[9].length = snake.size();
		ttop10[9].rating = rating;
	
		qsort(ttop10, 10, sizeof(SRecord), rec_compare);
	
		top10_table();

	
		write_top10();
	}
}

void CGame::pak(int y) {
	scr.pos_str(width / 2 - 15, y, "Press any key for continue...");
	_getch();
	clearkeys();
}

bool CGame::once_more() {
	scr.pos_str(width / 2 - 12, height - 3, "O n c e    m o r e ?");

	int ch = _getch();
	clearkeys();
	if (ch == 'N' || ch == 'n' || ch == 27)
		return false;
	return true;
}

const char *top10_filename = "snake.dat";   

void CGame::read_top10() {
	ifstream fin(top10_filename);
	if (fin) {
		for (int i = 0; i < 10; i++)
			fin >> ttop10[i];
	}
	fin.close();
}

void CGame::write_top10() {
	ofstream fout(top10_filename);
	if (fout) {
		for (int i = 0; i < 10; i++)
			fout << ttop10[i];
	}
	fout.close();
}

const char *ver_number = "v 1.0";
const char *copyright = "(c) Vit, 2019.";

void CGame::logo() {
	scr.pos_str(width / 2 - 9, 10, "*******GAME*******");
	scr.pos_str(width / 2 - 7, 12, "S  N  A  K  E");
	scr.pos_str(width / 2 - 3, 16, ver_number);
	scr.pos_str(width / 2 - 9, height, copyright);
	pak(22);
}

void CGame::goodbye() {
	scr.cls();
	_cprintf("Oldschool Snake %s\n%s\n", ver_number, copyright);
}


const char FOOD = '*|*';      

void CGame::game_loop() {

	duration_game = 0;
	rating = rating_i = 0.0;

	draw_field();          

	snake.reset(SCoord(width / 2, height / 2));     
													
													
	Command cmd = CMD_NOCOMMAND;
	State stt = STATE_OK;

	SCoord delta(-1, 0);              
	SCoord food = make_food();          
	scr.pos(food.x, food.y, FOOD);      

	snake.draw(scr);                   

	print_stat();                       

	clock_t time1, time2, duration;
	time1 = clock();

	do {

		if (_kbhit())                  
			cmd = get_command();        

		
		switch (cmd) {
		case CMD_LEFT:
			delta = SCoord(-1, 0);
			break;
		case CMD_RIGHT:
			delta = SCoord(1, 0);
			break;
		case CMD_UP:
			delta = SCoord(0, -1);
			break;
		case CMD_DOWN:
			delta = SCoord(0, 1);
			break;
		case CMD_EXIT:
			stt = STATE_EXIT;
		default:
			break;
		};

		SCoord hd = snake.head();      
		hd += delta;                   
		if (hd.x == 0 || hd.x == width - 1 || hd.y == 0 || hd.y == height - 1 || snake.into(hd))
			stt = STATE_DIED;

		if (stt == STATE_OK) {          
			snake.move(delta, scr);    

			if (snake.head() == food) { 
				snake.grow(food, 3);   
				food = make_food();    
				scr.pos(food.x, food.y, FOOD); 

				
				time2 = clock();
				duration = time2 - time1;
				duration_game += static_cast<double>(duration) / CLOCKS_PER_SEC;
				rating_i = static_cast<double>(snake.size()) / duration * CLOCKS_PER_SEC;
				rating += rating_i;    
				time1 = time2;

				print_stat();           
			}

			Sleep(latency);            
		}

	} while (stt == STATE_OK);         

	scr.pos_str(width / 2 - 8, 10, " G a m e    o v e r ");
	clearkeys();
	_getch();
	clearkeys();
}