#include "imageprocessor.h"

#include <QDebug>
#include <QSet>

#include "constants.h"

QImage ImageProcessor::rotateImage(
		const QImage &image,
		const int &rotationAngle
		) {
	double rotationAngleRadians = qDegreesToRadians(rotationAngle);

	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			newImage.setPixelColor(iw, ih, QColor(
						Constants::COLOR_MAX_VALUE,
						Constants::COLOR_MAX_VALUE,
						Constants::COLOR_MAX_VALUE
			));
		}
	}

	const int imageCenterW = w / 2;
	const int imageCenterH = h / 2;

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {

			int iwShifted = iw - imageCenterW;
			int ihShifted = ih - imageCenterH;

			int iwShiftedRotated = (int)(qCos(rotationAngleRadians) * iwShifted) - (int)(qSin(rotationAngleRadians) * ihShifted);
			int ihShiftedRotated = (int)(qSin(rotationAngleRadians) * iwShifted) + (int)(qCos(rotationAngleRadians) * ihShifted);

			int iwRotated = iwShiftedRotated + imageCenterW;
			int ihRotated = ihShiftedRotated + imageCenterH;

			if (iwRotated < 0 || w <= iwRotated) {
				continue;
			}

			if (ihRotated < 0 || h <= ihRotated) {
				continue;
			}

			newImage.setPixelColor(iw, ih, image.pixelColor(iwRotated, ihRotated));
		}
	}

	return newImage;
}

QImage ImageProcessor::monochromeFilter(const QImage &image) {
	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			auto col = image.pixelColor(iw, ih);
			auto r = col.red();
			auto g = col.green();
			auto b = col.blue();
			int gray = 0.3 * r + 0.59 * g + 0.11 * b;
			newImage.setPixelColor(iw, ih, QColor(gray, gray, gray));
		}
	}

	return newImage;
}

QImage ImageProcessor::inversionFilter(const QImage &image) {
	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			auto col = image.pixelColor(iw, ih);
			auto r = col.red();
			auto g = col.green();
			auto b = col.blue();
			newImage.setPixelColor(iw, ih, QColor(
									   Constants::COLOR_MAX_VALUE - r,
									   Constants::COLOR_MAX_VALUE - g,
									   Constants::COLOR_MAX_VALUE - b
			));
		}
	}

	return newImage;
}

QImage ImageProcessor::blurFilter(
		const QImage &image,
		const int &blurFilterSize
		) {

	const int &N = blurFilterSize;

	double blurMatrix[N * N];

	if (blurFilterSize <= Constants::BLUR_FILTER_MAX_SIZE_TO_USE_GAUSS_BLUR) {
		auto gaussFunction = [](const int x, const int y) -> double {
			double xf = static_cast<double>(x);
			double yf = static_cast<double>(y);
			int s2 = Constants::BLUR_FILTER_SIGMA * Constants::BLUR_FILTER_SIGMA;
			return qExp(-(xf * xf + yf * yf) / (2 * s2)) / (2 * M_PI * s2);
		};

		// Fill

		for (int ih = 0; ih < N; ++ih) {
			for (int iw = 0; iw < N; ++iw) {
				blurMatrix[ih * N + iw] = gaussFunction(ih - N / 2, iw - N / 2);
			}
		}

		// Normalize

		double div = blurMatrix[0];
		int blurMatrixSum = 0;
		for (int ih = 0; ih < N; ++ih) {
			for (int iw = 0; iw < N; ++iw) {
				blurMatrix[ih * N + iw] = qRound(blurMatrix[ih * N + iw] / div);
				blurMatrixSum += static_cast<int>(blurMatrix[ih * N + iw]);
			}
		}


		for (int ih = 0; ih < N; ++ih) {
			for (int iw = 0; iw < N; ++iw) {
				blurMatrix[ih * N + iw] /= blurMatrixSum;
			}
		}
	}
	else {
		for (int ih = 0; ih < N; ++ih) {
			for (int iw = 0; iw < N; ++iw) {
				blurMatrix[ih * N + iw] = 1.0 / (N * N);
			}
		}
	}

	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	applyKernel(image, newImage, w, h, blurMatrix, N);

	return newImage;
}

