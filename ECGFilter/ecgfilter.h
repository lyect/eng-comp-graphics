#ifndef ECGFILTER_H
#define ECGFILTER_H

#include <QApplication>

#include "mainwindow.h"
#include "constants.h"

class ECGFilter : public QApplication {

	Q_OBJECT

public:

	ECGFilter(int &argc, char *argv[]);

	int exec();

private:

	MainWindow mainWindow;
	QVector<QImage> states;
	qsizetype currentState;

	cv::InterpolationFlags currentInterpolation;
	Constants::DisplayMode displayMode;

	void connectObjects();
	void addState(const QImage &image);

	QImage interpolate(const QImage &image);

private slots:

	void onOpenFileActionTriggered(QString chosenFilePath);
	void onSaveFileActionTriggered(QString chosenFilePath);

	void onUndoActionTriggered();
	void onRedoActionTriggered();

	void onShowRealSizedImageActionTriggered();
	void onShowFittedImageActionTriggered(cv::InterpolationFlags interpolation);

	void onRotateImageActionTriggered(int rotationAngle);

	void onMonochromeFilterActionTriggered();
	void onInversionFilterActionTriggered();
	void onBlurFilterActionTriggered(int blurFilterSize);
	void onSharpenFilterActionTriggered();
	void onEmbossingFilterActionTriggered();
	void onGammaCorrectionFilterActionTriggered(float gammaValue);
	void onRobertsFilterActionTriggered(float binarizationThreshold, QColor firstColor, QColor secondColor);
	void onSobelFilterActionTriggered(float binarizationThreshold, QColor firstColor, QColor secondColor);
	void onWatercolorFilterActionTriggered();
	void onPencilFilterActionTriggered();

	void onFloydDitheringActionTriggered(int redQuantization, int greenQuantization, int blueQuantization);
	void onOrderedDitheringActionTriggered(int redQuantization, int greenQuantization, int blueQuantization);

	void onMainWindowResized();
};

#endif // ECGFILTER_H
