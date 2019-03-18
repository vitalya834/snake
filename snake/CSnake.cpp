#include "CSnake.h"

const char SNAKE_TAIL = '0';        


CSnake::CSnake() {
	head_mark = '<';
}

void CSnake::reset(SCoord start_pos) {
	worm.clear();
	worm.reserve(1000);         
	worm.push_back(start_pos);  
	worm.push_back(start_pos);  
	worm[0].x++;               
}

void CSnake::draw(CScreen& scr) {
	unsigned int wsize = worm.size() - 1;
	for (unsigned int i = 0; i < wsize; i++)
		scr.pos(worm[i].x, worm[i].y, SNAKE_TAIL);
	scr.pos(worm[wsize].x, worm[wsize].y, head_mark);
	drawn = worm.size();
}

void CSnake::move(const SCoord& delta, CScreen& scr) {
	if (drawn == worm.size())
		scr.pos(worm[0].x, worm[0].y, ' ');
	else
		drawn++;


	for (unsigned int i = 1; i < worm.size(); i++)
		worm[i - 1] = worm[i];

	worm[worm.size() - 1] += delta;       


	if (delta.x < 0)
		head_mark = '<';
	else if (delta.x > 0)
		head_mark = '>';
	else if (delta.y < 0)
		head_mark = 'A';
	else // (delta.y > 0)
		head_mark = 'V';

	scr.pos(worm[worm.size() - 1].x, worm[worm.size() - 1].y, head_mark);
	scr.pos(worm[worm.size() - 2].x, worm[worm.size() - 2].y, SNAKE_TAIL);
}

void CSnake::grow(const SCoord& pos, int growbits) {
	for (int i = 0; i < growbits; ++i)
		worm.insert(worm.begin(), pos);
}

bool CSnake::into(const SCoord& pos) {
	for (unsigned int i = 0; i < worm.size(); i++)
		if (worm[i].x == pos.x && worm[i].y == pos.y)
			return true;
	return false;
}

SCoord CSnake::head() {
	return worm[worm.size() - 1];
}

int CSnake::size() {
	return static_cast<int>(worm.size());
}