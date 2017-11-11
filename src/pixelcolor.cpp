#include "pixelcolor.h"
#include "helpers.h"

PixelColor::PixelColor(unsigned char _a, unsigned char _b, unsigned char _c, bool _isrgb) {
	a = _a;
	b = _b;
	c = _c;
	isrgb = _isrgb;
}

PixelColor::PixelColor(const PixelColor &p) {
	a = p.a;
	b = p.b;
	c = p.c;
	isrgb = p.isrgb;
}

void PixelColor::SetH(unsigned char& h) {
	if (isrgb) {
		return;
	}
	a = h;
}

void PixelColor::SetS(unsigned char& s) {
	if (isrgb) {
		return;
	}
	b = s;
}

void PixelColor::SetV(unsigned char& v) {
	if (isrgb) {
		return;
	}
	c = v;
}

void PixelColor::ToHSV() {
	if (isrgb) {
		isrgb = false;
		unsigned char min, max, delta, s, v;
		min = helpers::Min(a, helpers::Min(b, c));
		max = helpers::Max(a, helpers::Max(b, c));
		delta = max - min;
		v = max;
		if (max > 0) {
			s = (unsigned char)(((double)delta)*255.0 / (double)max);
		}
		else {
			b = 0;
			a = 0;
			c = max;
			return;
		}

		if (a == max)
			a = (unsigned char)((42.5*(double)(b - c)) / (double)delta);
		else if (b == max)
			a = (unsigned char)((42.5*(double)(c - a)) / (double)delta) + 86;
		else
			a = (unsigned char)((42.5*(double)(a - b)) / (double)delta) + 171;
		b = s;
		c = v;
	}
}

void PixelColor::ToRGB() {
	if (!isrgb) {
		isrgb = true;
		if (b == 0) {
			a = b = c;
			return;
		}
		int i;
		double f, p, q, t, h, r, g, bb;

		h = a / 42.5;

		i = h;
		f = h - i;

		double s = (double)b / 255.0;
		double v = (double)c / 255.0;

		p = v * (1.0 - s);
		q = v * (1.0 - (s * f));
		t = v * (1.0 - (s * (1.0 - f)));

		switch (i) {
		case 0:
			r = v;
			g = t;
			bb = p;
			break;
		case 1:
			r = q;
			g = v;
			bb = p;
			break;
		case 2:
			r = p;
			g = v;
			bb = t;
			break;
		case 3:
			r = p;
			g = q;
			bb = v;
			break;
		case 4:
			r = t;
			g = p;
			bb = v;
			break;
		default:
			r = v;
			g = p;
			bb = q;
			break;
		}

		a = (unsigned char)(r * 255);
		b = (unsigned char)(g * 255);
		c = (unsigned char)(bb * 255);
	}
}

unsigned char PixelColor::GetR() {
	if (isrgb) {
		return a;
	}
	return 0; // to indicate misuse
}

unsigned char PixelColor::GetG() {
	if (isrgb) {
		return b;
	}
	return 0; // to indicate misuse
}

unsigned char PixelColor::GetB() {
	if (isrgb) {
		return c;
	}
	return 0; // to indicate misuse
}
