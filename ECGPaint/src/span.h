#ifndef SPAN_H
#define SPAN_H

#include <QtCore/QHashFunctions>

class Span {

public:

	Span();
	Span(const int &y);
	Span(const int &xl, const int &xr, const int &y);

	int getXl() const;
	int getXr() const;
	int getY() const;

	void setXl(const int &xl);
	void setXr(const int &xr);
	void setY(const int &y);

	bool operator<(const Span &other) const;
	bool operator==(const Span &other) const;

private:

	int xl;
	int xr;
	int y;

};

inline uint qHash(const Span &span, size_t seed) {
	return qHash(span.getXl() * span.getXr() * span.getY(), seed);
}

#endif // SPAN_H
