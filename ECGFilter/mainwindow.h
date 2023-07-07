#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QSizePolicy>

#include "dialogs/showfitteddialog.h"
#include "dialogs/imagerotationdialog.h"
#include "dialogs/gammacorrectiondialog.h"
#include "dialogs/blurfilterdialog.h"
#include "dialogs/robertsfilterdialog.h"
#include "dialogs/sobelfilterdialog.h"
#include "dialogs/floydditheringdialog.h"
#include "dialogs/orderedditheringdialog.h"
#include "displayarea.h"

class MainWindow : public QMainWindow {

	Q_OBJECT

public:

	MainWindow(QWidget *parent = nullptr);

	QRect getViewportRect() const;

	void unblockFilesystemActions();
	void unblockImageModificationActions();
	void blockUndoAction();
	void blockRedoAction();
	void unblockUndoAction();
	void unblockRedoAction();

	void resetDisplayMode();

	void showImage(const QImage &image);

	void showWarning(const QString &title, const QString &text);

private:

	ShowFittedDialog *showFittedDialog;

	ImageRotationDialog *imageRotationDialog;

	BlurFilterDialog *blurFilterDialog;
	GammaCorrectionDialog *gammaCorrectionDialog;
	RobertsFilterDialog *robertsFilterDialog;
	SobelFilterDialog *sobelFilterDialog;

	FloydDitheringDialog *floydDitheringDialog;
	OrderedDitheringDialog *orderedDitheringDialog;

	QToolBar *toolbar;

	QAction *openFileAction;
	QAction *saveFileAction;

	QAction *undoAction;
	QAction *redoAction;

	QAction *showRealSizedImageAction;
	QAction *showFittedImageAction;

	QAction *rotateImageAction;

	QAction *monochromeFilterAction;
	QAction *inversionFilterAction;
	QAction *blurFilterAction;
	QAction *sharpenFilterAction;
	QAction *embossingFilterAction;
	QAction *gammaCorrectionFilterAction;
	QAction *robertsFilterAction;
	QAction *sobelFilterAction;
	QAction *watercolorFilterAction;
	QAction *pencilFilterAction;

	QAction *floydDitheringAction;
	QAction *orderedDitheringAction;

	QAction *aboutAction;

	DisplayArea *displayArea;

	void initializeInterface();

	void resizeEvent(QResizeEvent *event) override;

signals:

	void openFileActionTriggered(QString chosenFilePath);
	void saveFileActionTriggered(QString chosenFilePath);

	void undoActionTriggered();
	void redoActionTriggered();

	void showFittedImageActionTriggered(cv::InterpolationFlags interpolation);
	void showRealSizedImageActionTriggered();

	void rotateImageActionTriggered(int rotationAngle);

	void monochromeFilterActionTriggered();
	void inversionFilterActionTriggered();
	void blurFilterActionTriggered(int blurFilterSize);
	void sharpenFilterActionTriggered();
	void embossingFilterActionTriggered();
	void gammaCorrectionFilterActionTriggered(float gammaValue);
	void robertsFilterActionTriggered(float binarizationThreshold, QColor firstColor, QColor secondColor);
	void sobelFilterActionTriggered(float binarizationThreshold, QColor firstColor, QColor secondColor);
	void watercolorFilterActionTriggered();
	void pencilFilterActionTriggered();

	void floydDitheringActionTriggered(int redQuantization, int greenQuantization, int blueQuantization);
	void orderedDitheringActionTriggered(int redQuantization, int greenQuantization, int blueQuantization);

	void resized();
};

#endif // MAINWINDOW_H
