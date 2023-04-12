#include "span.h"

Span::Span() {
	xl = -1;
	xr = -1;
	y = -1;
}

Span::Span(const int &y) {
	xl = -1;
	xr = -1;
	this->y = y;
}

Span::Span(const int &xl, const int &xr, const int &y) {
	this->xl = xl;
	this->xr = xr;
	this->y = y;
}

int Span::getXl() const {
	return xl;
}
int Span::getXr() const {
	return xr;
}
int Span::getY() const {
	return y;
}

void Span::setXl(const int &xl) {
	this->xl = xl;
}
void Span::setXr(const int &xr) {
	this->xr = xr;
}
void Span::setY(const int &y) {
	this->y = y;
}

bool Span::operator<(const Span &other) const {
	return (y < other.y || (y == other.y && xl < other.xl) || (y == other.y && xl == other.xl && xr < other.xr));
}

bool Span::operator==(const Span &other) const {
	return (y == other.y && xl == other.xl && xr == other.xr);
}
