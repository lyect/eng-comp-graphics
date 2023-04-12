#include "stardialog.h"

StarDialog::StarDialog(QWidget *parent)
	: ToolDialog(parent) {

	Settings &settings = Settings::getInstance();

	thicknessParameterIndex = addParameter(
				"Thickness",
				settings.getMinPolygonThickness(),
				settings.getDefaultPolygonThickness(),
				settings.getMaxPolygonThickness()
	);

	verticesParameterIndex = addParameter(
				"Vertices",
				settings.getMinPolygonVertices(),
				settings.getDefaultPolygonVertices(),
				settings.getMaxPolygonVertices()
	);

	radiusParameterIndex = addParameter(
				"Radius",
				settings.getMinPolygonRadius(),
				settings.getDefaultPolygonRadius(),
				settings.getMaxPolygonRadius()
	);

	angleParameterIndex = addParameter(
				"Angle",
				settings.getMinPolygonAngle(),
				settings.getDefaultPolygonAngle(),
				settings.getMaxPolygonAngle()
	);
}

int StarDialog::selectedThickness() const {
	return getParameterValue(thicknessParameterIndex);
}

int StarDialog::selectedVertices() const {
	return getParameterValue(verticesParameterIndex);
}

int StarDialog::selectedRadius() const {
	return getParameterValue(radiusParameterIndex);
}

int StarDialog::selectedAngle() const {
	return getParameterValue(angleParameterIndex);
}
