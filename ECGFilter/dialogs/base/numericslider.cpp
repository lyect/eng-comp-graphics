#include "numericslider.h"

// ##############
// #   PUBLIC   #
// ##############

NumericSlider::NumericSlider(
		const float &minValue,
		const float &maxValue,
		const float &defaultValue,
		const float &step,
		QWidget *parent
) : QSlider(Qt::Horizontal, parent) {

	this->realMinValue = 0;
	// Without +0.5 division error happens
	this->realMaxValue = static_cast<int>((maxValue - minValue) / step + 0.5);

	this->numericMinValue = minValue;
	this->numericMaxValue = maxValue;

	this->step = step;

	setMinimum(realMinValue);
	setMaximum(realMaxValue);

	setValue(defaultValue);
}

float NumericSlider::getValue() const {

	return real2numeric(QSlider::value());
}

void NumericSlider::setValue(const float &value) {
	QSlider::setValue(numeric2real(value));
}

// ###############
// #   PRIVATE   #
// ###############

float NumericSlider::real2numeric(const int &realValue) const {
	if (realValue < realMinValue) {
		return numericMinValue;
	}
	if (realValue > realMaxValue) {
		return numericMaxValue;
	}

	return step * (realValue - realMinValue) + numericMinValue;
}

int NumericSlider::numeric2real(const float &numericValue) const {
	if (numericValue < numericMinValue) {
		return realMinValue;
	}
	if (numericValue > numericMaxValue) {
		return realMaxValue;
	}

	for (int x = realMinValue; x <= realMaxValue; ++x) {
		if (x * step + numericMinValue <= numericValue && numericValue < (x + 1) * step + numericMinValue) {
			return x;
		}
	}
}
