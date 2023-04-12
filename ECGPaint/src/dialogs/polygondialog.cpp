#include "polygondialog.h"

PolygonDialog::PolygonDialog(QWidget *parent)
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

int PolygonDialog::selectedThickness() const {
	return getParameterValue(thicknessParameterIndex);
}

int PolygonDialog::selectedVertices() const {
	return getParameterValue(verticesParameterIndex);
}

int PolygonDialog::selectedRadius() const {
	return getParameterValue(radiusParameterIndex);
}

int PolygonDialog::selectedAngle() const {
	return getParameterValue(angleParameterIndex);
}
