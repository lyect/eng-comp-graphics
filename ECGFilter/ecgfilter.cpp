#include "ecgfilter.h"

#include "imageprocessor.h"

cv::Mat QImageToMat(const QImage & image){
	cv::Mat out;
	switch(image.format()) {

	case QImage::Format_Invalid:
	{
		cv::Mat empty;
		empty.copyTo(out);
		break;
	}
	case QImage::Format_RGB32:
	{
		cv::Mat view(image.height(),image.width(),CV_8UC4,(void *)image.constBits(),image.bytesPerLine());
		view.copyTo(out);
		break;
	}
	case QImage::Format_RGB888:
	{
		cv::Mat view(image.height(),image.width(),CV_8UC3,(void *)image.constBits(),image.bytesPerLine());
		cv::cvtColor(view, out, cv::COLOR_RGB2BGR);
		break;
	}
	default:
	{
		QImage conv = image.convertToFormat(QImage::Format_RGB32);
		cv::Mat view(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(),conv.bytesPerLine());
		view.copyTo(out);
		break;
	}
	}
	return out;
}

QImage cvMatToQImage( const cv::Mat &inMat )
   {
	  switch ( inMat.type() )
	  {
		 // 8-bit, 4 channel
		 case CV_8UC4:
		 {
			QImage image( inMat.data,
						  inMat.cols, inMat.rows,
						  static_cast<int>(inMat.step),
						  QImage::Format_RGB32 );
			return image;
		 }

		 // 8-bit, 3 channel
		 case CV_8UC3:
		 {
			QImage image( inMat.data,
						  inMat.cols, inMat.rows,
						  static_cast<int>(inMat.step),
						  QImage::Format_RGB888 );
			return image.rgbSwapped();
		 }

		 // 8-bit, 1 channel
		 case CV_8UC1:
		 {
			QImage image( inMat.data,
						  inMat.cols, inMat.rows,
						  static_cast<int>(inMat.step),
						  QImage::Format_Grayscale8 );
			return image;
		 }

		 default:
			qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
			break;
	  }

	  return QImage();
   }

ECGFilter::ECGFilter(int &argc, char *argv[]) : QApplication(argc, argv) {

	currentState = -1;
	displayMode = Constants::DisplayMode::REAL_SIZED;

	connectObjects();
}

int ECGFilter::exec() {

	mainWindow.show();

	return QApplication::exec();
}

// ###############
// #   PRIVATE   #
// ###############

void ECGFilter::connectObjects() {

	QObject::connect(&mainWindow, &MainWindow::openFileActionTriggered, this, &ECGFilter::onOpenFileActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::saveFileActionTriggered, this, &ECGFilter::onSaveFileActionTriggered);

	QObject::connect(&mainWindow, &MainWindow::undoActionTriggered, this, &ECGFilter::onUndoActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::redoActionTriggered, this, &ECGFilter::onRedoActionTriggered);

	QObject::connect(&mainWindow, &MainWindow::showRealSizedImageActionTriggered, this, &ECGFilter::onShowRealSizedImageActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::showFittedImageActionTriggered, this, &ECGFilter::onShowFittedImageActionTriggered);

	QObject::connect(&mainWindow, &MainWindow::rotateImageActionTriggered, this, &ECGFilter::onRotateImageActionTriggered);

	QObject::connect(&mainWindow, &MainWindow::monochromeFilterActionTriggered, this, &ECGFilter::onMonochromeFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::inversionFilterActionTriggered, this, &ECGFilter::onInversionFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::blurFilterActionTriggered, this, &ECGFilter::onBlurFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::sharpenFilterActionTriggered, this, &ECGFilter::onSharpenFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::embossingFilterActionTriggered, this, &ECGFilter::onEmbossingFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::gammaCorrectionFilterActionTriggered, this, &ECGFilter::onGammaCorrectionFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::robertsFilterActionTriggered, this, &ECGFilter::onRobertsFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::sobelFilterActionTriggered, this, &ECGFilter::onSobelFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::watercolorFilterActionTriggered, this, &ECGFilter::onWatercolorFilterActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::pencilFilterActionTriggered, this, &ECGFilter::onPencilFilterActionTriggered);

	QObject::connect(&mainWindow, &MainWindow::floydDitheringActionTriggered, this, &ECGFilter::onFloydDitheringActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::orderedDitheringActionTriggered, this, &ECGFilter::onOrderedDitheringActionTriggered);

	QObject::connect(&mainWindow, &MainWindow::resized, this, &ECGFilter::onMainWindowResized);
}