QImage ImageProcessor::sharpenFilter(const QImage &image) {

	const int N = 5;

	double sharpenMatrix[N * N] = {
	  -1.0/8, -1.0/8, -1.0/8, -1.0/8, -1.0/8,
	  -1.0/8,  2.0/8,  2.0/8,  2.0/8, -1.0/8,
	  -1.0/8,  2.0/8,  8.0/8,  2.0/8, -1.0/8,
	  -1.0/8,  2.0/8,  2.0/8,  2.0/8, -1.0/8,
	  -1.0/8, -1.0/8, -1.0/8, -1.0/8, -1.0/8,
	};
	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	applyKernel(image, newImage, w, h, sharpenMatrix, N);

	return newImage;
}

QImage ImageProcessor::embossingFilter(const QImage &image) {

	const int N = 3;

	double embossingMatrix[N * N] = {
		0, 0, 0,
		1, 0, -1,
		0, 0, 0
	};

	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	applyKernel(image, newImage, w, h, embossingMatrix, N);

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			auto col = newImage.pixelColor(iw, ih);
			auto r = col.red();
			auto g = col.green();
			auto b = col.blue();
			newImage.setPixelColor(iw, ih, QColor(
				   clamp(r + (Constants::COLOR_MAX_VALUE + 1) / 2, 0, Constants::COLOR_MAX_VALUE),
				   clamp(g + (Constants::COLOR_MAX_VALUE + 1) / 2, 0, Constants::COLOR_MAX_VALUE),
				   clamp(b + (Constants::COLOR_MAX_VALUE + 1) / 2, 0, Constants::COLOR_MAX_VALUE)
			));
		}
	}

	return newImage;
}

QImage ImageProcessor::gammaCorrectionFilter(
		const QImage &image,
		const float &gammaValue
		) {

	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			auto col = image.pixelColor(iw, ih);
			auto r = col.red();
			auto g = col.green();
			auto b = col.blue();
			int redGamma   = Constants::COLOR_MAX_VALUE * qPow(static_cast<float>(r) / Constants::COLOR_MAX_VALUE, gammaValue);
			int greenGamma = Constants::COLOR_MAX_VALUE * qPow(static_cast<float>(g) / Constants::COLOR_MAX_VALUE, gammaValue);
			int blueGamma  = Constants::COLOR_MAX_VALUE * qPow(static_cast<float>(b) / Constants::COLOR_MAX_VALUE, gammaValue);
			newImage.setPixelColor(iw, ih, QColor(redGamma, greenGamma, blueGamma));
		}
	}

	return newImage;
}

QImage ImageProcessor::robertsFilter(
		const QImage &image,
		const float &binarizationThreshold,
		const QColor &firstColor,
		const QColor &secondColor
		) {

	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			int h1, w1, h2, w2;
			int I1, I2;
			int r1, r2, g1, g2, b1, b2;
			QColor col1, col2;

			// First gradient
			h1 = ih;
			w1 = iw;
			h2 = getIndexReflection(ih + 1, 0, h - 1);
			w2 = getIndexReflection(iw + 1, 0, w - 1);

			col1 = image.pixelColor(w1, h1);
			r1 = col1.red();
			g1 = col1.green();
			b1 = col1.blue();

			col2 = image.pixelColor(w2, h2);
			r2 = col2.red();
			g2 = col2.green();
			b2 = col2.blue();

			I1 = (r1 + g1 + b1) / 3;
			I2 = (r2 + g2 + b2) / 3;
			int G1 = I1 - I2;

			// Second gradient
			h1 = ih;
			w1 = getIndexReflection(iw + 1, 0, w - 1);
			h2 = getIndexReflection(ih + 1, 0, h - 1);
			w2 = iw;

			col1 = image.pixelColor(w1, h1);
			r1 = col1.red();
			g1 = col1.green();
			b1 = col1.blue();

			col2 = image.pixelColor(w2, h2);
			r2 = col2.red();
			g2 = col2.green();
			b2 = col2.blue();

			I1 = (r1 + g1 + b1) / 3;
			I2 = (r2 + g2 + b2) / 3;
			int G2 = I1 - I2;

			if (qAbs(G1) + qAbs(G2) > binarizationThreshold) {
				newImage.setPixelColor(iw, ih, firstColor);
			}
			else {
				newImage.setPixelColor(iw, ih, secondColor);
			}
		}
	}

	return newImage;
}

