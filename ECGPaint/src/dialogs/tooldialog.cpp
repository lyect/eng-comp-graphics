#include "tooldialog.h"

// ##########
// # PUBLIC #
// ##########

ToolDialog::ToolDialog(QWidget *parent)
	: QDialog(parent) {

	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

	layout = new QVBoxLayout(this);

	layout->addWidget(buttonBox);

	setLayout(layout);

	QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &ToolDialog::accept);
	QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &ToolDialog::reject);
	QObject::connect(this, &ToolDialog::accepted, this, &ToolDialog::onAccepted);
	QObject::connect(this, &ToolDialog::rejected, this, &ToolDialog::onRejected);
}

void ToolDialog::setParameterValue(const int &parameterIndex, const int &parameterValue) {
	parameters[parameterIndex]->setValue(parameterValue);
}

int ToolDialog::getParameterValue(int parameterIndex) const {
	return parameters[parameterIndex]->getValue();
}

void ToolDialog::open() {

	for (auto param : parameters) {
		openParameterValues.push_back(param->getValue());
	}

	QDialog::open();
}

// #############
// # PROTECTED #
// #############

qsizetype ToolDialog::addParameter(
		const QString &parameterName,
		const int &parameterMinValue,
		const int &parameterDefaultValue,
		const int &parameterMaxValue
) {

	DialogParameter *dialogParameter = new DialogParameter(
			parameterName,
			parameterMinValue,
			parameterDefaultValue,
			parameterMaxValue,
			this
	);

	layout->insertWidget(layout->count() - 1, dialogParameter);
	parameters.push_back(dialogParameter);

	return parameters.count() - 1;
}

// #################
// # PRIVATE SLOTS #
// #################

void ToolDialog::onAccepted() {
	openParameterValues.clear();
}

void ToolDialog::onRejected() {
	for (int pIndex = 0; pIndex < parameters.count(); ++pIndex) {
		parameters[pIndex]->setValue(openParameterValues[pIndex]);
	}
	openParameterValues.clear();
}
