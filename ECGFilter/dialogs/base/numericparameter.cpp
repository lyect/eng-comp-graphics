#include "numericparameter.h"

// ##############
// #   PUBLIC   #
// ##############

NumericParameter::NumericParameter(
		const QString &title,
		const float &minValue,
		const float &maxValue,
		const float &defaultValue,
		const float &step,
		QWidget *parent
) : QWidget(parent) {

	this->minValue = minValue;
	this->maxValue = maxValue;
	this->defaultValue = defaultValue;

	minValueText = QString::number(minValue);
	maxValueText = QString::number(maxValue);
	defaultValueText = QString::number(defaultValue);

	lastCorrectValue = minValue - 1;

	mainLayout = new QVBoxLayout(this);
	label = new QLabel(title, this);
	slider = new NumericSlider(
				minValue,
				maxValue,
				defaultValue,
				step,
				this
	);
	lineEdit = new QLineEdit(this);

	lineEdit->setPlaceholderText(defaultValueText);

	mainLayout->addWidget(label);
	mainLayout->addWidget(slider);
	mainLayout->addWidget(lineEdit);

	setLayout(mainLayout);

	connectWidgets();
}

float NumericParameter::getValue() const {
	return slider->getValue();
}

void NumericParameter::setValue(const float &value) {
	lastCorrectValue = value;
	slider->setValue(value);
}

// ###############
// #   PRIVATE   #
// ###############

void NumericParameter::connectWidgets() {
	// Connect slider to the lineEdit
	QObject::connect(slider, &QSlider::valueChanged, this, [=](int realValue){
		float numericValue = slider->getValue();
		lineEdit->setText(QString::number(numericValue));
		lastCorrectValue = numericValue;
	});

	// Connect lineEdit to the slider
	QObject::connect(lineEdit, &QLineEdit::editingFinished, this, [=](){

		QString text = lineEdit->text();

		if (text.isEmpty()) {
			slider->blockSignals(true);
			setValue(defaultValue);
			lastCorrectValue = minValue - 1;
			slider->blockSignals(false);
			return;
		}

		bool conversionStatus;
		float value = text.toFloat(&conversionStatus);

		if (!conversionStatus) {
			if (lastCorrectValue == minValue - 1) {
				lineEdit->setText("");
			}
			else {
				lineEdit->setText(QString::number(lastCorrectValue));
			}
			return;
		}

		if (value < minValue) {
			lineEdit->setText(minValueText);
			slider->blockSignals(true);
			slider->setValue(minValue);
			lastCorrectValue = minValue;
			slider->blockSignals(false);
			return;
		}

		if (value > maxValue) {
			lineEdit->setText(maxValueText);
			slider->blockSignals(true);
			slider->setValue(maxValue);
			lastCorrectValue = maxValue;
			slider->blockSignals(false);
			return;
		}

		slider->blockSignals(true);
		slider->setValue(value);
		lastCorrectValue = value;
		slider->blockSignals(false);
		lineEdit->setText(QString::number(slider->getValue()));
	});
}
