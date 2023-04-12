#include "parameterdialog.h"

// ##############
// #   PUBLIC   #
// ##############

ParameterDialog::ParameterDialog(QWidget *parent) : QDialog(parent) {
	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

	mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);

	QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &ParameterDialog::accept);
	QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &ParameterDialog::reject);
	QObject::connect(this, &ParameterDialog::accepted, this, &ParameterDialog::onAccepted);
	QObject::connect(this, &ParameterDialog::rejected, this, &ParameterDialog::onRejected);
}

void ParameterDialog::setNumericParameterValue(const int &numericParameterIndex, const float &numericParameterValue) {
	numericParameters[numericParameterIndex]->setValue(numericParameterValue);
}

float ParameterDialog::getNumericParameterValue(const int &numericParameterIndex) const {
	return numericParameters[numericParameterIndex]->getValue();
}

void ParameterDialog::setColorParameterValue(const int &colorParameterIndex, const QColor &colorParameterValue) {
	colorParameters[colorParameterIndex]->setColor(colorParameterValue);
}

QColor ParameterDialog::getColorParameterValue(const int &colorParameterIndex) const {
	return colorParameters[colorParameterIndex]->getColor();
}

void ParameterDialog::setSwitchParameterChoice(const int &switchParameterIndex, const qsizetype &choice) {
	switchParameters[switchParameterIndex]->setChoice(choice);
}

qsizetype ParameterDialog::getSwitchParameterChoice(const int &switchParameterIndex) const {
	return switchParameters[switchParameterIndex]->getChoice();
}

// #################
// #   PROTECTED   #
// #################

qsizetype ParameterDialog::addNumericParameter(
		const QString &parameterName,
		const float &parameterMinValue,
		const float &parameterMaxValue,
		const float &parameterDefaultValue,
		const float &step
) {

	NumericParameter *numericParameter = new NumericParameter(
			parameterName,
			parameterMinValue,
			parameterMaxValue,
			parameterDefaultValue,
			step,
			this
	);

	mainLayout->insertWidget(mainLayout->count() - 1, numericParameter);
	numericParameters.push_back(numericParameter);

	return numericParameters.count() - 1;
}

qsizetype ParameterDialog::addColorParameter(
		const QString &parameterName,
		const QColor &parameterDefaultColor
) {

	ColorParameter *colorParameter = new ColorParameter(
				parameterName,
				parameterDefaultColor,
				this
	);

	mainLayout->insertWidget(mainLayout->count() - 1, colorParameter);
	colorParameters.push_back(colorParameter);

	return colorParameters.count() - 1;
}

qsizetype ParameterDialog::addSwitchParameter(
		const QString &title,
		const qsizetype &choicesNumber,
		const QVector<QString> &choicesNames,
		const qsizetype &defaultChoice
) {

	SwitchParameter *switchParameter = new SwitchParameter(
				title,
				choicesNumber,
				choicesNames,
				defaultChoice,
				this
	);

	mainLayout->insertWidget(mainLayout->count() - 1, switchParameter);
	switchParameters.push_back(switchParameter);

	return switchParameters.count() - 1;
}

// ################
// # PUBLIC SLOTS #
// ################

int ParameterDialog::exec() {

	for (auto np : numericParameters) {
		openNumericParameterValues.push_back(np->getValue());
	}

	for (auto cp : colorParameters) {
		openColorParameterValues.push_back(cp->getColor());
	}

	for (auto sp : switchParameters) {
		openSwitchParameterValues.push_back(sp->getChoice());
	}

	return QDialog::exec();
}

// #################
// # PRIVATE SLOTS #
// #################

void ParameterDialog::onAccepted() {
	openNumericParameterValues.clear();
	openColorParameterValues.clear();
	openSwitchParameterValues.clear();
}

void ParameterDialog::onRejected() {
	for (int npIndex = 0; npIndex < numericParameters.count(); ++npIndex) {
		numericParameters[npIndex]->setValue(openNumericParameterValues[npIndex]);
	}
	openNumericParameterValues.clear();

	for (int cpIndex = 0; cpIndex < colorParameters.count(); ++cpIndex) {
		colorParameters[cpIndex]->setColor(openColorParameterValues[cpIndex]);
	}
	openColorParameterValues.clear();

	for (int spIndex = 0; spIndex < switchParameters.count(); ++spIndex) {
		switchParameters[spIndex]->setChoice(openSwitchParameterValues[spIndex]);
	}
	openSwitchParameterValues.clear();
}
