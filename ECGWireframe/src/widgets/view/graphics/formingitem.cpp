#include "formingitem.h"

#include "src/common/constants.h"

FormingItem::FormingItem(
		const qreal *xAnglePtr,
		const qreal *yAnglePtr,
		const qreal *zAnglePtr,
		const QVector3D *cameraPointPtr,
		const qreal *znPtr,
		const qreal *matrixPtr,
		QGraphicsItem *parent
) : QGraphicsItem(parent) {
	m_xAnglePtr = xAnglePtr;
	m_yAnglePtr = yAnglePtr;
	m_zAnglePtr = zAnglePtr;

	m_cameraPointPtr = cameraPointPtr;
	m_znPtr = znPtr;

	m_matrixPtr = matrixPtr;

	m_scale = 1;
}

void FormingItem::addPoint(const QVector4D &point) {
	m_points.push_back(point);
}

void FormingItem::setScale(qreal scale) {
	m_scale = scale;

	for (auto &p : m_points) {
		p.setX(p.x() / m_scale);
		p.setY(p.y() / m_scale);
		p.setZ(p.z() / m_scale);
		//p.setW(p.w() / m_scale);
	}
}

void FormingItem::updateForming(
		const qreal *xAnglePtr,
		const qreal *yAnglePtr,
		const qreal *zAnglePtr,
		const QVector3D *cameraPointPtr,
		const qreal *znPtr,
		const qreal *matrixPtr
) {
	m_xAnglePtr = xAnglePtr;
	m_yAnglePtr = yAnglePtr;
	m_zAnglePtr = zAnglePtr;

	m_cameraPointPtr = cameraPointPtr;
	m_znPtr = znPtr;

	m_matrixPtr = matrixPtr;

	prepareGeometryChange();
}

QRectF FormingItem::boundingRect() const {
	qreal xMin, xMax;
	qreal yMin, yMax;
	qreal w, x, y;
	w = m_matrixPtr[12] * m_points[0].x() + \
		m_matrixPtr[13] * m_points[0].y() + \
		m_matrixPtr[14] * m_points[0].z() + \
		m_matrixPtr[15] * m_points[0].w();
	x = m_matrixPtr[0] * m_points[0].x() + \
		m_matrixPtr[1] * m_points[0].y() + \
		m_matrixPtr[2] * m_points[0].z() + \
		m_matrixPtr[3] * m_points[0].w();
	y = m_matrixPtr[4] * m_points[0].x() + \
		m_matrixPtr[5] * m_points[0].y() + \
		m_matrixPtr[6] * m_points[0].z() + \
		m_matrixPtr[7] * m_points[0].w();
	x /= w;
	y /= w;
	xMin = x; xMax = x;
	yMin = y; yMax = y;
	for (qsizetype t = 1; t < m_points.count(); ++t) {
		w = m_matrixPtr[12] * m_points[t].x() + \
			m_matrixPtr[13] * m_points[t].y() + \
			m_matrixPtr[14] * m_points[t].z() + \
			m_matrixPtr[15] * m_points[t].w();
		x = m_matrixPtr[0] * m_points[t].x() + \
			m_matrixPtr[1] * m_points[t].y() + \
			m_matrixPtr[2] * m_points[t].z() + \
			m_matrixPtr[3] * m_points[t].w();
		y = m_matrixPtr[4] * m_points[t].x() + \
			m_matrixPtr[5] * m_points[t].y() + \
			m_matrixPtr[6] * m_points[t].z() + \
			m_matrixPtr[7] * m_points[t].w();
		x /= w;
		y /= w;
		xMin = qMin(xMin, x);
		xMax = qMax(xMax, x);
		yMin = qMin(yMin, y);
		yMax = qMax(yMax, y);
	}

	return QRectF(QPointF(xMin * m_scale, yMin * m_scale), QPointF(xMax * m_scale, yMax * m_scale));
}

void FormingItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QPen prevPainterPen = painter->pen();
	QBrush prevPainterBrush = painter->brush();

	painter->setPen(QPen(Constants::MAIN_WIREFRAME_COLOR));
	painter->setBrush(Qt::NoBrush);

	qreal pw, px, py;
	pw = m_matrixPtr[12] * m_points[0].x() + \
		m_matrixPtr[13] * m_points[0].y() + \
		m_matrixPtr[14] * m_points[0].z() + \
		m_matrixPtr[15] * m_points[0].w();
	px = m_matrixPtr[0] * m_points[0].x() + \
		m_matrixPtr[1] * m_points[0].y() + \
		m_matrixPtr[2] * m_points[0].z() + \
		m_matrixPtr[3] * m_points[0].w();
	py = m_matrixPtr[4] * m_points[0].x() + \
		m_matrixPtr[5] * m_points[0].y() + \
		m_matrixPtr[6] * m_points[0].z() + \
		m_matrixPtr[7] * m_points[0].w();
	if (pw != 0) {
		px /= pw;
		py /= pw;
	}
	for (qsizetype t = 0; t < m_points.count(); ++t) {
		qreal cw = m_matrixPtr[12] * m_points[t].x() + \
			m_matrixPtr[13] * m_points[t].y() + \
			m_matrixPtr[14] * m_points[t].z() + \
			m_matrixPtr[15] * m_points[t].w();
		qreal cx = m_matrixPtr[0] * m_points[t].x() + \
			m_matrixPtr[1] * m_points[t].y() + \
			m_matrixPtr[2] * m_points[t].z() + \
			m_matrixPtr[3] * m_points[t].w();
		qreal cy = m_matrixPtr[4] * m_points[t].x() + \
			m_matrixPtr[5] * m_points[t].y() + \
			m_matrixPtr[6] * m_points[t].z() + \
			m_matrixPtr[7] * m_points[t].w();
		if (cw != 0) {
			cx /= cw;
			cy /= cw;
		}
		painter->drawLine(px * m_scale, py * m_scale, cx * m_scale, cy * m_scale);
		px = cx;
		py = cy;
	}

	painter->setPen(prevPainterPen);
	painter->setBrush(prevPainterBrush);
}
