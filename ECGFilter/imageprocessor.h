#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QtMath>

const static QVector<int> orderedDitheringMatrix2 = {
	0, 2,
	3, 1
};

const static QVector<int> orderedDitheringMatrix4 = {
	 0,  8,  2, 10,
	12,  4, 14,  6,
	 3, 11,  1,  9,
	15,  7, 13,  5
};

class ImageProcessor {

private:

	ImageProcessor() = default;

public:
	static QImage rotateImage(
			const QImage &image,
			const int &rotationAngle
	);
	static QImage monochromeFilter(const QImage &image);
	static QImage inversionFilter(const QImage &image);
	static QImage blurFilter(
			const QImage &image,
			const int &blurFilterSize
	);
	static QImage sharpenFilter(const QImage &image);
	static QImage embossingFilter(const QImage &image);
	static QImage gammaCorrectionFilter(
			const QImage &image,
			const float &gammaValue
	);
	static QImage robertsFilter(
			const QImage &image,
			const float &binarizationThreshold,
			const QColor &firstColor,
			const QColor &secondColor
	);
	static QImage sobelFilter(
			const QImage &image,
			const float &binarizationThreshold,
			const QColor &firstColor,
			const QColor &secondColor
	);
	static QImage watercolorFilter(const QImage &image);
	static QImage pencilFilter(const QImage &image);
	static QImage floydDithering(
			const QImage &image,
			const int &redQuantization,
			const int &greenQuantization,
			const int &blueQuantization
	);
	static QImage orderedDithering(
			const QImage &image,
			const int &redQuantization,
			const int &greenQuantization,
			const int &blueQuantization
	);

private:

	static void applyKernel(
			const QImage &imageBits,
			QImage &newImageBits,
			const int &bitsWidth,
			const int &bitsHeight,
			double *kernel,
			const int &kernelSize
	);

	static int getIndexReflection(const int &i, const int &maxi, const int &mini);

	static int getNearestColorQuant(const int &color, const int &quantization);

	inline static int clamp(int x, int xmin, int xmax);

	static void makeOrderedDithering(QImage &image, const int &h, const int &w, const int *dm, const int &N, const int &rq, const int &gq, const int &bq);
};

#endif // IMAGEPROCESSOR_H
