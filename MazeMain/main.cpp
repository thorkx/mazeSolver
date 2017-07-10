/*
 * main.cpp
 *
 *  Created on: 2017-06-09
 *      Author: etudiant
 */

#include "Maze.h"
using namespace MazeLib;

int main(){

	Maze tmp = Maze("largemaze.txt");
	tmp.solveMaze();
	return 0;
}

