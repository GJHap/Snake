#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <chrono>
#include <iterator>
#include <list>
#include <random>
#include <thread>
#include <cstdint>
#include <curses.h>

#define KEY_SPACE 32
#define KEY_ESC 27

class Snake {
	private:
		struct BodyPart {
			uint32_t x;
			uint32_t y;
			BodyPart(const uint32_t&, const uint32_t&);
			BodyPart();
		};

		char snakeBodyChar;
		char outerEdgeChar;
		char foodChar;

		uint32_t currentDirection;
		uint32_t maxHeight;
		uint32_t maxWidth;
		uint32_t delayTime;
		uint32_t points;

		BodyPart food;
		std::list<BodyPart> snake;

		std::default_random_engine randomGenerator;

		void moveSnake();
		void generateFood();
		void consumeFood();
		bool collision();
		bool gotFood();
		
	public:
		Snake();
		~Snake();
		void start();

};

#endif