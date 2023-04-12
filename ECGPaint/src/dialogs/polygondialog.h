#ifndef POLYGONDIALOG_H
#define POLYGONDIALOG_H

#include "src/dialogs/tooldialog.h"
#include "src/conf/settings.h"

class PolygonDialog : public ToolDialog {

	Q_OBJECT

public:

	explicit PolygonDialog(QWidget *parent = nullptr);

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

#endif // POLYGONDIALOG_H
