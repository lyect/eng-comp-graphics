#include "orderedditheringdialog.h"

#include "constants.h"

OrderedDitheringDialog::OrderedDitheringDialog(QWidget *parent) : ParameterDialog(parent) {
	redQuantizationParameterIndex = addNumericParameter(
				"Red color quantization",
				Constants::MIN_ORDERED_DITHERING_RED_QUANTIZATION,
				Constants::MAX_ORDERED_DITHERING_RED_QUANTIZATION,
				Constants::DEFAULT_ORDERED_DITHERING_RED_QUANTIZATION,
				Constants::ORDERED_DITHERING_RED_QUANTIZATION_STEP
	);
	greenQuantizationParameterIndex = addNumericParameter(
				"Green color quantization",
				Constants::MIN_ORDERED_DITHERING_GREEN_QUANTIZATION,
				Constants::MAX_ORDERED_DITHERING_GREEN_QUANTIZATION,
				Constants::DEFAULT_ORDERED_DITHERING_GREEN_QUANTIZATION,
				Constants::ORDERED_DITHERING_GREEN_QUANTIZATION_STEP
	);
	blueQuantizationParameterIndex = addNumericParameter(
				"Blue color quantization",
				Constants::MIN_ORDERED_DITHERING_BLUE_QUANTIZATION,
				Constants::MAX_ORDERED_DITHERING_BLUE_QUANTIZATION,
				Constants::DEFAULT_ORDERED_DITHERING_BLUE_QUANTIZATION,
				Constants::ORDERED_DITHERING_BLUE_QUANTIZATION_STEP
	);
}

int OrderedDitheringDialog::getChosenRedQuantization() const {
	return getNumericParameterValue(redQuantizationParameterIndex);
}

int OrderedDitheringDialog::getChosenGreenQuantization() const {
	return getNumericParameterValue(greenQuantizationParameterIndex);
}

int OrderedDitheringDialog::getChosenBlueQuantization() const {
	return getNumericParameterValue(blueQuantizationParameterIndex);
}

std::tuple<int, int, int> OrderedDitheringDialog::getChosenQuantization() const {
	int redQuantization = getNumericParameterValue(redQuantizationParameterIndex);
	int greenQuantization = getNumericParameterValue(greenQuantizationParameterIndex);
	int blueQuantization = getNumericParameterValue(blueQuantizationParameterIndex);
	return std::make_tuple(redQuantization, greenQuantization, blueQuantization);
}