QImage ImageProcessor::sobelFilter(
		const QImage &image,
		const float &binarizationThreshold,
		const QColor &firstColor,
		const QColor &secondColor
		) {
	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	//  A | B | C
	// ---+---+---
	//  D | # | E
	// ---+---+---
	//  F | G | H

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			int ah = getIndexReflection(ih - 1, 0, h - 1);
			int aw = getIndexReflection(iw - 1, 0, w - 1);
			int bh = getIndexReflection(ih - 1, 0, h - 1);
			int bw = iw;
			int ch = getIndexReflection(ih - 1, 0, h - 1);
			int cw = getIndexReflection(iw + 1, 0, w - 1);
			int dh = ih;
			int dw = getIndexReflection(iw - 1, 0, w - 1);
			int eh = ih;
			int ew = getIndexReflection(iw + 1, 0, w - 1);
			int fh = getIndexReflection(ih + 1, 0, h - 1);
			int fw = getIndexReflection(iw - 1, 0, w - 1);
			int gh = getIndexReflection(ih + 1, 0, h - 1);
			int gw = iw;
			int hh = getIndexReflection(ih + 1, 0, h - 1);
			int hw = getIndexReflection(iw + 1, 0, w - 1);

			auto pa = image.pixelColor(aw, ah);
			auto pb = image.pixelColor(bw, bh);
			auto pc = image.pixelColor(cw, ch);
			auto pd = image.pixelColor(dw, dh);
			auto pe = image.pixelColor(ew, eh);
			auto pf = image.pixelColor(fw, fh);
			auto pg = image.pixelColor(gw, gh);
			auto ph = image.pixelColor(hw, hh);

			int aI = (pa.red() + pa.green() + pa.blue()) / 3;
			int bI = (pb.red() + pb.green() + pb.blue()) / 3;
			int cI = (pc.red() + pc.green() + pc.blue()) / 3;
			int dI = (pd.red() + pd.green() + pd.blue()) / 3;
			int eI = (pe.red() + pe.green() + pe.blue()) / 3;
			int fI = (pf.red() + pf.green() + pf.blue()) / 3;
			int gI = (pg.red() + pg.green() + pg.blue()) / 3;
			int hI = (ph.red() + ph.green() + ph.blue()) / 3;

			int Sx = (cI + 2*eI + hI) - (aI + 2 * dI + fI);
			int Sy = (fI + 2*gI + hI) - (aI + 2 * bI + cI);

			if (qAbs(Sx) + qAbs(Sy) > binarizationThreshold) {
				newImage.setPixelColor(iw, ih, firstColor);
			}
			else {
				newImage.setPixelColor(iw, ih, secondColor);
			}
		}
	}

	return newImage;
}

QImage ImageProcessor::watercolorFilter(const QImage &image) {
	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	// Apply median filter

	const int N = Constants::WATERCOLOR_FILTER_SIZE;

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			QVector<QColor> neighbors;
			for (int kr = 0; kr < N; ++kr) { // kernel row
				for (int kc = 0; kc < N; ++kc) { // kernel column
					const int rs = kr - N / 2; // row shift
					const int cs = kc - N / 2; // col shift

					int rih = getIndexReflection(ih + rs, 0, h - 1); // reflected ih
					int riw = getIndexReflection(iw + cs, 0, w - 1); // reflected iw

					neighbors.push_back(image.pixelColor(riw, rih));
				}
			}

			std::sort(neighbors.begin(), neighbors.end(), [](const QColor &c1, const QColor &c2) -> bool {
				return (c1.red() < c2.red()) ||
						(c1.red() == c2.red() && c1.green() < c2.green()) ||
						(c1.red() == c2.red() && c1.green() == c2.green() && c1.blue() < c2.blue());
			});

			newImage.setPixelColor(iw, ih, neighbors[N * N / 2 + 1]);
		}
	}

	return sharpenFilter(newImage);
}

