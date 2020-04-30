#include "puha-render.h"

static int magic_value = 0xfeedfeed; // used to validate buffer writes, if these get overriden then algorithm is wrong
static int font_char_width = 6; // the width of a single char in the font data
static int canvas_width = 320;
static int canvas_height = 200;

Gfx320x200::Gfx320x200()
{
	// we allocate a sligly larger buffer so one off errors can be detected
	// and won't crash the application
	real_buffer = new int[canvas_width * (canvas_height + 2)];
	buffer = real_buffer + canvas_width;

	// we fill first row and last rows of buffer
	{
		int* first_row = real_buffer;
		int* last_row = buffer + canvas_width * canvas_height;
		for (int i = 0; i < 320; i++) {
			first_row[i] = last_row[i] = magic_value;
		}
	}

	color = 0x0;
	RectangleFilled(0, 0, 319, 199);
	color = 0xffffff;
}

bool Gfx320x200::AreBoundsValid() {
	bool is_valid = true;
	int* first_row = real_buffer;
	int* last_row = buffer + canvas_width * canvas_height;
	for (int i = 0; i < canvas_width; i++) {
		if (first_row[i] != 0xfeedfeed || last_row[i] != 0xfeedfeed) {
			is_valid = false;
		}
	}
	return is_valid;
}

ClippedGfx<Gfx320x200> Gfx320x200::CreateViewport()
{
	return CreateViewport(0, 0, Width, Height);
}

ClippedGfx<Gfx320x200> Gfx320x200::CreateViewport(int x, int y, int w, int h)
{
	return ClippedGfx<Gfx320x200>(*this, x, y, x+w, y+h);
}

Gfx320x200::~Gfx320x200() {
	delete[] real_buffer;
	real_buffer = nullptr;
	buffer = nullptr;
}


void Gfx320x200::PutStr(int x, int y, char* s, int color)
{
	while (*s)
	{
		PutChar(x, y, *s, color);
		x += font_char_width;
		s++;
	}
}

static unsigned char font_data_mg[408] = {
	0x20, 0x20, 0x20, 0x00, 0x20, 0x00, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00,
	0x50, 0xF8, 0x50, 0xF8, 0x50, 0x00, 0x78, 0xA0, 0x70, 0x28, 0xF0, 0x00,
	0x00, 0x90, 0x20, 0x40, 0x90, 0x00, 0x40, 0xA8, 0x50, 0x90, 0x68, 0x00,
	0x20, 0x20, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x40, 0x40, 0x20, 0x00,
	0x20, 0x10, 0x10, 0x10, 0x20, 0x00, 0x50, 0x20, 0x50, 0x00, 0x00, 0x00,
	0x00, 0x20, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x00,
	0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
	0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x70, 0x98, 0xA8, 0xC8, 0x70, 0x00,
	0x20, 0x60, 0xA0, 0x20, 0xF8, 0x00, 0xF0, 0x08, 0x70, 0x80, 0xF8, 0x00,
	0xF0, 0x08, 0x70, 0x08, 0xF0, 0x00, 0x80, 0x90, 0x90, 0xF8, 0x10, 0x00,
	0xF8, 0x80, 0xF0, 0x08, 0xF0, 0x00, 0x70, 0x80, 0xF0, 0x88, 0x70, 0x00,
	0xF8, 0x08, 0x10, 0x20, 0x20, 0x00, 0x70, 0x88, 0x70, 0x88, 0x70, 0x00,
	0x70, 0x88, 0x78, 0x08, 0x70, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00,
	0x00, 0x40, 0x00, 0x40, 0x40, 0x00, 0x10, 0x20, 0x40, 0x20, 0x10, 0x00,
	0x00, 0x70, 0x00, 0x70, 0x00, 0x00, 0x40, 0x20, 0x10, 0x20, 0x40, 0x00,
	0xC0, 0x20, 0x40, 0x00, 0x40, 0x00, 0x70, 0x88, 0x98, 0x80, 0x70, 0x00,
	0x70, 0x88, 0x88, 0xF8, 0x88, 0x00, 0xF0, 0x88, 0xF0, 0x88, 0xF0, 0x00,
	0x78, 0x80, 0x80, 0x80, 0x78, 0x00, 0xF0, 0x88, 0x88, 0x88, 0xF0, 0x00,
	0xF8, 0x80, 0xE0, 0x80, 0xF8, 0x00, 0xF8, 0x80, 0xE0, 0x80, 0x80, 0x00,
	0xF8, 0x80, 0x98, 0x88, 0xF8, 0x00, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x00,
	0xF8, 0x20, 0x20, 0x20, 0xF8, 0x00, 0x18, 0x08, 0x08, 0x88, 0x70, 0x00,
	0x88, 0x90, 0xE0, 0x90, 0x88, 0x00, 0x80, 0x80, 0x80, 0x80, 0xF8, 0x00,
	0x88, 0xD8, 0xA8, 0x88, 0x88, 0x00, 0x88, 0xC8, 0xA8, 0x98, 0x88, 0x00,
	0x70, 0x88, 0x88, 0x88, 0x70, 0x00, 0xF0, 0x88, 0xF0, 0x80, 0x80, 0x00,
	0x70, 0x88, 0x88, 0x90, 0x68, 0x00, 0xF0, 0x88, 0xF0, 0x88, 0x88, 0x00,
	0x78, 0x80, 0x70, 0x08, 0xF0, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x00,
	0x88, 0x88, 0x88, 0x88, 0x70, 0x00, 0x88, 0x88, 0x50, 0x50, 0x20, 0x00,
	0x88, 0x88, 0xA8, 0xA8, 0x50, 0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 0x00,
	0x88, 0x88, 0x50, 0x20, 0x20, 0x00, 0xF8, 0x10, 0x20, 0x40, 0xF8, 0x00,
	0x60, 0x40, 0x40, 0x40, 0x60, 0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00,
	0x30, 0x10, 0x10, 0x10, 0x30, 0x00, 0x20, 0x50, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x10, 0x20, 0x60, 0x20, 0x10, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
	0x40, 0x20, 0x30, 0x20, 0x40, 0x00, 0x00, 0x28, 0x50, 0x00, 0x00, 0x00
};

