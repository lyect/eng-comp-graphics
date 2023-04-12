#ifndef NUMERICSLIDER_H
#define NUMERICSLIDER_H

#include <QSlider>

class NumericSlider : public QSlider {

	Q_OBJECT

public:

	NumericSlider(
			const float &minValue,
			const float &maxValue,
			const float &defaultValue,
			const float &step,
			QWidget *parent = nullptr
	);

	float getValue() const;
	void setValue(const float &value);

private:

	int realMinValue;
	int realMaxValue;
	float numericMinValue;
	float numericMaxValue;
	float step;

	float real2numeric(const int &realValue) const;
	int numeric2real(const float &numericValue) const;
};

#endif // NUMERICSLIDER_H
