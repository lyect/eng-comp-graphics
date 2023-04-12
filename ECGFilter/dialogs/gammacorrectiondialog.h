#ifndef GAMMACORRECTIONDIALOG_H
#define GAMMACORRECTIONDIALOG_H

#include "dialogs/base/parameterdialog.h"

class GammaCorrectionDialog : public ParameterDialog {

	Q_OBJECT

public:

	explicit GammaCorrectionDialog(QWidget *parent = nullptr);

	float getChosenGammaValue() const;

private:

	qsizetype gammaValueParameterIndex;

};

#endif // GAMMACORRECTIONDIALOG_H
