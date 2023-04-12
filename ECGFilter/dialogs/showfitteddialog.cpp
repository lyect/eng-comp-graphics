#include "showfitteddialog.h"

ShowFittedDialog::ShowFittedDialog(QWidget *parent) : ParameterDialog(parent) {
	interpolationParameterIndex = addSwitchParameter(
			"Interpolation",
			6,
			{
					"Nearest neighbor",
					"Bilinear",
					"Bicubic",
					"Lanczos",
					"Bit exact bilinear",
					"Bit exact nearest neighbor"
			},
			1
	);
}

cv::InterpolationFlags ShowFittedDialog::getChosenInterpolation() const {
	qsizetype interpolationChoice = getSwitchParameterChoice(interpolationParameterIndex);

	switch (interpolationChoice) {
	case 0:
		return cv::INTER_NEAREST;
	case 1:
		return cv::INTER_LINEAR;
	case 2:
		return cv::INTER_CUBIC;
	case 3:
		return cv::INTER_LANCZOS4;
	case 4:
		return cv::INTER_LINEAR_EXACT;
	case 5:
		return cv::INTER_NEAREST_EXACT;
	}

	qWarning("unknown interpolation");

	return cv::INTER_LINEAR;
}
