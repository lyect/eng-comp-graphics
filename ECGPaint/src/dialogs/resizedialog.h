#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include "src/dialogs/tooldialog.h"
#include "src/conf/settings.h"

class ResizeDialog : public ToolDialog {

	Q_OBJECT

public:

	explicit ResizeDialog(QWidget *parent = nullptr);

	void setWidth(const int &width);
	void setHeight(const int &height);

	int selectedWidth() const;
	int selectedHeight() const;

private:

	qsizetype widthParameterIndex;
	qsizetype heightParameterIndex;
};

#endif // RESIZEDIALOG_H