QImage ImageProcessor::pencilFilter(const QImage &image) {
	QImage grayImage = monochromeFilter(image);
	QImage invertedGrayImage = inversionFilter(grayImage);
	QImage blurredInvertedGrayImage = blurFilter(invertedGrayImage, 11);

	const int w = image.width();
	const int h = image.height();

	QImage newImage(w, h, image.format());

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {
			int color;
			if (grayImage.pixelColor(iw, ih).red() == Constants::COLOR_MAX_VALUE) {
				color = 255;
			}
			else {
				color = clamp(
							blurredInvertedGrayImage.pixelColor(iw, ih).red() * (Constants::COLOR_MAX_VALUE) / (Constants::COLOR_MAX_VALUE - grayImage.pixelColor(iw, ih).red())	,
							0, Constants::COLOR_MAX_VALUE
				);
			}
			newImage.setPixelColor(iw, ih, QColor(color, color, color));
		}
	}

	return gammaCorrectionFilter(newImage, 2.5);
}

QImage ImageProcessor::floydDithering(
		const QImage &image,
		const int &redQuantization,
		const int &greenQuantization,
		const int &blueQuantization
		) {

	const int w = image.width();
	const int h = image.height();

	QImage newImage = image.copy();

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {

			int rq = getNearestColorQuant(newImage.pixelColor(iw, ih).red(),   redQuantization);
			int gq = getNearestColorQuant(newImage.pixelColor(iw, ih).green(), greenQuantization);
			int bq = getNearestColorQuant(newImage.pixelColor(iw, ih).blue(),  blueQuantization);

			int re = newImage.pixelColor(iw, ih).red() - rq;
			int ge = newImage.pixelColor(iw, ih).green() - gq;
			int be = newImage.pixelColor(iw, ih).blue() - bq;

			newImage.setPixelColor(iw, ih, QColor(rq, gq, bq));

			if (iw + 1 < w) {
				newImage.setPixelColor(iw + 1, ih, QColor(
							clamp(newImage.pixelColor(iw + 1, ih).red() + re * 7 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw + 1, ih).green() + ge * 7 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw + 1, ih).blue() + be * 7 / 16, 0, Constants::COLOR_MAX_VALUE)
				));
			}
			if (iw - 1 >= 0 && ih + 1 < h) {
				newImage.setPixelColor(iw - 1, ih + 1, QColor(
							clamp(newImage.pixelColor(iw - 1, ih + 1).red() + re * 3 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw - 1, ih + 1).green() + ge * 3 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw - 1, ih + 1).blue() + be * 3 / 16, 0, Constants::COLOR_MAX_VALUE)
				));
			}
			if (ih + 1 < h) {
				newImage.setPixelColor(iw, ih + 1, QColor(
							clamp(newImage.pixelColor(iw, ih + 1).red() + re * 5 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw, ih + 1).green() + ge * 5 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw, ih + 1).blue() + be * 5 / 16, 0, Constants::COLOR_MAX_VALUE)
				));
			}
			if (iw + 1 < w && ih + 1 < h) {
				newImage.setPixelColor(iw + 1, ih + 1, QColor(
							clamp(newImage.pixelColor(iw + 1, ih + 1).red() + re * 1 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw + 1, ih + 1).green() + ge * 1 / 16, 0, Constants::COLOR_MAX_VALUE),
							clamp(newImage.pixelColor(iw + 1, ih + 1).blue() + be * 1 / 16, 0, Constants::COLOR_MAX_VALUE)
				));
			}
		}
	}

	return newImage;
}

