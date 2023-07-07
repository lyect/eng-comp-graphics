#ifndef ROBERTSFILTERDIALOG_H
#define ROBERTSFILTERDIALOG_H

#include "dialogs/base/parameterdialog.h"

class RobertsFilterDialog : public ParameterDialog {

	Q_OBJECT

public:
	explicit RobertsFilterDialog(QWidget *parent = nullptr);

	float getBinarizationThreshold() const;
	QColor getFirstColor() const;
	QColor getSecondColor() const;
	std::tuple<QColor, QColor> getChosenColors() const;

private:

	qsizetype binarizationThresholdParameterIndex;
	qsizetype firstColorParameterIndex;
	qsizetype secondColorParameterIndex;
};

#endif // ROBERTSFILTERDIALOG_H
