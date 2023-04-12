#include "linedialog.h"

LineDialog::LineDialog(QWidget *parent)
	: ToolDialog(parent)
{
	Settings &settings = Settings::getInstance();

	thicknessParameterIndex = addParameter(
				"Thickness",
				settings.getMinLineThickness(),
				settings.getDefaultLineThickness(),
				settings.getMaxLineThickness()
	);
}

int LineDialog::selectedThickness() const {
	return getParameterValue(thicknessParameterIndex);
}
