#pragma once
#define _CRT_SECURE_NO_WARNINGS 1 

//System includes
#include <stack>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

//SFML includes
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <fstream>

//Random byte generator
std::random_device engine;

//Consts
const int screen_width = 64;
const int screen_height = 32;
const int cpu_speed = 60; //60hz

//Memory
std::uint8_t memory[4096];
std::uint16_t stack[16];
std::uint8_t V[16];
std::uint16_t I;

//Registers
std::uint8_t DT;
std::uint8_t ST;

//Pointers
std::uint16_t PC;
std::uint16_t SP;

//Screen
sf::RenderTexture* renderer = new sf::RenderTexture();
std::uint8_t screen[screen_width * screen_height];

//Keyboard
std::uint8_t keyboard[16];
enum key
{
	key_zero = 0x0,
	key_one = 0x1,
	key_two = 0x2,
	key_three = 0x3,
	key_four = 0x4,
	key_five = 0x5,
	key_six = 0x6,
	key_seven = 0x7,
	key_eight = 0x8,
	key_nine = 0x9,
	key_c = 0xC,
	key_d = 0xD,
	key_e = 0xE,
	key_f = 0xF,
	key_a = 0xA,
	key_b = 0xB
};

//Fontset
std::uint8_t font_set[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

sf::Sprite* drawScreen()
{
	renderer->create(screen_width * 16, screen_height * 16);
	renderer->clear(sf::Color::Red);

	for(int i = 0; i < screen_width; ++i)
	{
		for (int j = 0; j < screen_height; ++j)
		{
			if(screen[(j * 64) + i] != 0)
			{
				sf::RectangleShape rect(sf::Vector2f(16, 16));
				rect.setPosition(i * 16, j * 16);
				rect.setFillColor(sf::Color::White);
				renderer->draw(rect);
			}
			else
			{
				sf::RectangleShape rect(sf::Vector2f(16, 16));
				rect.setPosition(i * 16, j * 16);
				rect.setFillColor(sf::Color::Black);
				renderer->draw(rect);
			}
		}
	}
	renderer->display();

	sf::Sprite* sprite = new sf::Sprite();
	sprite->setTexture(renderer->getTexture(), true);

	return sprite;
}

void openGame(const char* filename)
{
	std::ifstream f(filename, std::ios::binary);
	char c;
	int i = 0x200;

	while (!f.eof() && i < 4096) {
		f.get(c);
		memory[i++] = c;
	}
}

void init()
{
	PC = 0x200;
	I = 0;
	DT = 0;
	ST = 0;
}

void clearMemory()
{
	for(int i = 0; i < 4096; ++i)
	{
		memory[i] = 0x0;
	}
}

void clearScreen()
{
	for (int i = 0; i < screen_width; ++i)
	{
		for (int j = 0; j < screen_height; ++j)
		{
			screen[i * j] = 0x0;
		}
	}
}

void loadFont()
{
	for(int i = 0; i < 80; ++i)
	{
		memory[i] = font_set[i];
	}
}

void HandleInput(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Num1:
		keyboard[key_one] = 0x1;
		break;
	case sf::Keyboard::Num2:
		keyboard[key_two] = 0x1;
		break;
	case sf::Keyboard::Num3:
		keyboard[key_three] = 0x1;
		break;
	case sf::Keyboard::Num4:
		keyboard[key_c] = 0x1;
		break;
	case sf::Keyboard::Q:
		keyboard[key_four] = 0x1;
		break;
	case sf::Keyboard::W:
		keyboard[key_five] = 0x1;
		break;
	case sf::Keyboard::E:
		keyboard[key_six] = 0x1;
		break;
	case sf::Keyboard::R:
		keyboard[key_d] = 0x1;
		break;
	case sf::Keyboard::A:
		keyboard[key_seven] = 0x1;
		break;
	case sf::Keyboard::S:
		keyboard[key_eight] = 0x1;
		break;
	case sf::Keyboard::D:
		keyboard[key_nine] = 0x1;
		break;
	case sf::Keyboard::F:
		keyboard[key_e] = 0x1;
		break;
	case sf::Keyboard::Z:
		keyboard[key_a] = 0x1;
		break;
	case sf::Keyboard::X:
		keyboard[key_zero] = 0x1;
		break;
	case sf::Keyboard::C:
		keyboard[key_b] = 0x1;
		break;
	case sf::Keyboard::V:
		keyboard[key_f] = 0x1;
		break;
	}
}

