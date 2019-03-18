#pragma once
#ifndef __CSNAKE_H__
#define __CSNAKE_H__

#include <vector>

#include "SCoord.h"
#include "CScreen.h"

using namespace std;

typedef vector<SCoord> CoordVector;

class CSnake {
public:
	CSnake();
	void reset(SCoord start_pos);                   
	void draw(CScreen& scr);                        
	void move(const SCoord& delta, CScreen& scr);  
	void grow(const SCoord& pos, int growbits);     
	bool into(const SCoord& pos);                  
	SCoord head();                                 
	int size();                                     

private:
	CoordVector worm;      
	char head_mark;         
	unsigned int drawn;     
};

#endif // __CSNAKE_H__