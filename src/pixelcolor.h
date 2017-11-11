#pragma once

class PixelColor
{
private:
	unsigned char a; // r, h
	unsigned char b; // g, s
	unsigned char c; // b, v
	bool isrgb; // false: hsv

public:
	PixelColor(unsigned char _a, unsigned char _b, unsigned char _c, bool _isrgb = true);
	PixelColor(const PixelColor &p);

	void SetH(unsigned char& h);
	void SetS(unsigned char& s);
	void SetV(unsigned char& v);

	unsigned char GetR();
	unsigned char GetG();
	unsigned char GetB();

	void ToHSV();
	void ToRGB();
};