void HandleOutput(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Num1:
		keyboard[key_one] = 0x0;
		break;
	case sf::Keyboard::Num2:
		keyboard[key_two] = 0x0;
		break;
	case sf::Keyboard::Num3:
		keyboard[key_three] = 0x0;
		break;
	case sf::Keyboard::Num4:
		keyboard[key_c] = 0x0;
		break;
	case sf::Keyboard::Q:
		keyboard[key_four] = 0x0;
		break;
	case sf::Keyboard::W:
		keyboard[key_five] = 0x0;
		break;
	case sf::Keyboard::E:
		keyboard[key_six] = 0x0;
		break;
	case sf::Keyboard::R:
		keyboard[key_d] = 0x0;
		break;
	case sf::Keyboard::A:
		keyboard[key_seven] = 0x0;
		break;
	case sf::Keyboard::S:
		keyboard[key_eight] = 0x0;
		break;
	case sf::Keyboard::D:
		keyboard[key_nine] = 0x0;
		break;
	case sf::Keyboard::F:
		keyboard[key_e] = 0x0;
		break;
	case sf::Keyboard::Z:
		keyboard[key_a] = 0x0;
		break;
	case sf::Keyboard::X:
		keyboard[key_zero] = 0x0;
		break;
	case sf::Keyboard::C:
		keyboard[key_b] = 0x0;
		break;
	case sf::Keyboard::V:
		keyboard[key_f] = 0x0;
		break;
	}
}