void Gfx320x200::PutChar(int x, int y, char c, int color)
{
	int offs = 54;
	int bwidth = 5;
	unsigned tile = 0;

	if (((c >= '!') && (c <= '`')) || ((c >= '{') && (c <= '~')))
		tile = c - '!';
	else
		if ((c >= 'a') && (c <= 'z'))
			tile = c - 'a' + 'A' - '!';
		else return;

	int bitoffs = tile * 6;

	for (int y0 = 0; y0 < 5; y0++)
		for (int x0 = 0; x0 < 5; x0++)
		{
			int ss = y0 + bitoffs;
			if (((font_data_mg[ss]) >> (7 - x0)) & 1) PutPixel(x + x0, y + y0, color);
		}
}

void Gfx320x200::Span(int x, int y, int w, int color)
{
	int *dest = &(buffer[x + (y << 6) + (y << 8)]);
	int *endp = dest + w;
	while (dest < endp) *(dest++) = color;
}

void Gfx320x200::HLine(int x1, int y1, int x2, int y2, int color)
{
	Span(x1, y1, x2 - x1 + 1, color);
}
void Gfx320x200::VLine(int x1, int y1, int x2, int y2, int color)
{
	int* dest = &(buffer[x1 + (y1 << 6) + (y1 << 8)]);
	int w = y2 - y1 + 1;
	int *endp = dest + (w << 6) + (w << 8);
	while (dest < endp) { *(dest) = color; dest = dest + 320; }
}

void Gfx320x200::Rectangle(int x1, int y1, int x2, int y2, int color)
{
	HLine(x1, y1, x2, y1, color); 
	HLine(x1, y2, x2, y2, color);
	VLine(x1, y1, x1, y2, color); 
	VLine(x2, y1, x2, y2, color);
}

void Gfx320x200::RectangleFilled(int x1, int y1, int x2, int y2, int color)
{
	int w = x2 - x1 + 1;
	for (int i = y1; i <= y2; i++) Span(x1, i, w, color);
}

void Gfx320x200::RectFill(int x1, int y1, int w, int h, int color)
{
	for (int i = 0; i < h; i++) Span(x1, y1+i, w, color);
}

void Gfx320x200::Line(int x1, int y1, int x2, int y2, int color)
{
	int dy = y2 - y1;

	if (dy < 0) { dy = -dy; x1 = x1 ^ x2; x2 = x1 ^ x2; x1 = x1 ^ x2; y1 = y1 ^ y2; y2 = y1 ^ y2; y1 = y1 ^ y2; }
	int dx = x2 - x1;

	int sx;
	if (dx < 0) { dx = -dx; sx = -1; }
	else sx = 1;


	if (dx == 0) { VLine(x1, y1, x2, y2, color); return; }
	else if (dy == 0) { if (sx > 0)HLine(x1, y1, x2, y2, color); else HLine(x2, y1, x1, y2); return; }


	int y0 = y1;
	int x0 = x1;
	if (dx >= dy)
	{
		int err = 0;
		int cx = dx; cx++;
		while (cx--)
		{
			PutPixel(x0, y0, color);
			x0 += sx;
			err += dy + 1;
			if (err > dx) { err -= dx; y0 += 1; }
		}
	}
	else
	{
		int err = 0;
		int cx = dy; cx++;
		while (cx--)
		{
			PutPixel(x0, y0, color);
			y0 += 1;
			err += dx;
			if (err > dy) { err -= dy; x0 += sx; }
		}
	}

}
