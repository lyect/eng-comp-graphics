#ifndef LINEDIALOG_H
#define LINEDIALOG_H

#include "src/dialogs/tooldialog.h"
#include "src/conf/settings.h"

class LineDialog : public ToolDialog {

	Q_OBJECT

public:

	explicit LineDialog(QWidget *parent = nullptr);

	int selectedThickness() const;

private:

	qsizetype thicknessParameterIndex;
};

#endif // LINEDIALOG_H
