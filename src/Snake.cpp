#include "Snake.hpp"

Snake::BodyPart::BodyPart(const uint32_t& xCoordinate, const uint32_t& yCoordinate) {
	x = xCoordinate;
	y = yCoordinate;
}

Snake::BodyPart::BodyPart() {
	x = 0;
	y = 0;
}

Snake::Snake() {
	initscr();
	cbreak();
	keypad(stdscr, true);
	curs_set(false);
	noecho();
	getmaxyx(stdscr, maxHeight, maxWidth);
	mvprintw((maxHeight - 4) / 2, (maxWidth - 11) / 2, "%s", "DIRECTIONS:");
	mvprintw(((maxHeight - 4) / 2) + 1, (maxWidth - 23) / 2, "%s", "Press ANY KEY to start.");
	mvprintw(((maxHeight - 4) / 2) + 2, (maxWidth - 57) / 2, "%s", "Use LEFT, RIGHT, UP, DOWN arrow keys to control movement.");
	mvprintw(((maxHeight - 4) / 2) + 3, (maxWidth - 16) / 2, "%s", "Use ESC to quit.");
	refresh();
	getch();
	clear();
	refresh();
	nodelay(stdscr, true);

	uint32_t seed = (uint32_t)std::chrono::system_clock::now().time_since_epoch().count();
	randomGenerator.seed(seed);

	snakeBodyChar = 'X';
	outerEdgeChar = '#';
	foodChar = '$';
	currentDirection = KEY_LEFT;
	delayTime = 100000;
	points = 0;

	generateFood();
	for (uint32_t i = 0; i < 5; ++i) {
		snake.push_back(BodyPart((maxWidth / 2) + i, maxHeight / 2));
	}


	for (uint32_t j = 0; j < 2; ++j) {
		for (uint32_t i = 0; i < maxWidth; ++i) {
			j == 0 ? mvaddch(0, i, outerEdgeChar) : mvaddch(maxHeight - 2, i, outerEdgeChar);
		}
	}

	for (uint32_t j = 0; j < 2; ++j) {
		for (uint32_t i = 0; i < maxHeight - 2; ++i) {
			j == 0 ? mvaddch(i, 0, outerEdgeChar) : mvaddch(i, maxWidth-1, outerEdgeChar);
		}
	}

	for (BodyPart& bodyPart : snake) {
		mvaddch(bodyPart.y, bodyPart.x, snakeBodyChar);
	}

	
	mvprintw(maxHeight - 1, 0, "%d", points);
	refresh();
}

Snake::~Snake() {
	nodelay(stdscr, false);
	getch();
	attron(A_NORMAL);
	delwin(stdscr);
}

void Snake::start() {
	uint32_t inputDirection;
	while ((inputDirection = getch()) != KEY_ESC && !collision()) {
		if (inputDirection == KEY_LEFT && currentDirection != KEY_RIGHT)
			currentDirection = inputDirection;
		else if (inputDirection == KEY_RIGHT && currentDirection != KEY_LEFT)
			currentDirection = inputDirection;
		else if (inputDirection == KEY_UP && currentDirection != KEY_DOWN)
			currentDirection = inputDirection;
		else if(inputDirection == KEY_DOWN && currentDirection != KEY_UP)
			currentDirection = inputDirection;

		std::this_thread::sleep_for(std::chrono::microseconds(delayTime));
		moveSnake();
		if (gotFood()) {
			consumeFood();
			generateFood();
		}
		
	}
	mvaddstr(maxHeight / 2, (maxWidth - 10) / 2, "Game Over!");
	refresh();
}

void Snake::moveSnake() {
	switch(currentDirection) {
		case KEY_LEFT:
			mvaddch(std::prev(snake.end()) -> y, std::prev(snake.end()) -> x, KEY_SPACE);
			snake.pop_back();
			snake.push_front(BodyPart((snake.begin() -> x) - 1, snake.begin() -> y));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			break;

		case KEY_RIGHT:
			mvaddch(std::prev(snake.end()) -> y, std::prev(snake.end()) -> x, KEY_SPACE);
			snake.pop_back();
			snake.push_front(BodyPart((snake.begin() -> x) + 1, snake.begin() -> y));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			break;

		case KEY_UP:
			mvaddch(std::prev(snake.end()) -> y, std::prev(snake.end()) -> x, KEY_SPACE);
			snake.pop_back();
			snake.push_front(BodyPart(snake.begin() -> x, (snake.begin() -> y) - 1));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			break;

		case KEY_DOWN:
			mvaddch(std::prev(snake.end()) -> y, std::prev(snake.end()) -> x, KEY_SPACE);
			snake.pop_back();
			snake.push_front(BodyPart(snake.begin() -> x, (snake.begin() -> y) + 1));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			break;

		default:
			break;
	}
	refresh();
}

void Snake::generateFood() {
	bool validArea = false;
	while (!validArea) {
		uint32_t x = randomGenerator() % (maxWidth - 1) + 1;
		uint32_t y = randomGenerator() % (maxHeight - 3) + 1;

		bool notInsideSnake = true;

		for (BodyPart& bodyPart : snake) {
			if (bodyPart.y == y && bodyPart.x == x)
				notInsideSnake = false;
		}

		if (notInsideSnake) {
			food.x = x;
			food.y = y;
			validArea = true;
		}
	}
	mvaddch(food.y, food.x, foodChar);
	refresh();
}

void Snake::consumeFood() {
	switch(currentDirection) {
		case KEY_LEFT:
			snake.push_front(BodyPart((snake.begin() -> x) - 1, snake.begin() -> y));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			points += 10;
			if ((points % 100) == 0) {
				delayTime -= 20000;
			}
			mvprintw(maxHeight-1, 0, "%d", points);
			break;
		case KEY_RIGHT:
			snake.push_front(BodyPart((snake.begin() -> x) + 1, snake.begin() -> y));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			points += 10;
			if ((points % 100) == 0) {
				delayTime -= 20000;
			}
			mvprintw(maxHeight - 1, 0, "%d", points);
			break;

		case KEY_UP:
			snake.push_front(BodyPart(snake.begin() -> x, (snake.begin() -> y) - 1));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			points += 10;
			if ((points % 100) == 0) {
				delayTime -= 20000;
			}
			mvprintw(maxHeight - 1, 0, "%d", points);
			break;

		case KEY_DOWN:
			snake.push_front(BodyPart(snake.begin() -> x, (snake.begin() -> y) + 1));
			mvaddch(snake.begin() -> y, snake.begin() -> x, snakeBodyChar);
			points += 10;
			if ((points % 100) == 0) {
				delayTime -= 20000;
			}
			mvprintw(maxHeight - 1, 0, "%d", points);
			break;
		default:
			break;
	}
	refresh();
}

bool Snake::collision() {
	if (snake.begin() -> x == 0 ||
		snake.begin() -> x == maxWidth - 1 ||
		snake.begin() -> y == 0 ||
		snake.begin() -> y == maxHeight - 2)
		return true;

	for (auto it = std::next(snake.begin(), 2); it != snake.end(); ++it)
		if (snake.begin() -> x == it -> x && snake.begin() -> y == it -> y)
			return true;

	return false;
}

bool Snake::gotFood() {
	if (snake.begin() -> x == food.x && snake.begin() -> y == food.y)
		return true;
	return false;
}