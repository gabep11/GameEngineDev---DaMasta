#pragma once
#include <string>

class Game
{

public:
	Game()
	{
		gameTitle = "DaGameTitle";
	}

	void initGame();

private:

	std::string gameTitle;

};