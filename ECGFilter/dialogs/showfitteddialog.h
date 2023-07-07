#ifndef SHOWFITTEDDIALOG_H
#define SHOWFITTEDDIALOG_H

#include <opencv2/imgproc.hpp>
#include "dialogs/base/parameterdialog.h"

class ShowFittedDialog : public ParameterDialog {

	Q_OBJECT

public:
	explicit ShowFittedDialog(QWidget *parent = nullptr);

	cv::InterpolationFlags getChosenInterpolation() const;

private:

	qsizetype interpolationParameterIndex;
};
#endif // SHOWFITTEDDIALOG_H
