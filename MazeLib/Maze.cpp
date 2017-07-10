#include "Maze.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>

namespace MazeLib {

typedef std::vector<std::string> mazeMap;

Maze::Maze(std::string name) {
	std::string mapName = name;
	std::ifstream inputFile;

	inputFile.open(mapName, std::ios::in);
	if (inputFile.is_open()) {
		readMap(inputFile, map);
		// TODO Exception check dimenstions
		populateMap(tileArray);
	} else {
		// TODO Exception Error loading map
		std::cout << "!!! Error loading map !!!" << std::endl;
		;
	}

	for (auto line : map)
		std::cout << line << std::endl;
	inputFile.close();
}

Maze::~Maze() {
	///TODO precondition map != 0;
	//_delete();
}

void MazeLib::Maze::solveMaze() {
	findVertices(verticesVect);

	/*
	// Test findVerticesOK
	auto tmpS = *verticesVect.begin();
	auto startTmp = tileArray[std::get<0>(tmpS)][std::get<1>(tmpS)];
	std::cout << (startTmp->tileType == StartTile) << " " << true << std::endl;
	auto tmpE = *(--verticesVect.end());
	auto endTmp = tileArray[std::get<0>(tmpE)][std::get<1>(tmpE)];
	std::cout << (endTmp->tileType == EndTile) << " " << true << std::endl;
	*/
	/*
	 * (for (auto itr = verticesVect.begin() ; itr != verticesVect.end(); itr++){
	 auto tmp = *itr;
	 //std::cout << tileArray[std::get<0>(tmp)][std::get<1>(tmp)]->tileType;
	 }
	 */

	for (size_t i = 0; i < verticesVect.size(); i++)
		findEdges(i);

	/* Test findEdgesOK
	 std::cout << listAdj[0].size() << "0" << std::endl;
	 for (auto voisin : listAdj[0]) std::cout << voisin << " " << std::get<0>(verticesVect[voisin]) << std::get<1>(verticesVect[voisin]) << std::endl;
	 std::cout << listAdj[9].size() << "9" << std::endl;
	 for (auto voisin : listAdj[9]) std::cout << voisin << " " << std::get<0>(verticesVect[voisin]) << std::get<1>(verticesVect[voisin]) << std::endl;
	 std::cout << listAdj[10].size() << "10" << std::endl;
	 for (auto voisin : listAdj[10]) std::cout << voisin << " " << std::get<0>(verticesVect[voisin]) << std::get<1>(verticesVect[voisin]) << std::endl;
	 */

	std::vector<unsigned int> solutionPath = BFS(0);
	/*for (auto v : solutionPath) {
		std::cout << std::get<0>(verticesVect[v]) << "-" << std::get<1>(verticesVect[v]) << std::endl;
	}*/

	drawPath();

	/*
	std::cout << std::get<0>(verticesVect[*(--solutionPath.end())])
			<< std::get<1>(verticesVect[*(--solutionPath.end())]);
	std::cout << solutionPath.size();
	*/
}

Maze::tile MazeLib::Maze::getStart() const {
	// TODO startTile existe;
	return startTile;
}

Maze::tile MazeLib::Maze::getEnd() const {
	// TODO endTile existe;
	return endTile;
}

int MazeLib::Maze::getLines() const {
	// TODO precondition map != 0;
	return nbLines;
}

int MazeLib::Maze::getColumns() const {
	// TODO precondition map != 0;
	return nbColumns;
}

void Maze::readMap(std::ifstream& mapStream, MazeLib::mazeMap & map) {

	std::string line;
	int counterLines = 0;
	int counterColumns = 0;

	while (mapStream.good()) {
		std::getline(mapStream, line);
		if (counterColumns == 0)
			counterColumns = line.length();
		map.push_back(line);
		counterLines++;
		// TODO Lancer exception si longueur ligne pas standard
		// TODO Exception si pas de début (s) ou fin (e)
	}
	nbLines = counterLines;
	nbColumns = counterColumns;

	// TODO exception si nbLines ou nbColumns < 3
	std::cout << "Map loaded : " << nbColumns << "x" << nbLines << std::endl;
}

void MazeLib::Maze::populateMap(
		std::array<std::array<tile, MAX_WIDTH>, MAX_HEIGHT> & grid) {
	//PRECONDITION Map a été créée
	for (int i = 0; i < nbLines; i++) {
		for (int j = 0; j < nbColumns; j++) {
			grid[i][j] = (new Node());
			grid[i][j]->assignType(map[i][j]);

			if (grid[i][j]->tileType == StartTile) {
				startTile = grid[i][j];
			}

			else if (grid[i][j]->tileType == EndTile) {
				endTile = grid[i][j];
			}
		}
	}
}

void MazeLib::Maze::findVertices(
		std::vector<std::tuple<int, int>>& listVertices) {
	for (int i = 0; i < nbLines; i++) {
		for (int j = 0; j < nbColumns; j++) {
			if (tileArray[i][j]->tileType == StartTile) {
				listVertices.push_back(std::tuple<int, int>(i, j));
				tileArray[i][j]->vectIndex = listVertices.size();
			}
		}
	}
	for (int i = 0; i < nbLines; i++) {
		for (auto j = 0; j < nbColumns; j++) {
			if (tileArray[i][j]->tileType == Clear) {
				listVertices.push_back(std::tuple<int, int>(i, j));
				tileArray[i][j]->vectIndex = listVertices.size();
			}
		}
	}
	for (int i = 0; i < nbLines; i++) {
		for (int j = 0; j < nbColumns; j++) {
			if (tileArray[i][j]->tileType == EndTile) {
				listVertices.push_back(std::tuple<int, int>(i, j));
				tileArray[i][j]->vectIndex = listVertices.size();
			}
		}
	}
	nbVertices = listVertices.size();
}

/*
 * \brief Assign a tiletype to a node
 *
 * \param[in] tileChar, a character representing the tiletype (see enum TileType)
 * \param[out] Node.tileType is assigned
 */
void MazeLib::Maze::Node::assignType(char tileChar) {
	TileType returnType;
	if (tileChar == '#')
		returnType = Wall;
	else if (tileChar == ' ')
		returnType = Clear;
	else if (tileChar == 's')
		returnType = StartTile;
	else if (tileChar == 'e')
		returnType = EndTile;
	else
		returnType = OutOfBounds;
	this->tileType = returnType;
}

void MazeLib::Maze::findEdges(unsigned int verticeIndex) {
	auto vertice = verticesVect[verticeIndex];
	auto indexCount = 0;
	auto list = new std::list<unsigned int>;
	listAdj.push_back(*list);
	for (auto itr = verticesVect.begin(); itr != verticesVect.end(); itr++) {
		auto tmp = *itr;
		if (isInRange(vertice, tmp)
				and ((tileArray[std::get<0>(tmp)][std::get<1>(tmp)]->tileType
						== Clear)
						or (tileArray[std::get<0>(tmp)][std::get<1>(tmp)]->tileType
								== EndTile))) {
			listAdj[verticeIndex].push_back(indexCount);
		}
		indexCount++;

	}
}

std::vector<unsigned int> MazeLib::Maze::listAdjacentVertices(
		unsigned int vertice) const {
	//PRECONDITION(sommet < m_nbSommets);
	std::list<unsigned int> liste = listAdj[vertice];
	std::vector<unsigned int> vectVertices;
	for (auto itr = liste.begin(); itr != liste.end(); ++itr) {
		vectVertices.push_back(*itr);
	}
	return vectVertices;
}

void MazeLib::Maze::_delete() {
	for (auto i = 0 ; i < nbLines ; i++){
		for (auto j = 0 ; j < nbColumns ; j++) {
			delete tileArray[i][j];
		}
	}
}

/*
 * \brief Check if vertice is within one node of target vertice
 * \param[in] coorIn, tuple<int, int> representing the first vertice's coordinates
 * \param[in] coorOut, tuple<int, int> representing the second vertice's coordinates
 *
 * \return true if within one node
 */
bool MazeLib::Maze::isInRange(std::tuple<int, int> coorIn,
		std::tuple<int, int> coorOut) const {
	//TODO PRECONDITION(COORDINATES WITHIN MAP
	if (((std::get<0>(coorOut) == std::get<0>(coorIn) + 1)
			and (std::get<1>(coorOut) == std::get<1>(coorIn)))
			or ((std::get<0>(coorOut) == std::get<0>(coorIn) - 1)
					and (std::get<1>(coorOut) == std::get<1>(coorIn)))
			or ((std::get<0>(coorOut) == std::get<0>(coorIn))
					and (std::get<1>(coorOut) == std::get<1>(coorIn) + 1))
			or ((std::get<0>(coorOut) == std::get<0>(coorIn))
					and (std::get<1>(coorOut) == std::get<1>(coorIn) - 1))) {
		return true;
	} else
		return false;
}

std::vector<unsigned int> MazeLib::Maze::BFS(unsigned int vertice) const {
	std::queue<unsigned int> verticeQueue;
	std::vector<bool> visitedVertices(nbVertices, false);
	std::vector<unsigned int> path;

	visitedVertices[vertice] = true;
	verticeQueue.push(vertice);

	while (!verticeQueue.empty()) {
		auto nextV = verticeQueue.front();
		verticeQueue.pop();
		path.push_back(nextV);

		auto neighbours = listAdjacentVertices(nextV);
		for (auto neighbour : neighbours) {
			if (!visitedVertices[neighbour]) {
				tileArray[std::get<0>(verticesVect[neighbour])][std::get<1>(verticesVect[neighbour])]->previous = tileArray[std::get<0>(verticesVect[nextV])][std::get<1>(verticesVect[nextV])];
				verticeQueue.push(neighbour);
				visitedVertices[neighbour] = true;
			}
		}
	}
	return path;
}

void MazeLib::Maze::drawPath() {
	auto driver = endTile->previous;

	while (driver != startTile) {
		driver->tileType = Path;
		driver = driver->previous;
		//TODO exception si pas de
	}

	std::cout << "Solved maze" << std::endl;
	for (auto i = 0 ; i < nbLines ; i++){
		for (auto j = 0 ; j < nbColumns ; j++) {
			if (tileArray[i][j]->tileType == Path) {
				std::cout << "*";
			}
			else if (tileArray[i][j]->tileType == Clear) {
				std::cout << " ";
			}
			else if (tileArray[i][j]->tileType == Wall) {
				std::cout << "#";
			}
			else if (tileArray[i][j]->tileType == StartTile) {
				std::cout << "s";
			}
			else if (tileArray[i][j]->tileType == EndTile) {
				std::cout << "e";
			}
			else {
				//TODO throw exception
			}

		}
		std::cout << std::endl;
	}
}
std::array<std::array<MazeLib::Maze::tile, MAX_WIDTH>, MAX_HEIGHT> MazeLib::Maze::generateMap() {
	std::srand(std::time(NULL));

	std::array<std::array<MazeLib::Maze::tile, MAX_WIDTH>, MAX_HEIGHT> tmpMapArray;
	for (size_t i = 0 ; i < tmpMapArray.size() ; i++) {
		for (size_t j = 0 ; j < tmpMapArray[i].size() ; j++) {
			tmpMapArray[i][j] = new Node(0, 0, false, -1, OutOfBounds);
		}
	}

	for (size_t i = 1 ; i < tmpMapArray.size() ; i++) {
		for (size_t j = 1 ; j < tmpMapArray[i].size() ; j++) {
			tmpMapArray[i][j]->tileType = Wall;
		}
	}

	unsigned int currenti;
	unsigned int currentj;

	auto startBorder = std::rand() % 4;
	switch(startBorder) {
		case(0) : {
			auto startTileIndex = 1 + std::rand() % (tmpMapArray[0].size() - 1);
			tmpMapArray[0][startTileIndex]->tileType = StartTile;
			startTile = tmpMapArray[0][startTileIndex];
			currenti = 0;
			currentj = startTileIndex;
			break;
		}
		case(1) : {
			auto startTileIndex = 1 + std::rand() % (tmpMapArray[0].size() - 1);
			tmpMapArray[tmpMapArray.size() - 1][startTileIndex]->tileType = StartTile;
			startTile = tmpMapArray[tmpMapArray.size() - 1][startTileIndex];
			currenti = tmpMapArray.size() - 1;
			currentj = startTileIndex;
			break;
		}
		case(2) : {
			auto startTileIndex = 1 + std::rand() % (tmpMapArray.size() - 1);
			tmpMapArray[startTileIndex][0]->tileType = StartTile;
			startTile = tmpMapArray[startTileIndex][0];
			currenti = startTileIndex;
			currentj = 0;
			break;
		}
		case(3) : {
			auto startTileIndex = 1 + std::rand() % (tmpMapArray.size() - 1);
			tmpMapArray[startTileIndex][tmpMapArray[0].size() - 1]->tileType = StartTile;
			startTile = tmpMapArray[startTileIndex][tmpMapArray[0].size() - 1];
			currenti = startTileIndex;
			currentj = tmpMapArray[0].size() - 1;
			break;
		}
	}

	std::queue<Node> nodeQueue;
	nodeQueue.push(startTile);

	while (!nodeQueue.empty()) {


	}


	return tmpMapArray;
}

} //namespace

