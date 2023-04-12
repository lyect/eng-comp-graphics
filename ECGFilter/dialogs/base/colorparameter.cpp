#include "colorparameter.h"

#include "constants.h"

// ##############
// #   PUBLIC   #
// ##############

ColorParameter::ColorParameter(
		const QString &title,
		const QColor &defaultColor,
		QWidget *parent
) : QWidget(parent) {

	mainLayout = new QVBoxLayout(this);
	label = new QLabel(title, this);
	button = new QPushButton(this);

	setColor(defaultColor);
	button->setIconSize(QSize(
				Constants::COLOR_PARAMETER_BUTTON_FILLING_WIDTH,
				Constants::COLOR_PARAMETER_BUTTON_FILLING_HEIGHT
	));

	mainLayout->addWidget(label);
	mainLayout->addWidget(button);

	setLayout(mainLayout);

	connectWidgets();
}

QColor ColorParameter::getColor() const {
	return lastColor;
}

void ColorParameter::setColor(const QColor &color) {
	lastColor = color;

	QPixmap colorPixmap(
				Constants::COLOR_PARAMETER_BUTTON_FILLING_WIDTH - 2 * Constants::COLOR_PARAMETER_BUTTON_FILLING_WIDTH_INDENT,
				Constants::COLOR_PARAMETER_BUTTON_FILLING_HEIGHT - 2 * Constants::COLOR_PARAMETER_BUTTON_FILLING_HEIGHT_INDENT
	);
	colorPixmap.fill(lastColor);

	button->setIcon(QIcon(colorPixmap));

	update();
}

// ###############
// #   PRIVATE   #
// ###############

void ColorParameter::connectWidgets() {
	QObject::connect(button, &QPushButton::clicked, this, [this]() -> void {
		auto c = QColorDialog::getColor(lastColor, this, "Choose color");
		setColor(c);
	});
}
