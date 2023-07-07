#include "sobelfilterdialog.h"

#include "constants.h"

SobelFilterDialog::SobelFilterDialog(QWidget *parent) : ParameterDialog(parent) {
	binarizationThresholdParameterIndex = addNumericParameter(
				"Binarization threshold",
				Constants::MIN_SOBEL_FILTER_BINARIZATION_THRESHOLD,
				Constants::MAX_SOBEL_FILTER_BINARIZATION_THRESHOLD,
				Constants::DEFAULT_SOBEL_FILTER_BINARIZATION_THRESHOLD,
				Constants::SOBEL_FILTER_BINARIZATION_THRESHOLD_STEP
	);
	firstColorParameterIndex = addColorParameter(
				"First color",
				Constants::DEFAULT_SOBEL_FILTER_FIRST_COLOR
	);
	secondColorParameterIndex = addColorParameter(
				"Second color",
				Constants::DEFAULT_SOBEL_FILTER_SECOND_COLOR
	);
}

float SobelFilterDialog::getBinarizationThreshold() const {
	return getNumericParameterValue(binarizationThresholdParameterIndex);
}


QColor SobelFilterDialog::getFirstColor() const {
	return getColorParameterValue(firstColorParameterIndex);
}

QColor SobelFilterDialog::getSecondColor() const {
	return getColorParameterValue(secondColorParameterIndex);
}

std::tuple<QColor, QColor> SobelFilterDialog::getChosenColors() const {
	QColor firstColor = getColorParameterValue(firstColorParameterIndex);
	QColor secondColor = getColorParameterValue(secondColorParameterIndex);

	return std::make_tuple(firstColor, secondColor);
}
