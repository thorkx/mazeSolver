#include <gtest/gtest.h>
#include "Maze.h"

namespace MazeLib {

TEST(MazeCreation, NewMaze5x3Has5ColumnsOK){
	auto tmp = MazeLib::Maze("testCase1.txt");
	ASSERT_EQ(5, tmp.getColumns());
}

TEST(MazeCreation, NewMaze5x3Has3LinesOK){
	auto tmp = MazeLib::Maze("testCase1.txt");
	ASSERT_EQ(3, tmp.getLines());
}

TEST(MazeCreation, testCase1MazeStartIsStartTileTypeOK){
	auto tmp = MazeLib::Maze("testCase1.txt");
	std::cout << tmp.getStart()->tileType;
	ASSERT_EQ(tmp.getStart()->tileType, StartTile);
}

TEST(MazeCreation, testCase1MazeEndIsEndTileTypeOK){
	auto tmp = MazeLib::Maze("testCase1.txt");
	std::cout << tmp.getEnd()->tileType;
	ASSERT_EQ(tmp.getEnd()->tileType, EndTile);
}

} //namespace MazeLib

