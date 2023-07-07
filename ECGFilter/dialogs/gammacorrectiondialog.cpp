#include "gammacorrectiondialog.h"

#include "constants.h"

GammaCorrectionDialog::GammaCorrectionDialog(QWidget *parent) : ParameterDialog(parent) {
	gammaValueParameterIndex = addNumericParameter(
				"Gamma value",
				Constants::MIN_GAMMA_VALUE,
				Constants::MAX_GAMMA_VALUE,
				Constants::DEFAULT_GAMMA_VALUE,
				Constants::GAMMA_VALUE_STEP
	);
}

float GammaCorrectionDialog::getChosenGammaValue() const {
	return getNumericParameterValue(gammaValueParameterIndex);
}
