#include "imagerotationdialog.h"

#include "constants.h"

ImageRotationDialog::ImageRotationDialog(QWidget *parent) : ParameterDialog(parent) {
	rotationAngleParameterIndex = addNumericParameter(
				"Rotation angle",
				Constants::MIN_ROTATION_ANGLE,
				Constants::MAX_ROTATION_ANGLE,
				Constants::DEFAULT_ROTATION_ANGLE,
				Constants::ROTATION_ANGLE_STEP
	);
}

int ImageRotationDialog::getChosenRotationAngle() const {
	return getNumericParameterValue(rotationAngleParameterIndex);
}
