#include "resizedialog.h"

ResizeDialog::ResizeDialog(QWidget *parent)
	: ToolDialog(parent) {

	Settings &settings = Settings::getInstance();

	widthParameterIndex = addParameter(
				"Width",
				settings.getMinPaintAreaWidth(),
				settings.getDefaultPaintAreaWidth(),
				settings.getMaxPaintAreaWidth()
	);

	heightParameterIndex = addParameter(
				"Height",
				settings.getMinPaintAreaHeight(),
				settings.getDefaultPaintAreaHeight(),
				settings.getMaxPaintAreaHeight()
	);
}

void ResizeDialog::setWidth(const int &width) {
	setParameterValue(widthParameterIndex, width);
}

void ResizeDialog::setHeight(const int &height) {
	setParameterValue(heightParameterIndex, height);
}

int ResizeDialog::selectedWidth() const {
	return getParameterValue(widthParameterIndex);
}

int ResizeDialog::selectedHeight() const {
	return getParameterValue(heightParameterIndex);
}
