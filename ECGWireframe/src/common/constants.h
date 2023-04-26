#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "qcolor.h"

namespace Constants {

	// ###################
	// #   CONSTRAINTS   #
	// ###################

	// Window constraints

	constexpr int MIN_WINDOW_WIDTH = 640;
	constexpr int MIN_WINDOW_HEIGHT = 480;

	// Editor constraints

	constexpr int MIN_EDITOR_WIDTH = 1200;
	constexpr int MIN_EDITOR_HEIGHT = 960;
	constexpr double MIN_EDITOR_ZOOM_FACTOR = 1.0;
	constexpr double MAX_EDITOR_ZOOM_FACTOR = 3.0;

	// Render parameters constraints

	constexpr int MIN_N_RENDER_PARAMETER_VALUE = 1;
	constexpr int MAX_N_RENDER_PARAMETER_VALUE = 150;
	constexpr int MIN_M1_RENDER_PARAMETER_VALUE = 10;
	constexpr int MAX_M1_RENDER_PARAMETER_VALUE = 150;
	constexpr int MIN_M2_RENDER_PARAMETER_VALUE = 10;
	constexpr int MAX_M2_RENDER_PARAMETER_VALUE = 150;

	// Geometry constraints

	constexpr double EPSILON = 1e-6;

	// ################
	// #   DEFAULTS   #
	// ################

	// Window defaults

	constexpr int DEFAULT_WINDOW_WIDTH = 800;
	constexpr int DEFAULT_WINDOW_HEIGHT = 600;

	// Editor defaults

	constexpr int DEFAULT_EDITOR_WIDTH = 1200;
	constexpr int DEFAULT_EDITOR_HEIGHT = 960;
	constexpr double EDITOR_AXIS_TICK_WIDTH = 5.0;
	constexpr double EDITOR_AXIS_TICK_STEP = 20.0;
	constexpr double EDITOR_AXIS_TICK_START = 0.5;
	constexpr double EDITOR_AXIS_ARROW_WIDTH = 10.0;
	constexpr double EDITOR_AXIS_ARROW_LENGTH = 16.0;
	const QColor EDITOR_AXIS_COLOR = Qt::white;
	const QColor EDITOR_BACKGROUND_COLOR = Qt::black;
	const QColor EDITOR_CONTROL_OBJECT_COLOR = Qt::red;
	const QColor EDITOR_SPLINE_COLOR = Qt::cyan;
	const QColor EDITOR_SELECTION_COLOR = Qt::white;
	constexpr double EDITOR_ZOOM_FACTOR_STEP = 0.05;
	constexpr double EDITOR_CONTROL_POINT_RADIUS = 10.0;

	// Render parameters defaults

	constexpr int DEFAULT_N_RENDER_PARAMETER_VALUE = 10;
	constexpr int DEFAULT_M1_RENDER_PARAMETER_VALUE = 10;
	constexpr int DEFAULT_M2_RENDER_PARAMETER_VALUE = 10;

} // Constants

#endif // CONSTANTS_H
