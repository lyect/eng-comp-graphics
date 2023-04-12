#ifndef STARDIALOG_H
#define STARDIALOG_H

#include "src/dialogs/tooldialog.h"
#include "src/conf/settings.h"

class StarDialog : public ToolDialog {

	Q_OBJECT

public:

	explicit StarDialog(QWidget *parent = nullptr);

	int selectedThickness() const;
	int selectedVertices() const;
	int selectedRadius() const;
	int selectedAngle() const;

private:

	qsizetype thicknessParameterIndex;
	qsizetype verticesParameterIndex;
	qsizetype radiusParameterIndex;
	qsizetype angleParameterIndex;
};

#endif // STARDIALOG_H
