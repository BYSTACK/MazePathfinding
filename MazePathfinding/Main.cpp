#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Cell.h"

sf::Image input;

std::vector<std::vector<Cell*>*> cells;

std::vector<sf::Vector2i> activeCells;
std::vector<sf::Vector2i> nextActiveCells;
std::string inputFileName;

bool showAnim = false;
int startX, startY, endX, endY;
int picSizeX;
int picSizeY;

void getUserInput()
{
	std::cout << "Поиск пути в лабиринте" << std::endl;
	std::cout << "Введите имя файла с лабиринтом: ";

	std::cin >> inputFileName;

	input.loadFromFile(inputFileName + ".png");
	std::cout << "\nВведите координаты начала лабиринта: ";


	std::cin >> startX;
	std::cin >> startY;

	std::cout << "\nВведите координаты конца лабиринта: ";
	std::cin >> endX;
	std::cin >> endY;



	std::cout << "\nПоказать анимацию? (y/n): ";
	std::string din;
	std::cin >> din;

	if (din == "y")
	{
		showAnim = true;
	}
}

void scanPicture()
{
	for (unsigned int i = 0; i < input.getSize().x; i++) //Преобразование изображения в двумерный вектор клеток
	{
		std::vector<Cell*>* col = new std::vector<Cell*>;

		for (unsigned int j = 0; j < input.getSize().y; j++)
		{
			Cell* cell = new Cell;
			if (input.getPixel(i, j).r == 0 && input.getPixel(i, j).g == 0 && input.getPixel(i, j).b == 0)
			{
				cell->isSolid = true;
			}
			else
			{
				cell->isSolid = false;
			}
			col->push_back(cell);
		}
		cells.push_back(col);
	}

	cells[startX]->at(startY)->isVisited = true; //Делаем стартовую клетку посещенной и ее расстояние до старт. клетки ставим 0
	cells[startX]->at(startY)->newIsVisited = true;
	cells[startX]->at(startY)->dist = 0;

	activeCells.push_back(sf::Vector2i(startX, startY));

	picSizeX = input.getSize().x;
	picSizeY = input.getSize().y;
}

bool calcFloodStep()
{/*
	for (int i = 0; i < picSizeX; i++)
	{
		for (int j = 0; j < picSizeY; j++)
		{
			if (cells[i]->at(j)->isVisited)
			{
				for (int k = -1; k < 2; k += 2) //Левая и правая клетки
				{
					if (!(k == -1 && i == 0) && !(k == 1 && i == picSizeX - 1)) //Проверка не находится ли клетка вне двумерного вектора
						if (!(cells[i + k]->at(j)->isVisited) && !(cells[i + k]->at(j)->isSolid))
						{
							cells[i + k]->at(j)->newIsVisited = true;
							int dist = cells[i]->at(j)->dist + 1;
							input.setPixel(i + k, j, sf::Color(255, 255 - dist % 256, 255 - dist % 256));
							cells[i + k]->at(j)->dist = dist;
						}

				}
				for (int l = -1; l < 2; l += 2) //Верхняя и нижняя клетки
				{
					if (!(l == -1 && j == 0) && !(l == 1 && j == picSizeY - 1)) //Проверка не находится ли клетка вне двумерного вектора
						if (!(cells[i]->at(j + l)->isVisited) && !(cells[i]->at(j + l)->isSolid))
						{
							cells[i]->at(j + l)->newIsVisited = true;
							int dist = cells[i]->at(j)->dist + 1;
							input.setPixel(i, j + l, sf::Color(255, 255 - dist % 256, 255 - dist % 256));
							cells[i]->at(j + l)->dist = dist;
						}
				}
			}
		}
	}*/

	for (int i = 0; i < activeCells.size(); i++)
	{
		int x = activeCells.at(i).x;
		int y = activeCells.at(i).y;

		for (int k = -1; k < 2; k += 2) //Левая и правая клетки
		{
			if (!(k == -1 && x == 0) && !(k == 1 && x == picSizeX - 1)) //Проверка не находится ли клетка вне двумерного вектора
				if (!(cells[x + k]->at(y)->isVisited) && !(cells[x + k]->at(y)->newIsVisited) && !(cells[x + k]->at(y)->isSolid))
				{
					cells[x + k]->at(y)->newIsVisited = true;
					nextActiveCells.push_back(sf::Vector2i(x + k, y));

					int dist = cells[x]->at(y)->dist + 1;
					input.setPixel(x + k, y, sf::Color(255, 255 - dist % 256, 255 - dist % 256));
					cells[x + k]->at(y)->dist = dist;
				}

		}
		for (int l = -1; l < 2; l += 2) //Верхняя и нижняя клетки
		{
			if (!(l == -1 && y == 0) && !(l == 1 && y == picSizeY - 1)) //Проверка не находится ли клетка вне двумерного вектора
				if (!(cells[x]->at(y + l)->isVisited) && !(cells[x]->at(y + l)->newIsVisited) && !(cells[x]->at(y + l)->isSolid))
				{
					cells[x]->at(y + l)->newIsVisited = true;
					nextActiveCells.push_back(sf::Vector2i(x, y + l));

					int dist = cells[x]->at(y)->dist + 1;
					input.setPixel(x, y + l, sf::Color(255, 255 - dist % 256, 255 - dist % 256));
					cells[x]->at(y + l)->dist = dist;
				}
		}
	}

	for (int i = 0; i < nextActiveCells.size(); i++)
	{
		int x = nextActiveCells[i].x;
		int y = nextActiveCells[i].y;
		cells[x]->at(y)->isVisited = cells[x]->at(y)->newIsVisited;
	}

	//activeCells.clear();
	activeCells = nextActiveCells;
	nextActiveCells.clear();

	if (cells[endX]->at(endY)->isVisited) //Если конечная клетка пути посещена, делаем ее частью пути и раскрашиваем
	{
		cells[endX]->at(endY)->partOfPath = true;
		cells[endX]->at(endY)->newPartOfPath = true;
		input.setPixel(endX, endY, sf::Color(0, 0, 255));
		activeCells.clear();
		activeCells.push_back(sf::Vector2i(endX, endY));
		return true;
	}
	else
	{
		return false;
	}
}

