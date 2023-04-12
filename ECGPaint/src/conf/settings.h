#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>

class Settings {

public:

	static Settings &getInstance() {
		static Settings instance;
		return instance;
	}

	Settings(Settings const &other) = delete;
	void operator=(Settings const &other) = delete;

	const int &getMinWindowWidth() const;
	const int &getMinWindowHeight() const;
	const int &getDefaultWindowWidth() const;
	const int &getDefaultWindowHeight() const;

	const int &getMinPaintAreaWidth() const;
	const int &getMinPaintAreaHeight() const;
	const int &getDefaultPaintAreaWidth() const;
	const int &getDefaultPaintAreaHeight() const;
	const int &getMaxPaintAreaWidth() const;
	const int &getMaxPaintAreaHeight() const;

	const int &getMinLineThickness() const;
	const int &getDefaultLineThickness() const;
	const int &getMaxLineThickness() const;

	const int &getMinPolygonThickness() const;
	const int &getDefaultPolygonThickness() const;
	const int &getMaxPolygonThickness() const;
	const int &getMinPolygonVertices() const;
	const int &getDefaultPolygonVertices() const;
	const int &getMaxPolygonVertices() const;
	const int &getMinPolygonRadius() const;
	const int &getDefaultPolygonRadius() const;
	const int &getMaxPolygonRadius() const;
	const int &getMinPolygonAngle() const;
	const int &getDefaultPolygonAngle() const;
	const int &getMaxPolygonAngle() const;

	const int &getMinStarThickness() const;
	const int &getDefaultStarThickness() const;
	const int &getMaxStarThickness() const;
	const int &getMinStarVertices() const;
	const int &getDefaultStarVertices() const;
	const int &getMaxStarVertices() const;
	const int &getMinStarRadius() const;
	const int &getDefaultStarRadius() const;
	const int &getMaxStarRadius() const;
	const int &getMinStarAngle() const;
	const int &getDefaultStarAngle() const;
	const int &getMaxStarAngle() const;

	const QColor &getBackgroundColor() const;

	const int &getIconWidth() const;
	const int &getIconHeight() const;

private:

	Settings();

	static constexpr int MIN_WINDOW_WIDTH = 640;
	static constexpr int MIN_WINDOW_HEIGHT = 480;
	static constexpr int DEFAULT_WINDOW_WIDTH = 480;
	static constexpr int DEFAULT_WINDOW_HEIGHT = 640;

	static constexpr int MIN_PAINT_AREA_WIDTH = 64;
	static constexpr int MIN_PAINT_AREA_HEIGHT = 64;
	static constexpr int DEFAULT_PAINT_AREA_WIDTH = 256;
	static constexpr int DEFAULT_PAINT_AREA_HEIGHT = 256;
	static constexpr int MAX_PAINT_AREA_WIDTH = 1024;
	static constexpr int MAX_PAINT_AREA_HEIGHT = 1024;

	static constexpr int MIN_LINE_THICKNESS = 1;
	static constexpr int DEFAULT_LINE_THICKNESS = 5;
	static constexpr int MAX_LINE_THICKNESS = 10;

	static constexpr int MIN_POLYGON_THICKNESS = 1;
	static constexpr int DEFAULT_POLYGON_THICKNESS = 5;
	static constexpr int MAX_POLYGON_THICKNESS = 10;
	static constexpr int MIN_POLYGON_VERTICES = 3;
	static constexpr int DEFAULT_POLYGON_VERTICES = 6;
	static constexpr int MAX_POLYGON_VERTICES = 16;
	static constexpr int MIN_POLYGON_RADIUS = 32;
	static constexpr int DEFAULT_POLYGON_RADIUS = 64;
	static constexpr int MAX_POLYGON_RADIUS = 512;
	static constexpr int MIN_POLYGON_ANGLE = 0;
	static constexpr int DEFAULT_POLYGON_ANGLE = 0;
	static constexpr int MAX_POLYGON_ANGLE = 359;

	static constexpr int MIN_STAR_THICKNESS = 1;
	static constexpr int DEFAULT_STAR_THICKNESS = 5;
	static constexpr int MAX_STAR_THICKNESS = 10;
	static constexpr int MIN_STAR_VERTICES = 3;
	static constexpr int DEFAULT_STAR_VERTICES = 5;
	static constexpr int MAX_STAR_VERTICES = 16;
	static constexpr int MIN_STAR_RADIUS = 32;
	static constexpr int DEFAULT_STAR_RADIUS = 64;
	static constexpr int MAX_STAR_RADIUS = 512;
	static constexpr int MIN_STAR_ANGLE = 0;
	static constexpr int DEFAULT_STAR_ANGLE = 0;
	static constexpr int MAX_STAR_ANGLE = 359;

	const QColor BACKGROUND_COLOR = Qt::white;

	static constexpr int ICON_WIDTH = 40;
	static constexpr int ICON_HEIGHT = 40;
};

#endif // SETTINGS_H
