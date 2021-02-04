#pragma once

#include "display.hpp"

class Text
{
public:
	void Print(char c)
	{
		switch (c)
		{
			case '\n':
				GoToXY(0, Y() + 1);
				break;
			case '\r':
				GoToXY(0, Y());
				break;
			case '\b':
				GoToXY(X() - 1, Y());
				break;
			case '\t':
			{
				uint8_t x = X();
				for (uint8_t i = 0; i < 4 - x % 4; i++)
					Print(' ');
				break;
			}
			case '\a': // print cursor
				set_cursor_next = true;
				break;
			default:
			{
				if (!set_cursor_next)
				{
					display.DrawProgMem((uint8_t*)&font[c - font_begin], 1, char_width);
					break;
				}
				set_cursor_next = false;
				uint8_t d[char_width];
				for (uint8_t i = 0; i < char_width; i++)
					d[i] = pgm_read_byte(&font[c - font_begin][i]) | pgm_read_byte(&cursor[i]);
				display.Draw(d, 1, char_width);
				break;
			}
		}
	}
	
	void Print(const char* str)
	{
		for (; *str != '\0'; str++)
			Print(*str);
		if (set_cursor_next)
			Print(' ');
	}
	
	void PrintProgMem(const char* str)
	{
		for (; ; str++)
		{
			char c = pgm_read_byte(str);
			if (c == '\0')
				break;
			Print(c);
		}
		if (set_cursor_next)
			Print(' ');
	}
	
	void GoToXY(uint8_t x, uint8_t y)
	{
		display.GoToXY(x * char_width, y);
	}
	
	uint8_t X() { return display.cursor_x / char_width; }
	uint8_t Y() { return display.cursor_y; }
	
private:
	static const uint8_t char_width = 6;
	static const char font_begin = ' ';
	static const uint8_t font[][char_width];
	static const uint8_t cursor[char_width];
	bool set_cursor_next = false;
};

extern Text text;
