#include "robertsfilterdialog.h"

#include "constants.h"

RobertsFilterDialog::RobertsFilterDialog(QWidget *parent) : ParameterDialog(parent) {
	binarizationThresholdParameterIndex = addNumericParameter(
				"Binarization threshold",
				Constants::MIN_ROBERTS_FILTER_BINARIZATION_THRESHOLD,
				Constants::MAX_ROBERTS_FILTER_BINARIZATION_THRESHOLD,
				Constants::DEFAULT_ROBERTS_FILTER_BINARIZATION_THRESHOLD,
				Constants::ROBERTS_FILTER_BINARIZATION_THRESHOLD_STEP
	);
	firstColorParameterIndex = addColorParameter(
				"First color",
				Constants::DEFAULT_ROBERTS_FILTER_FIRST_COLOR
	);
	secondColorParameterIndex = addColorParameter(
				"Second color",
				Constants::DEFAULT_ROBERTS_FILTER_SECOND_COLOR
	);
}

float RobertsFilterDialog::getBinarizationThreshold() const {
	return getNumericParameterValue(binarizationThresholdParameterIndex);
}

QColor RobertsFilterDialog::getFirstColor() const {
	return getColorParameterValue(firstColorParameterIndex);
}

QColor RobertsFilterDialog::getSecondColor() const {
	return getColorParameterValue(secondColorParameterIndex);
}

std::tuple<QColor, QColor> RobertsFilterDialog::getChosenColors() const {
	QColor firstColor = getColorParameterValue(firstColorParameterIndex);
	QColor secondColor = getColorParameterValue(secondColorParameterIndex);

	return std::make_tuple(firstColor, secondColor);
}
