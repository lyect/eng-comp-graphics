#ifndef NUMERICPARAMETER_H
#define NUMERICPARAMETER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>

#include "numericslider.h"

class NumericParameter : public QWidget {

	Q_OBJECT

public:

	NumericParameter(
			const QString &title,
			const float &minValue,
			const float &maxValue,
			const float &defaultValue,
			const float &step,
			QWidget *parent = nullptr
	);

	float getValue() const;
	void setValue(const float &value);

private:

	QVBoxLayout *mainLayout;
	QLabel *label;
	NumericSlider *slider;
	QLineEdit *lineEdit;

	float minValue;
	float maxValue;
	float defaultValue;

	QString minValueText;
	QString maxValueText;
	QString defaultValueText;

	float lastCorrectValue;

	void connectWidgets();

signals:

};

#endif // NUMERICPARAMETER_H
