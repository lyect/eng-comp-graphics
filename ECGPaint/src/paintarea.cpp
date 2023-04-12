#include "paintarea.h"

PaintArea::PaintArea(
		int paintSceneWidth,
		int paintSceneHeight,
		QColor backgroundColor,
		QWidget *parent
) : QGraphicsView(parent)
{
	setAlignment(Qt::AlignLeft | Qt::AlignTop);

	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	paintScene = new QGraphicsScene(this);
	setScene(paintScene);

	this->paintSceneWidth = paintSceneWidth;
	this->paintSceneHeight = paintSceneHeight;

	this->backgroundColor = backgroundColor;

	lastPixmapItem = nullptr;

	paintImage = QImage(paintSceneWidth, paintSceneHeight, QImage::Format_RGB32);
	paintImage.fill(backgroundColor);
	setSceneRect(0, 0, paintSceneWidth, paintSceneHeight);
	lastPixmapItem = paintScene->addPixmap(QPixmap::fromImage(paintImage));
}

int PaintArea::getWidth() const {
	return paintSceneWidth;
}

int PaintArea::getHeight() const {
	return paintSceneHeight;
}

void PaintArea::resize(const int &width, const int &height) {
	this->paintSceneWidth = width;
	this->paintSceneHeight = height;

	newPaintImage = QImage(paintSceneWidth, paintSceneHeight, QImage::Format_RGB32);
	newPaintImage.fill(backgroundColor);

	QPainter painter;
	painter.begin(&newPaintImage);
	painter.drawImage(0, 0, paintImage);
	painter.end();

	setSceneRect(0, 0, paintSceneWidth, paintSceneHeight);

	redrawImage();
}

void PaintArea::drawLine(
		const QPoint &p1,
		const QPoint &p2,
		const int &thickness,
		const QColor &color
) {
	newPaintImage = paintImage;

	if (thickness == 1) { // Bresenham algorithm
		drawBresenhamLine(p1, p2, color);
	}
	else {
		drawCommonLine(
					p1, p2,
					thickness,
					color
		);
	}

	redrawImage();
}

void PaintArea::drawPolygon(
		const QPoint &p,
		const int &thickness,
		const int &vertices,
		const int &radius,
		const int &angle,
		const QColor &color
) {
	newPaintImage = paintImage;

	auto points = getPolygonPoints(p, vertices, radius, angle);

	int n = points.count();

	for (int i = 1; i < n; ++i) {
		drawFigureEdge(points[i - 1], points[i], thickness, color);
	}
	drawFigureEdge(points[n - 1], points[0], thickness, color);

	redrawImage();
}

void PaintArea::drawStar(
		const QPoint &p,
		const int &thickness,
		const int &vertices,
		const int &radius,
		const int &angle,
		const QColor &color
) {
	newPaintImage = paintImage;

	auto outsidePoints = getPolygonPoints(p, vertices, radius, angle);
	auto insidePoints = getPolygonPoints(p, vertices, 0.5 * radius, angle - 180 / vertices);

	int n = outsidePoints.count();

	for (int i = 0; i < n; ++i) {
		drawFigureEdge(outsidePoints[i], insidePoints[i], thickness, color);
	}

	for (int i = 0; i < n - 1; ++i) {
		drawFigureEdge(insidePoints[i], outsidePoints[i + 1], thickness, color);
	}

	drawFigureEdge(insidePoints[n - 1], outsidePoints[0], thickness, color);

	redrawImage();
}

void PaintArea::fill(
		const QPoint &p,
		const QColor &newColor
) {
	QRgb newColorRgb = qRgb(newColor.red(), newColor.green(), newColor.blue());

	QColor oldColor = paintImage.pixelColor(p);
	QRgb oldColorRgb = qRgb(oldColor.red(), oldColor.green(), oldColor.blue());

	if (oldColor == newColor) {
		return;
	}

	QStack<Span> spans;

	newPaintImage = paintImage;
	QRgb *pixels = (QRgb*)(newPaintImage.bits());

	auto makeXl = [this, &pixels, &oldColorRgb](int &xl, const int &y){
		while (xl >= 0 && pixels[y * paintSceneWidth + xl] == oldColorRgb) {
			--xl;
		}
		xl += 1;
	};
	auto makeXr = [this, &pixels, &oldColorRgb](int &xr, const int &y){
		while (xr < paintSceneWidth && pixels[y * paintSceneWidth + xr] == oldColorRgb) {
			++xr;
		}
		xr -= 1;
	};

	int xl = p.x();
	int xr = p.x();

	makeXl(xl, p.y());
	makeXr(xr, p.y());

	spans.push_back(Span(xl, xr, p.y()));

	while (!spans.isEmpty()) {

		Span span = spans.pop();

		const int y = span.getY();

		for (int x = span.getXl(); x <= span.getXr(); ++x) {
			pixels[y * paintSceneWidth + x] = newColorRgb;
		}

		bool topChecked = false;
		if (0 <= y - 1) {
			for (int x = span.getXl(); x <= span.getXr(); ++x) {
				if (pixels[(y - 1) * paintSceneWidth + x] == oldColorRgb) {
					if (!topChecked) {
						topChecked = true;

						xl = x;
						xr = x;
						makeXl(xl, y - 1);
						makeXr(xr, y - 1);

						spans.push(Span(xl, xr, y - 1));
					}
				}
				else {
					topChecked = false;
				}
			}
		}

		bool bottomChecked = false;
		if (y + 1 < paintSceneHeight) {
			for (int x = span.getXl(); x <= span.getXr(); ++x) {
				if (pixels[(y + 1) * paintSceneWidth + x] == oldColorRgb) {
					if (!bottomChecked) {
						bottomChecked = true;

						xl = x;
						xr = x;
						makeXl(xl, y + 1);
						makeXr(xr, y + 1);

						spans.push(Span(xl, xr, y + 1));
					}
				}
				else {
					bottomChecked = false;
				}
			}
		}
	}

	redrawImage();
}

