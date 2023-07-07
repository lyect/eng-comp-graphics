#include "blurfilterdialog.h"

#include "constants.h"

BlurFilterDialog::BlurFilterDialog(QWidget *parent) : ParameterDialog(parent) {
	sizeParameterIndex = addNumericParameter(
				"Size",
				Constants::MIN_BLUR_FILTER_SIZE,
				Constants::MAX_BLUR_FILTER_SIZE,
				Constants::DEFAULT_BLUR_FILTER_SIZE,
				Constants::BLUR_FILTER_SIZE_STEP
	);
}

int BlurFilterDialog::getChosenSize() const {
	return getNumericParameterValue(sizeParameterIndex);
}
