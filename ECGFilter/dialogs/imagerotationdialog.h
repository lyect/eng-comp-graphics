#ifndef IMAGEROTATIONDIALOG_H
#define IMAGEROTATIONDIALOG_H

#include "dialogs/base/parameterdialog.h"

class ImageRotationDialog : public ParameterDialog {

	Q_OBJECT

public:

	explicit ImageRotationDialog(QWidget *parent = nullptr);

	int getChosenRotationAngle() const;

private:

	qsizetype rotationAngleParameterIndex;

};

#endif // IMAGEROTATIONDIALOG_H
