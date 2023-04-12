#include "floydditheringdialog.h"

#include "constants.h"

FloydDitheringDialog::FloydDitheringDialog(QWidget *parent) : ParameterDialog(parent) {
	redQuantizationParameterIndex = addNumericParameter(
				"Red color quantization",
				Constants::MIN_FLOYD_DITHERING_RED_QUANTIZATION,
				Constants::MAX_FLOYD_DITHERING_RED_QUANTIZATION,
				Constants::DEFAULT_FLOYD_DITHERING_RED_QUANTIZATION,
				Constants::FLOYD_DITHERING_RED_QUANTIZATION_STEP
	);
	greenQuantizationParameterIndex = addNumericParameter(
				"Green color quantization",
				Constants::MIN_FLOYD_DITHERING_GREEN_QUANTIZATION,
				Constants::MAX_FLOYD_DITHERING_GREEN_QUANTIZATION,
				Constants::DEFAULT_FLOYD_DITHERING_GREEN_QUANTIZATION,
				Constants::FLOYD_DITHERING_GREEN_QUANTIZATION_STEP
	);
	blueQuantizationParameterIndex = addNumericParameter(
				"Blue color quantization",
				Constants::MIN_FLOYD_DITHERING_BLUE_QUANTIZATION,
				Constants::MAX_FLOYD_DITHERING_BLUE_QUANTIZATION,
				Constants::DEFAULT_FLOYD_DITHERING_BLUE_QUANTIZATION,
				Constants::FLOYD_DITHERING_BLUE_QUANTIZATION_STEP
	);
}

int FloydDitheringDialog::getChosenRedQuantization() const {
	return getNumericParameterValue(redQuantizationParameterIndex);
}

int FloydDitheringDialog::getChosenGreenQuantization() const {
	return getNumericParameterValue(greenQuantizationParameterIndex);
}

int FloydDitheringDialog::getChosenBlueQuantization() const {
	return getNumericParameterValue(blueQuantizationParameterIndex);
}

std::tuple<int, int, int> FloydDitheringDialog::getChosenQuantization() const {
	int redQuantization = getNumericParameterValue(redQuantizationParameterIndex);
	int greenQuantization = getNumericParameterValue(greenQuantizationParameterIndex);
	int blueQuantization = getNumericParameterValue(blueQuantizationParameterIndex);
	return std::make_tuple(redQuantization, greenQuantization, blueQuantization);
}