void ECGFilter::addState(const QImage &image) {
	if (currentState < states.count() - 1) {
		states.remove(currentState + 1, states.count() - currentState - 1);
	}

	states.push_back(image);
	++currentState;

	mainWindow.blockRedoAction();
	if (currentState == 0) {
		mainWindow.blockUndoAction();
	}
	else {
		mainWindow.unblockUndoAction();
	}

	if (displayMode == Constants::DisplayMode::FITTED) {
		mainWindow.showImage(interpolate(states[currentState]));
	}
	else {
		mainWindow.showImage(states[currentState]);
	}
}

QImage ECGFilter::interpolate(const QImage &image) {

	cv::Mat mat = QImageToMat(image);

	QRect vr = mainWindow.getViewportRect();

	double scale = qMin((double)vr.height() / image.height(), (double)vr.width() / image.width());

	int newHeight = image.height() * scale;
	int newWidth = image.width() * scale;

	cv::Mat newMat(newHeight, newWidth, CV_8UC3);

	cv::resize(mat, newMat, cv::Size(newWidth, newHeight), 0, 0, currentInterpolation);

	return cvMatToQImage(newMat);
}

// #####################
// #   PRIVATE SLOTS   #
// #####################

void ECGFilter::onOpenFileActionTriggered(QString chosenFilePath) {
	QImage image;

	if (chosenFilePath.isNull()) {
		return;
	}

	bool loadStatus = image.load(chosenFilePath);

	if (!loadStatus) {
		mainWindow.showWarning("Warning", "Failed to open file!");
		return;
	}

	if (image.width() < 8 || image.height() < 8) {
		mainWindow.showWarning("Warning", "Image must be at least 8x8 pixels in size!");
		return;
	}

	displayMode = Constants::DisplayMode::REAL_SIZED;
	image = image.convertToFormat(QImage::Format_RGB888);

	mainWindow.unblockFilesystemActions();
	mainWindow.unblockImageModificationActions();
	mainWindow.resetDisplayMode();

	currentState = -1;
	addState(image);
}

void ECGFilter::onSaveFileActionTriggered(QString chosenFilePath) {
	const QImage &image = states.last();

	if (chosenFilePath.isNull()) {
		return;
	}

	bool saveStatus = image.save(chosenFilePath, "png");

	if (!saveStatus) {
		mainWindow.showWarning("Warning", "Failed to save file!");
		return;
	}
}

void ECGFilter::onUndoActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	--currentState;

	if (currentState == 0) {
		mainWindow.blockUndoAction();
	}
	mainWindow.unblockRedoAction();

	if (displayMode == Constants::DisplayMode::FITTED) {
		mainWindow.showImage(interpolate(states[currentState]));
	}
	else {
		mainWindow.showImage(states[currentState]);
	}
	restoreOverrideCursor();
}

void ECGFilter::onRedoActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	++currentState;

	if (currentState == states.count() - 1) {
		mainWindow.blockRedoAction();
	}
	mainWindow.unblockUndoAction();

	if (displayMode == Constants::DisplayMode::FITTED) {
		mainWindow.showImage(interpolate(states[currentState]));
	}
	else {
		mainWindow.showImage(states[currentState]);
	}
	restoreOverrideCursor();
}

void ECGFilter::onShowRealSizedImageActionTriggered() {
	displayMode = Constants::DisplayMode::REAL_SIZED;
	mainWindow.showImage(states[currentState]);
}

void ECGFilter::onShowFittedImageActionTriggered(cv::InterpolationFlags interpolation) {
	displayMode = Constants::DisplayMode::FITTED;
	currentInterpolation = interpolation;
	mainWindow.showImage(interpolate(states[currentState]));
}