QImage ImageProcessor::orderedDithering(
		const QImage &image,
		const int &redQuantization,
		const int &greenQuantization,
		const int &blueQuantization
		) {

	const int odm2[4] = {
		0, 2,
		3, 1
	};

	const int odm4[16] = {
		 0,  8,  2, 10,
		12,  4, 14,  6,
		 3, 11,  1,  9,
		15,  7, 13,  5
	};

	const int odm8[64] = {
		 0, 32,  8, 40,  2, 34, 10, 42,
		48, 16, 56, 24, 50, 18, 58, 26,
		12, 44,  4, 36, 14, 46,  6, 38,
		60, 28, 52, 20, 62, 30, 54, 22,
		 3, 35, 11, 43,  1, 33,  9, 41,
		51, 19, 59, 27, 49, 17, 57, 25,
		15, 47,  7, 39, 13, 45,  5, 37,
		63, 31, 55, 23, 61, 29, 53, 21
	};

	const int odm16[256] =   {
		 0, 191,  48, 239,  12, 203,  60, 251,   3, 194,  51, 242,  15, 206,  63, 254,
	   127,  64, 175, 112, 139,  76, 187, 124, 130,  67, 178, 115, 142,  79, 190, 127,
		32, 223,  16, 207,  44, 235,  28, 219,  35, 226,  19, 210,  47, 238,  31, 222,
	   159,  96, 143,  80, 171, 108, 155,  92, 162,  99, 146,  83, 174, 111, 158,  95,
		 8, 199,  56, 247,   4, 195,  52, 243,  11, 202,  59, 250,   7, 198,  55, 246,
	   135,  72, 183, 120, 131,  68, 179, 116, 138,  75, 186, 123, 134,  71, 182, 119,
		40, 231,  24, 215,  36, 227,  20, 211,  43, 234,  27, 218,  39, 230,  23, 214,
	   167, 104, 151,  88, 163, 100, 147,  84, 170, 107, 154,  91, 166, 103, 150,  87,
		 2, 193,  50, 241,  14, 205,  62, 253,   1, 192,  49, 240,  13, 204,  61, 252,
	   129,  66, 177, 114, 141,  78, 189, 126, 128,  65, 176, 113, 140,  77, 188, 125,
		34, 225,  18, 209,  46, 237,  30, 221,  33, 224,  17, 208,  45, 236,  29, 220,
	   161,  98, 145,  82, 173, 110, 157,  94, 160,  97, 144,  81, 172, 109, 156,  93,
		10, 201,  58, 249,   6, 197,  54, 245,   9, 200,  57, 248,   5, 196,  53, 244,
	   137,  74, 185, 122, 133,  70, 181, 118, 136,  73, 184, 121, 132,  69, 180, 117,
		42, 233,  26, 217,  38, 229,  22, 213,  41, 232,  25, 216,  37, 228,  21, 212,
	   169, 106, 153,  90, 165, 102, 149,  86, 168, 105, 152,  89, 164, 101, 148,  85
	};

	const int w = image.width();
	const int h = image.height();

	QImage newImage = image.copy();

	int minQuantization = qMin(redQuantization, qMin(greenQuantization, blueQuantization));

	//makeOrderedDithering(newImage, h, w, odm4, 4, redQuantization, greenQuantization, blueQuantization);

	if (minQuantization <= Constants::ORDERED_DITHERING_16X16_MATRIX_THRESHOLD) {
		makeOrderedDithering(newImage, h, w, odm16, 16, redQuantization, greenQuantization, blueQuantization);
	}
	else if (
			 Constants::ORDERED_DITHERING_16X16_MATRIX_THRESHOLD < minQuantization &&
			 minQuantization <= Constants::ORDERED_DITHERING_8X8_MATRIX_THRESHOLD
	) {
		makeOrderedDithering(newImage, h, w, odm8, 8, redQuantization, greenQuantization, blueQuantization);
	}
	else if (
			 Constants::ORDERED_DITHERING_8X8_MATRIX_THRESHOLD < minQuantization &&
			 minQuantization <= Constants::ORDERED_DITHERING_4X4_MATRIX_THRESHOLD
	) {
		makeOrderedDithering(newImage, h, w, odm4, 4, redQuantization, greenQuantization, blueQuantization);
	}
	else {
		makeOrderedDithering(newImage, h, w, odm2, 2, redQuantization, greenQuantization, blueQuantization);
	}

	return newImage;
}

