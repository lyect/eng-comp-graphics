#ifndef BLURFILTERDIALOG_H
#define BLURFILTERDIALOG_H

#include "dialogs/base/parameterdialog.h"

class BlurFilterDialog : public ParameterDialog {

	Q_OBJECT

public:

	explicit BlurFilterDialog(QWidget *parent = nullptr);

	int getChosenSize() const;

private:

	qsizetype sizeParameterIndex;
};

#endif // BLURFILTERDIALOG_H