void clearInput()
{
	for(int i = 0; i < 16; ++i)
	{
		keyboard[i] = 0x0;
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(screen_width * 16, screen_height * 16), "8CHIP Emulator");

	short index = 0;
	short val = 0;

	init();
	loadFont();
	clearMemory();
	clearScreen();
	openGame("TANK");

	while (window.isOpen())
	{
		bool mustDraw = false;

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				HandleInput(event.key.code);
				break;
			case sf::Event::KeyReleased:
				HandleOutput(event.key.code);
				break;
			default:
				break;
			}
		}

		std::uint16_t opcode = memory[PC] << 8 | memory[PC + 1];

		switch (opcode & 0xF000)
		{
		case 0x0000:
			switch (opcode & 0x000F)
			{
			case  0x0000:
				clearScreen();
				mustDraw = true;
				PC += 2;
				break;
			case  0x000E:
				--SP;
				PC = stack[SP];
				PC += 2;
				break;
			}
			break;
		case 0x1000:
			PC = opcode & 0x0FFF;
			break;
		case 0x2000:
			stack[SP] = PC;
			++SP;
			PC = opcode & 0x0FFF;
			break;
		case 0x3000:
			index = opcode & 0x0F00;
			val = opcode & 0x00FF;
			if (V[(index >> 8)] == val) {
				PC += 4;
			}
			else {
				PC += 2;
			}
			break;
		case 0x4000:
			index = opcode & 0x0F00;
			val = opcode & 0x00FF;
			if (V[(index >> 8)] != val) {
				PC += 4;
			}
			else {
				PC += 2;
			}
			break;
		case 0x5000:
			index = opcode & 0x0F00;
			val = opcode & 0x00F0;
			if (V[(index >> 8)] == V[(val >> 4)]) {
				PC += 4;
			}
			else {
				PC += 2;
			}
			break;
		case 0x6000:
			index = opcode & 0x0F00;
			val = opcode & 0x00FF;
			V[(index >> 8)] = val;
			PC += 2;
			break;
		case 0x7000:
			index = opcode & 0x0F00;
			val = opcode & 0x00FF;
			V[(index >> 8)] += val;
			PC += 2;
			break;
		case 0x8000:

			switch (opcode & 0x000F)
			{
			case 0x0000:
				index = opcode & 0x0F00;
				val = opcode & 0x00F0;
				V[(index >> 8)] = V[(val >> 4)];
				PC += 2;
				break;
			case 0x0001:
				index = opcode & 0x0F00;
				val = opcode & 0x00F0;
				V[(index >> 8)] = V[(index >> 8)] | V[(val >> 4)];
				PC += 2;
				break;
			case 0x0002:
				index = opcode & 0x0F00;
				val = opcode & 0x00F0;
				V[(index >> 8)] = V[(index >> 8)] & V[(val >> 4)];
				PC += 2;
				break;
			case 0x0003:
				index = opcode & 0x0F00;
				val = opcode & 0x00F0;
				V[(index >> 8)] = V[(index >> 8)] ^ V[(val >> 4)];
				PC += 2;
				break;
			case 0x0004:
				index = opcode & 0x0F00;
				val = opcode & 0x00F0;
				if (V[(val >> 4)] > (0xFF - V[(index >> 8)]))
				{
					V[0xF] = 1;
				}
				else
				{
					V[0xF] = 0;
				}
				V[(index >> 8)] += V[(val >> 4)];
				PC += 2;
				break;
			case 0x0005:
				index = opcode & 0x0F00;
				val = opcode & 0x00F0;
				if (V[(val >> 4)] > V[(index >> 8)])
				{
					V[0xF] = 0;
				}
				else 
				{
					V[0xF] = 1;
				}
				V[(index >> 8)] -= V[(val >> 4)];
				PC += 2;
				break;
			case 0x0006:
				index = opcode & 0x0F00;
				V[0xF] = V[(index >> 8)] & 0x1;
				V[(index >> 8)] >>= 1;
				PC += 2;
				break;
			case 0x0007:
				index = opcode & 0x00F0;
				val = opcode & 0x00F0;
				if (V[(index >> 4)] > (0xFF - V[(val >> 4)]))
					V[0xF] = 1;
				else
					V[0xF] = 0;
				V[(index >> 8)] = V[(val >> 4)] - V[(index >> 8)];
				PC += 2;
				break;
			case 0x000E:
				index = opcode & 0x0F00;
				V[0xF] = V[index >> 8] >> 7;
				V[index >> 8] <<= 1;
				PC += 2;
				break;
			}

			break;
		case 0x9000:
			index = opcode & 0x0F00;
			val = opcode & 0x00F0;
			if (V[(index >> 8)] != V[(val >> 4)]) PC += 4;
			else PC += 2;
			break;
		case 0xA000:
			I = opcode & 0x0FFF;
			PC += 2;
			break;
		case 0xB000:
			PC = (opcode & 0x0FFF) + V[0];
			break;
		case 0xC000:
			{
			std::uint8_t random = engine();
			index = opcode & 0x0F00;
			val = opcode & 0x00FF;
			V[(index >> 8)] = random | val;
			PC += 2;
			}
			break;
		case 0xD000:
			{
			std::uint16_t x = V[(opcode & 0x0F00) >> 8];
			std::uint16_t y = V[(opcode & 0x00F0) >> 4];
			std::uint16_t height = opcode & 0x000F;
			std::uint16_t pixel;

			V[0xF] = 0;
			for (int yline = 0; yline < height; yline++)
			{
				pixel = memory[I + yline];
				for (int xline = 0; xline < 8; xline++)
				{
					if ((pixel & (0x80 >> xline)) != 0)
					{
						int index = (x + xline + ((y + yline) * 64));
						if (screen[index] == 1)
							V[0xF] = 1;
						if (index > sizeof(screen)) continue;
						screen[index] ^= 1;
					}
				}
			}

			mustDraw = true;
			PC += 2;
			}
			break;
		case 0xE000:
			switch (opcode & 0x00FF)
			{
			case 0x009E:
				if (keyboard[V[(opcode & 0x0F00) >> 8]] != 0)
					PC += 4;
				else
					PC += 2;
				break;

			case 0x00A1:
				if (keyboard[V[(opcode & 0x0F00) >> 8]] == 0)
					PC += 4;
				else
					PC += 2;
				break;
			}
			break;
		case 0xF000:
			switch(opcode & 0x00FF)
			{
			case 0x0007:
				index = opcode & 0x0F00;
				V[(index >> 8)] = DT;
				PC += 2;
				break;
			case 0x000A:
			{
				bool pressed = false;

				for (int i = 0; i < 16; ++i)
				{
					if (keyboard[i] != 0)
					{
						V[(opcode & 0x0F00) >> 8] = i;
						pressed = true;
					}
				}

				if (pressed)
				{
					PC += 2;
				}
			}
				break;
			case 0x0015:
				index = opcode & 0x0F00;
				DT = V[(index >> 8)];
				PC += 2;
				break;
			case 0x0018:
				index = opcode & 0x0F00;
				ST = V[(index >> 8)];
				PC += 2;
				break;
			case 0x001E:
				index = opcode & 0x0F00;
				if (I + V[index >> 8] > 0xFFF)
					V[0xF] = 1;
				else
					V[0xF] = 0;
				I = I + V[(index >> 8)];
				PC += 2;
				break;
			case 0x0029:
				I = V[(opcode & 0x0F00) >> 8] * 0x5;
				PC += 2;
				break;
			case 0x0033:
				memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
				memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
				memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
				PC += 2;
				break;
			case 0x0055:
				for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
				{
					memory[I + i] = V[i];
				}
				I += ((opcode & 0x0F00) >> 8) + 1;
				PC += 2;
				break;
			case 0x0065:
				for (int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
				{
					V[i] = memory[I + i];
				}
				I += ((opcode & 0x0F00) >> 8) + 1;
				PC += 2;
				break;
			}
			break;
		}

		if(mustDraw)
		{
			window.clear();
			window.draw(*drawScreen());
			window.display();
		}

		if (DT > 0) --DT;
		if (ST > 0) --ST;

		std::chrono::milliseconds duration(cpu_speed / 60);
		std::this_thread::sleep_for(duration);
	}

	return 0;
}