void ECGFilter::onRotateImageActionTriggered(int rotationAngle) {
	setOverrideCursor(Qt::WaitCursor);
	QImage rotatedImage = ImageProcessor::rotateImage(states[currentState], rotationAngle);
	addState(rotatedImage);
	restoreOverrideCursor();
}

void ECGFilter::onMonochromeFilterActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	QImage monochromeImage = ImageProcessor::monochromeFilter(states[currentState]);
	addState(monochromeImage);
	restoreOverrideCursor();
}

void ECGFilter::onInversionFilterActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	QImage invertedImage = ImageProcessor::inversionFilter(states[currentState]);
	addState(invertedImage);
	restoreOverrideCursor();
}

void ECGFilter::onBlurFilterActionTriggered(int blurFilterSize) {
	setOverrideCursor(Qt::WaitCursor);
	QImage blurFilteredImage = ImageProcessor::blurFilter(states[currentState], blurFilterSize);
	addState(blurFilteredImage);
	restoreOverrideCursor();
}

void ECGFilter::onSharpenFilterActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	QImage sharpenedImage = ImageProcessor::sharpenFilter(states[currentState]);
	addState(sharpenedImage);
	restoreOverrideCursor();
}

void ECGFilter::onEmbossingFilterActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	QImage embossedImage = ImageProcessor::embossingFilter(states[currentState]);
	addState(embossedImage);
	restoreOverrideCursor();
}

void ECGFilter::onGammaCorrectionFilterActionTriggered(float gammaValue) {
	setOverrideCursor(Qt::WaitCursor);
	QImage gammaCorrectedImage = ImageProcessor::gammaCorrectionFilter(states[currentState], gammaValue);
	addState(gammaCorrectedImage);
	restoreOverrideCursor();
}

void ECGFilter::onRobertsFilterActionTriggered(float binarizationThreshold, QColor firstColor, QColor secondColor) {
	setOverrideCursor(Qt::WaitCursor);
	QImage robertsFilteredImage = ImageProcessor::robertsFilter(states[currentState], binarizationThreshold, firstColor, secondColor);
	addState(robertsFilteredImage);
	restoreOverrideCursor();
}

void ECGFilter::onSobelFilterActionTriggered(float binarizationThreshold, QColor firstColor, QColor secondColor) {
	setOverrideCursor(Qt::WaitCursor);
	QImage sobelFilteredImage = ImageProcessor::sobelFilter(states[currentState], binarizationThreshold, firstColor, secondColor);
	addState(sobelFilteredImage);
	restoreOverrideCursor();
}

void ECGFilter::onWatercolorFilterActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	QImage watercolorFilteredImage = ImageProcessor::watercolorFilter(states[currentState]);
	addState(watercolorFilteredImage);
	restoreOverrideCursor();
}

void ECGFilter::onPencilFilterActionTriggered() {
	setOverrideCursor(Qt::WaitCursor);
	QImage pencilFilteredImage = ImageProcessor::pencilFilter(states[currentState]);
	addState(pencilFilteredImage);
	restoreOverrideCursor();
}


void ECGFilter::onFloydDitheringActionTriggered(int redQuantization, int greenQuantization, int blueQuantization) {
	setOverrideCursor(Qt::WaitCursor);
	QImage floydDitheredImage = ImageProcessor::floydDithering(states[currentState], redQuantization, greenQuantization, blueQuantization);
	addState(floydDitheredImage);
	restoreOverrideCursor();
}

void ECGFilter::onOrderedDitheringActionTriggered(int redQuantization, int greenQuantization, int blueQuantization) {
	setOverrideCursor(Qt::WaitCursor);
	QImage orderedDitheredImage = ImageProcessor::orderedDithering(states[currentState], redQuantization, greenQuantization, blueQuantization);
	addState(orderedDitheredImage);
	restoreOverrideCursor();
}

void ECGFilter::onMainWindowResized() {
	if (displayMode == Constants::DisplayMode::FITTED) {
		mainWindow.showImage(interpolate(states[currentState]));
	}
}
