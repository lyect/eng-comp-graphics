#ifndef SOBELFILTERDIALOG_H
#define SOBELFILTERDIALOG_H

#include "dialogs/base/parameterdialog.h"

class SobelFilterDialog : public ParameterDialog {

	Q_OBJECT

public:
	explicit SobelFilterDialog(QWidget *parent = nullptr);

	float getBinarizationThreshold() const;
	QColor getFirstColor() const;
	QColor getSecondColor() const;
	std::tuple<QColor, QColor> getChosenColors() const;

private:

	qsizetype binarizationThresholdParameterIndex;
	qsizetype firstColorParameterIndex;
	qsizetype secondColorParameterIndex;
};
#endif // SOBELFILTERDIALOG_H
