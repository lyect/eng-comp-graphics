#ifndef CONSTANTS_H
#define CONSTANTS_H

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

	// Render parameters constraints

	constexpr int MIN_N_RENDER_PARAMETER_VALUE = 10;
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

	// Render parameters constraints

	constexpr int DEFAULT_N_RENDER_PARAMETER_VALUE = 50;
	constexpr int DEFAULT_M1_RENDER_PARAMETER_VALUE = 50;
	constexpr int DEFAULT_M2_RENDER_PARAMETER_VALUE = 50;

} // Constants

#endif // CONSTANTS_H
