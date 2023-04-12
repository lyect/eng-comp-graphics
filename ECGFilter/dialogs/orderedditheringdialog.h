#ifndef ORDEREDDITHERINGDIALOG_H
#define ORDEREDDITHERINGDIALOG_H

#include "dialogs/base/parameterdialog.h"

class OrderedDitheringDialog : public ParameterDialog {

	Q_OBJECT

public:

	explicit OrderedDitheringDialog(QWidget *parent = nullptr);

	int getChosenRedQuantization() const;
	int getChosenGreenQuantization() const;
	int getChosenBlueQuantization() const;
	std::tuple<int, int, int> getChosenQuantization() const;

private:

	qsizetype redQuantizationParameterIndex;
	qsizetype greenQuantizationParameterIndex;
	qsizetype blueQuantizationParameterIndex;
};

#endif // ORDEREDDITHERINGDIALOG_H
