#include "switchparameter.h"

// ##############
// #   PUBLIC   #
// ##############

SwitchParameter::SwitchParameter(
		const QString &title,
		const qsizetype &choicesNumber,
		const QVector<QString> &choicesNames,
		const qsizetype &defaultChoice,
		QWidget *parent
) : QWidget(parent) {
	mainLayout = new QVBoxLayout(this);
	choicesGroup = new QButtonGroup(this);
	label = new QLabel(title, this);

	mainLayout->addWidget(label);
	for (qsizetype choice = 0; choice < choicesNumber; ++choice) {
		QRadioButton *choiceButton = new QRadioButton(choicesNames[choice], this);
		choicesGroup->addButton(choiceButton);
		mainLayout->addWidget(choiceButton);
	}

	setLayout(mainLayout);

	setChoice(defaultChoice);
}

qsizetype SwitchParameter::getChoice() const {
	auto buttons = choicesGroup->buttons();
	for (int i = 0; i < buttons.count(); ++i) {
		if (buttons[i]->isChecked()) {
			return i;
		}
	}
	return 0;
}

void SwitchParameter::setChoice(const qsizetype &choice) {
	auto buttons = choicesGroup->buttons();
	buttons[choice]->setChecked(Qt::Checked);
}