// ###############
// #   PRIVATE   #
// ###############

void ImageProcessor::applyKernel(
		const QImage &image,
		QImage &newImage,
		const int &bitsWidth,
		const int &bitsHeight,
		double *kernel,
		const int &kernelSize
) {
	for (int ih = 0; ih < bitsHeight; ++ih) {
		for (int iw = 0; iw < bitsWidth; ++iw) {
			double red = 0;
			double green = 0;
			double blue = 0;
			for (int kr = 0; kr < kernelSize; ++kr) { // kernel row
				for (int kc = 0; kc < kernelSize; ++kc) { // kernel column
					const int rs = kr - kernelSize / 2; // row shift
					const int cs = kc - kernelSize / 2; // col shift

					int rih = getIndexReflection(ih + rs, 0, bitsHeight - 1); // reflected ih
					int riw = getIndexReflection(iw + cs, 0, bitsWidth - 1); // reflected iw

					red   += kernel[kr * kernelSize + kc] * image.pixelColor(riw, rih).red();
					green += kernel[kr * kernelSize + kc] * image.pixelColor(riw, rih).green();
					blue  += kernel[kr * kernelSize + kc] * image.pixelColor(riw, rih).blue();
				}
			}

			newImage.setPixelColor(iw, ih, QColor(
						clamp(  static_cast<int>(red), 0, Constants::COLOR_MAX_VALUE),
						clamp(static_cast<int>(green), 0, Constants::COLOR_MAX_VALUE),
						clamp( static_cast<int>(blue), 0, Constants::COLOR_MAX_VALUE)
									   )
			);
		}
	}
}

int ImageProcessor::getIndexReflection(const int &i, const int &mini, const int &maxi) {
	if (mini <= i && i <= maxi) {
		return i;
	}
	else if (i < mini) {
		return mini + (mini - i) - 1;
	}
	// i > maxi
	return maxi - (i - maxi) + 1;
}

int ImageProcessor::getNearestColorQuant(const int &color, const int &quantization) {
	int quantSize = (Constants::COLOR_MAX_VALUE + 1) / (quantization - 1);
	int ql = color / quantSize;
	int qr = (color + quantSize) / quantSize;
	if (color - ql * quantSize <= qr * quantSize - color) {
		return clamp(ql * quantSize, 0, Constants::COLOR_MAX_VALUE);
	}
	return clamp(qr * quantSize, 0, Constants::COLOR_MAX_VALUE);
}

inline int ImageProcessor::clamp(int x, int xmin, int xmax) {
	return qMin(qMax(xmin, x), xmax);
}

void ImageProcessor::makeOrderedDithering(QImage &image, const int &h, const int &w, const int *dm, const int &N, const int &rq, const int &gq, const int &bq) {

	for (int ih = 0; ih < h; ++ih) {
		for (int iw = 0; iw < w; ++iw) {

			int ra = (Constants::COLOR_MAX_VALUE) * (2 * dm[(ih % N) * N + (iw % N)] - N * N) / (2 * N * N * (rq - 1));
			int ga = (Constants::COLOR_MAX_VALUE) * (2 * dm[(ih % N) * N + (iw % N)] - N * N) / (2 * N * N * (gq - 1));
			int ba = (Constants::COLOR_MAX_VALUE) * (2 * dm[(ih % N) * N + (iw % N)] - N * N) / (2 * N * N * (bq - 1));

			image.setPixelColor(iw, ih, QColor(
						getNearestColorQuant(image.pixelColor(iw, ih).red()   + ra, rq),
						getNearestColorQuant(image.pixelColor(iw, ih).green() + ga, gq),
						getNearestColorQuant(image.pixelColor(iw, ih).blue()  + ba, bq)
			));
		}
	}
}


