bool calcPathStep()
{
	/*
	for (int i = 0; i < picSizeX; i++)
	{
		for (int j = 0; j < picSizeY; j++)
		{
			if (cells[i]->at(j)->partOfPath)
			{
				for (int k = -1; k < 2; k += 2)
				{
					if (!(k == -1 && i == 0) && !(k == 1 && i == picSizeX - 1))
						if (!(cells[i + k]->at(j)->partOfPath) && !(cells[i + k]->at(j)->isSolid) && (cells[i + k]->at(j)->dist < cells[i]->at(j)->dist))
						{
							cells[i + k]->at(j)->newPartOfPath = true;
							input.setPixel(i + k, j, sf::Color(0, 0, 255));
						}

				}
				for (int l = -1; l < 2; l += 2)
				{
					if (!(l == -1 && j == 0) && !(l == 1 && j == picSizeY - 1))
						if (!(cells[i]->at(j + l)->partOfPath) && !(cells[i]->at(j + l)->isSolid) && (cells[i]->at(j + l)->dist < cells[i]->at(j)->dist))
						{
							cells[i]->at(j + l)->newPartOfPath = true;
							input.setPixel(i, j + l, sf::Color(0, 0, 255));
						}
				}
			}
		}
	}
	*/

	for (int i = 0; i < activeCells.size(); i++)
	{
		int x = activeCells[i].x;
		int y = activeCells[i].y;

		for (int k = -1; k < 2; k += 2)
		{
			if (!(k == -1 && x == 0) && !(k == 1 && x == picSizeX - 1))
				if (!(cells[x + k]->at(y)->partOfPath) && !(cells[x + k]->at(y)->newPartOfPath) && !(cells[x + k]->at(y)->isSolid) && (cells[x + k]->at(y)->dist < cells[x]->at(y)->dist))
				{
					cells[x + k]->at(y)->newPartOfPath = true;
					nextActiveCells.push_back(sf::Vector2i(x + k, y));
					input.setPixel(x + k, y, sf::Color(0, 0, 255));
				}

		}
		for (int l = -1; l < 2; l += 2)
		{
			if (!(l == -1 && y == 0) && !(l == 1 && y == picSizeY - 1))
				if (!(cells[x]->at(y + l)->partOfPath) && !(cells[x]->at(y + l)->newPartOfPath) && !(cells[x]->at(y + l)->isSolid) && (cells[x]->at(y + l)->dist < cells[x]->at(y)->dist))
				{
					cells[x]->at(y + l)->newPartOfPath = true;
					nextActiveCells.push_back(sf::Vector2i(x, y+l));
					input.setPixel(x, y + l, sf::Color(0, 0, 255));
				}
		}
	}

	for (int i = 0; i < nextActiveCells.size(); i++)
	{
		int x = nextActiveCells[i].x;
		int y = nextActiveCells[i].y;
		
		cells[x]->at(y)->partOfPath = cells[x]->at(y)->newPartOfPath;
	}

	activeCells = nextActiveCells;

	nextActiveCells.clear();

	return cells[startX]->at(startY)->partOfPath;

}

int main()
{
	setlocale(LC_ALL, "Russian");

	getUserInput();
	scanPicture();


	bool isFloodDone = false;
	bool isPathDone = false;

	if (showAnim)
	{
		sf::RenderWindow window(sf::VideoMode(picSizeX, picSizeY), "Pathfinding");
		window.setFramerateLimit(60);

		sf::Texture text;
		text.loadFromImage(input);

		sf::Sprite spr;
		spr.setTexture(text);



		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}


			if (!isFloodDone)
			{
				isFloodDone = calcFloodStep();
			}
			else if (!isPathDone)
			{
				isPathDone = calcPathStep();
			}



			text.loadFromImage(input);
			//spr.setTexture(text);

			window.clear();
			window.draw(spr);
			window.display();
		}
	}
	else
	{
		while (!isFloodDone)
		{
			isFloodDone = calcFloodStep();
		}

		while (!isPathDone) {
			isPathDone = calcPathStep();
		}
	}

	input.saveToFile(inputFileName + "-solved.png");

	system("pause");
	return 0;
}