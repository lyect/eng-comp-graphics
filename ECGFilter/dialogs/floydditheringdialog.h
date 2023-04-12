#ifndef FLOYDDITHERINGDIALOG_H
#define FLOYDDITHERINGDIALOG_H

#include "dialogs/base/parameterdialog.h"

class FloydDitheringDialog : public ParameterDialog {

	Q_OBJECT

public:

	explicit FloydDitheringDialog(QWidget *parent = nullptr);

	int getChosenRedQuantization() const;
	int getChosenGreenQuantization() const;
	int getChosenBlueQuantization() const;
	std::tuple<int, int, int> getChosenQuantization() const;

private:

	qsizetype redQuantizationParameterIndex;
	qsizetype greenQuantizationParameterIndex;
	qsizetype blueQuantizationParameterIndex;
};

#endif // FLOYDDITHERINGDIALOG_H
