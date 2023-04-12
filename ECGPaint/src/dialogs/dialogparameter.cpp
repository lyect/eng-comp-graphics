#include "dialogparameter.h"

// ##########
// # PUBLIC #
// ##########

DialogParameter::DialogParameter(
		const QString &title,
		const int &minValue,
		const int &defaultValue,
		const int &maxValue,
		QWidget *parent
) : QWidget(parent) {
	this->minValue = minValue;
	this->defaultValue = defaultValue;
	this->maxValue = maxValue;

	minValueText = QString::number(minValue);
	defaultValueText = QString::number(defaultValue);
	maxValueText = QString::number(maxValue);

	// Consider minValue - 1 as NULL value
	lastCorrectValue = minValue - 1;

	layout = new QVBoxLayout(this);

	parameterLabel = new QLabel(title, this);
	parameterSlider = new QSlider(Qt::Horizontal, this);
	parameterLineEdit = new QLineEdit(this);

	layout->addWidget(parameterLabel);
	layout->addWidget(parameterSlider);
	layout->addWidget(parameterLineEdit);

	setLayout(layout);

	initializeWidgets();
	connectWidgets();
}

void DialogParameter::setValue(const int &value) {
	lastCorrectValue = value;
	parameterSlider->setValue(value);
}

int DialogParameter::getValue() const {
	return parameterSlider->value();
}

// ###########
// # PRIVATE #
// ###########

void DialogParameter::initializeWidgets() {
	parameterSlider->setMinimum(minValue);
	parameterSlider->setMaximum(maxValue);
	parameterSlider->setValue(defaultValue);

	parameterLineEdit->setPlaceholderText(defaultValueText);
}

void DialogParameter::connectWidgets() {
	// Connect parameterSlider to the parameterLineEdit
	QObject::connect(parameterSlider, &QSlider::valueChanged, this, [=](int value){
		parameterLineEdit->setText(QString::number(value));
		lastCorrectValue = value;
	});

	// Connect parameterLineEdit to the parameterSlider
	QObject::connect(parameterLineEdit, &QLineEdit::textEdited, this, [=](const QString &text){

		if (text.isEmpty()) {
			parameterSlider->blockSignals(true);
			parameterSlider->setValue(defaultValue);
			lastCorrectValue = minValue - 1;
			parameterSlider->blockSignals(false);
			return;
		}

		bool conversionStatus;
		int value = text.toInt(&conversionStatus);

		if (!conversionStatus) {
			if (lastCorrectValue == minValue - 1) {
				parameterLineEdit->setText("");
			}
			else {
				parameterLineEdit->setText(QString::number(lastCorrectValue));
			}
			return;
		}

		if (value < minValue) {
			parameterLineEdit->setText(minValueText);
			parameterSlider->blockSignals(true);
			parameterSlider->setValue(minValue);
			lastCorrectValue = minValue;
			parameterSlider->blockSignals(false);
			return;
		}

		if (value > maxValue) {
			parameterLineEdit->setText(maxValueText);
			parameterSlider->blockSignals(true);
			parameterSlider->setValue(maxValue);
			lastCorrectValue = maxValue;
			parameterSlider->blockSignals(false);
			return;
		}

		parameterLineEdit->setText(text);
		parameterSlider->blockSignals(true);
		parameterSlider->setValue(value);
		lastCorrectValue = value;
		parameterSlider->blockSignals(false);
	});
}
