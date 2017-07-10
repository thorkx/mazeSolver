/*
 * Maze.h
 *
 *  Created on: 2017-06-08
 *      Author: etudiant
 */

#ifndef MAZE_H_
#define MAZE_H_

#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <queue>

const int MAX_WIDTH = 100;
const int MAX_HEIGHT = 100;

namespace MazeLib {

enum TileType {
	Clear = 10, Wall = 1, OutOfBounds = 0, StartTile = 66, EndTile = 33, Path = 9
};

class Maze {
public:
	Maze(std::string);
	~Maze();

	typedef std::vector<std::string> mazeMap;

	void solveMaze();
	int getLines() const;
	int getColumns() const;

private:
	class Node {
	public:
		Node * next;
		Node * previous;
		bool visited;
		int vectIndex;
		TileType tileType;
		Node(Node * n = 0, Node * p = 0, bool v = false, int i = -1, TileType t = OutOfBounds) : next(n), previous(p), visited(v), vectIndex(i), tileType(t){};

		void assignType(char);

	};

	typedef Node * tile;

	tile startTile;
	tile endTile;

	int nbLines;
	int nbColumns;

	int nbVertices;

	mazeMap map;

	std::array<std::array<tile, MAX_WIDTH>, MAX_HEIGHT> tileArray;
	std::vector<std::tuple<int, int>> verticesVect;
	std::vector<std::list<unsigned int> > listAdj;

	bool isInRange(std::tuple<int, int>, std::tuple<int, int>) const;

	std::vector<unsigned int> BFS(unsigned int) const;

	std::vector<unsigned int> listAdjacentVertices(unsigned int) const;
	void findVertices(std::vector<std::tuple<int, int>>&);
	void findEdges(unsigned int);

	void drawPath();

	void readMap(std::ifstream&, mazeMap &);
	void populateMap(std::array<std::array<tile, MAX_WIDTH>, MAX_HEIGHT> &);
	void _delete();

	std::array<std::array<tile, MAX_WIDTH>, MAX_HEIGHT> generateMap();

public:
	tile getStart() const;
	tile getEnd() const;

};

} //namespace MazeLib
#endif /* MAZE_H_ */