void PaintArea::clear() {
	newPaintImage = QImage(paintSceneWidth, paintSceneHeight, QImage::Format_RGB32);
	newPaintImage.fill(backgroundColor);

	redrawImage();
}

void PaintArea::drawImage(const QImage &image) {
	this->paintSceneWidth = image.width();
	this->paintSceneHeight = image.height();

	newPaintImage = image;
	setSceneRect(0, 0, paintSceneWidth, paintSceneHeight);

	redrawImage();
}

bool PaintArea::canFit(const QImage &image) const {

	Settings &settings = Settings::getInstance();

	const int &width = image.width();
	const int &height = image.height();

	bool goodWidth = settings.getMinPaintAreaWidth() <= width && width <= settings.getMaxPaintAreaWidth();
	bool goodHeight = settings.getMinPaintAreaHeight() <= height && height <= settings.getMaxPaintAreaHeight();

	return goodWidth && goodHeight;
}

const QImage &PaintArea::getImage() const {
	return paintImage;
}

void PaintArea::redrawImage() {
	paintScene->removeItem(lastPixmapItem);
	paintImage = newPaintImage;
	delete lastPixmapItem;

	lastPixmapItem = paintScene->addPixmap(QPixmap::fromImage(paintImage));
}

QVector<QPoint> PaintArea::getPolygonPoints(
		const QPoint &p,
		const int &vertices,
		const int &radius,
		const int &angle
) {
	QVector<QPoint> points;

	float polygonAngle = 2 * M_PI / vertices;

	QPointF prevPoint(
				-radius * std::sin(M_PI * angle / 180),
				-radius * std::cos(M_PI * angle / 180)
	);

	points.push_back(prevPoint.toPoint());

	for (int v = 0; v < vertices - 1; ++v) {
		// x' = x * cos(a) - y * sin(a)
		// y' = x * sin(a) + y * cos(a)

		QPointF curPoint(
				prevPoint.x() * std::cos(polygonAngle) - prevPoint.y() * std::sin(polygonAngle),
				prevPoint.x() * std::sin(polygonAngle) + prevPoint.y() * std::cos(polygonAngle)
		);

		points.push_back(curPoint.toPoint());

		prevPoint = curPoint;
	}

	for (auto &point : points) {
		point.setX(point.x() + p.x());
		point.setY(point.y() + p.y());
	}

	return points;
}

void PaintArea::drawCommonLine(
		const QPoint &p1,
		const QPoint &p2,
		const int &thickness,
		const QColor &color
) {
	QPainter painter;
	painter.begin(&newPaintImage);
	painter.setPen(QPen(color, thickness));
	painter.drawLine(p1, p2);
	painter.end();
}

void PaintArea::drawBresenhamLine(
		QPoint p1,
		QPoint p2,
		const QColor &color
) {

	QRgb colorRGB = qRgb(color.red(), color.green(), color.blue());

	// Vertical lines
	if (p1.x() == p2.x()) {
		for (int y = std::min(p1.y(), p2.y()); y <= std::max(p1.y(), p2.y()); ++y) {
			newPaintImage.setPixel(p1.x(), y, colorRGB);
		}
		return;
	}

	// Horizontal line
	if (p1.y() == p2.y()) {
		for (int x = p1.x(); x <= p2.x(); ++x) {
			newPaintImage.setPixel(x, p1.y(), colorRGB);
		}
		return;
	}

	int dx = abs(p2.x() - p1.x());
	int dy = abs(p2.y() - p1.y());

	if (dy < dx) {

		// X1 must be an X coordinate of the left point
		if (p1.x() > p2.x()) {
			std::swap(p1, p2);
		}

		const bool upSlope = ((p1.y() > p2.y()) ? true : false);

		int x = p1.x();
		int y = p1.y();
		int err = 0;

		while (x != p2.x() || y != p2.y()) {
			if (0 <= x && x < paintSceneWidth && 0 <= y && y < paintSceneHeight) {
				newPaintImage.setPixel(x, y, colorRGB);
			}
			++x;
			err += 2 * dy;
			if (err > dx) {
				err -= 2 * dx;
				y += ((upSlope) ? -1 : 1);
			}
		}

		if (0 <= p2.x() && p2.x() < paintSceneWidth && 0 <= p2.y() && p2.y() < paintSceneHeight) {
			newPaintImage.setPixel(p2, colorRGB);
		}
	}
	else {

		// Y1 must be an Y coordinate of the top point
		if (p1.y() > p2.y()) {
			std::swap(p1, p2);
		}

		const bool upSlope = ((p2.x() > p1.x()) ? true : false);

		int x = p1.x();
		int y = p1.y();
		int err = 0;

		while (x != p2.x() || y != p2.y()) {
			if (0 <= x && x < paintSceneWidth && 0 <= y && y < paintSceneHeight) {
				newPaintImage.setPixel(x, y, colorRGB);
			}
			++y;
			err += 2 * dx;
			if (err > dy) {
				err -= 2 * dy;
				x += ((upSlope) ? 1 : -1);
			}
		}

		if (0 <= p2.x() && p2.x() < paintSceneWidth && 0 <= p2.y() && p2.y() < paintSceneHeight) {
			newPaintImage.setPixel(p2, colorRGB);
		}
	}
}

void PaintArea::drawFigureEdge(
		const QPoint &p1,
		const QPoint &p2,
		const int &thickness,
		const QColor &color
) {
	if (thickness == 1) {
		drawBresenhamLine(p1, p2, color);
	}
	else {
		drawCommonLine(p1, p2, thickness, color);
	}
}
