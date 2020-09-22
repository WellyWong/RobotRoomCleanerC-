class interface
{
	std::vector<int> pos;
	std::vector<std::vector<int>> room;
	int dir = 0;		//up = 0, right = 1, down = 2, left = 3

public:
	interface(std::vector<int> Pos, std::vector<std::vector<int>> Room) : pos(Pos), room(Room) {}

	bool move();
	void turnLeft();
	void turnRight();
	void clean();
};

bool interface::move()
{
	int x = 0, y = 0;

	switch (dir)
	{
	case 0:
		x = -1;
		break;
	case 1:
		y = 1;
		break;
	case 2:
		x = 1;
		break;
	case 3:
		y = -1;
		break;
	}
	int newX = pos[0] + x;
	int newY = pos[1] + y;
	bool outOfBounds = (newX < 0 || newY < 0 || newX > room.size() - 1 || newY > room[0].size() - 1);
	//All grids in the room are marked by either 0 or 1.
	//0 means the cell is blocked, while 1 means the cell is accessible.
	if (outOfBounds || room[newX][newY] == 0)
		return false;

	pos[0] = newX;
	pos[1] = newY;
	std::cout << "Robot at: " << pos[0] << ", " << pos[1] << std::endl;
	return true;
}

void interface::turnLeft()
{
	dir -= 1;
	if (dir < 0)
		dir = 3;
}

void interface::turnRight()
{
	dir += 1;
	if (dir > 3)
		dir = 0;
}

void interface::clean()
{
	std::cout << "Cleaning " << pos[0] << ", " << pos[1] << std::endl;
}

void dfs(interface& Robot, int i, int j, int currDir, std::set<std::pair<int, int>>& visited)
{
	if (visited.count(std::make_pair(i,j)))
		return;

	visited.insert(std::make_pair(i, j));
	Robot.clean();

	for (int n = 0; n < 4; ++n)		//move in 4 directions
	{
		if (Robot.move())
		{
			int row = i, col = j;
			switch (currDir)
			{
				case 0: row -= 1; break;
				case 1: col += 1; break;
				case 2: row += 1; break;
				case 3: col -= 1; break;
			}
			//visit next branch
			dfs(Robot, row, col, currDir, visited);

			//go back to original place, because dfs, return from recursion does not make the robot move back
			Robot.turnLeft();
			Robot.turnLeft();
			Robot.move();

			//redirect to currDir
			Robot.turnRight();
			Robot.turnRight();
		}
		Robot.turnRight();
		currDir += 1;
		currDir %= 4;
	}
}

void cleanRoom(interface& Robot)
{
	std::set<std::pair<int, int>> visited;
	dfs(Robot, 1, 1, 0, visited);
}

int main()
{
	std::vector<std::vector<int>> Room = { {1,1,1,1,1,0,1,1}, {1,1,1,1,1,0,1,1}, {1,0,1,1,1,1,1,1}, {0,0,0,1,0,0,0,0}, {1,1,1,1,1,1,1,1} };
	std::vector<int> Pos = { 1,1 };

	interface Robot(Pos, Room);

	cleanRoom(Robot);
  return 0;
}
