#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "constants.h"

// ##############
// #   PUBLIC   #
// ##############

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) {

	setMinimumWidth(Constants::MIN_WINDOW_WIDTH);
	setMinimumHeight(Constants::MIN_WINDOW_HEIGHT);

	resize(
			Constants::DEFAULT_WINDOW_WIDTH,
			Constants::DEFAULT_WINDOW_HEIGHT
	);

	setWindowIcon(QIcon(":/resources/icons/window/icon_128.png"));
	setWindowTitle("ECGFilter");

	initializeInterface();
}

QRect MainWindow::getViewportRect() const {
	return displayArea->getViewportRect();
}

void MainWindow::unblockFilesystemActions() {
	openFileAction->setEnabled(true);
	saveFileAction->setEnabled(true);
}

void MainWindow::unblockImageModificationActions() {
	rotateImageAction->setEnabled(true);

	monochromeFilterAction->setEnabled(true);
	inversionFilterAction->setEnabled(true);
	blurFilterAction->setEnabled(true);
	sharpenFilterAction->setEnabled(true);
	embossingFilterAction->setEnabled(true);
	gammaCorrectionFilterAction->setEnabled(true);
	robertsFilterAction->setEnabled(true);
	sobelFilterAction->setEnabled(true);
	watercolorFilterAction->setEnabled(true);
	pencilFilterAction->setEnabled(true);

	floydDitheringAction->setEnabled(true);
	orderedDitheringAction->setEnabled(true);
}

void MainWindow::blockUndoAction() {
	undoAction->setEnabled(false);
}

void MainWindow::blockRedoAction() {
	redoAction->setEnabled(false);
}

void MainWindow::unblockUndoAction() {
	undoAction->setEnabled(true);
}

void MainWindow::unblockRedoAction() {
	redoAction->setEnabled(true);
}

void MainWindow::resetDisplayMode() {
	//displayArea->setRealSizedDisplayMode();
	showRealSizedImageAction->setEnabled(false);
	showFittedImageAction->setEnabled(true);
}

void MainWindow::showImage(const QImage &image) {
	displayArea->drawImage(image);
}

void MainWindow::showWarning(const QString &title, const QString &text) {
	QMessageBox::warning(this, title, text);
}

// ###############
// #   PRIVATE   #
// ###############

void MainWindow::initializeInterface() {

	// ========== INITIALIZE DIALOGS ==============================================================

	showFittedDialog = new ShowFittedDialog(this);

	imageRotationDialog = new ImageRotationDialog(this);

	blurFilterDialog = new BlurFilterDialog(this);
	gammaCorrectionDialog = new GammaCorrectionDialog(this);
	robertsFilterDialog = new RobertsFilterDialog(this);
	sobelFilterDialog = new SobelFilterDialog(this);

	floydDitheringDialog = new FloydDitheringDialog(this);
	orderedDitheringDialog = new OrderedDitheringDialog(this);

	// ========== INITIALIZE ACTIONS ==============================================================

	// FILESYSTEM

	openFileAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filesystem/open_file_icon.png")), "Open file", this
	);
	saveFileAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filesystem/save_file_icon.png")), "Save file", this
	);

	// STATES

	undoAction = new QAction(
				QIcon(QPixmap(":/resources/icons/states/undo_icon.png")), "Undo", this
	);
	redoAction = new QAction(
				QIcon(QPixmap(":/resources/icons/states/redo_icon.png")), "Redo", this
	);

	// DISPLAY MODES

	showRealSizedImageAction = new QAction(
				QIcon(QPixmap(":/resources/icons/displaymodes/show_real_sized_image_icon.png")), "Real size", this
	);
	showFittedImageAction = new QAction(
				QIcon(QPixmap(":/resources/icons/displaymodes/show_fitted_image_icon.png")), "Fit to the screen", this
	);

	// OTHER ACTIONS

	rotateImageAction = new QAction(
				QIcon(QPixmap(":/resources/icons/other/rotate_image_icon.png")), "Rotate", this
	);

	// FILTERS

	monochromeFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/monochrome_filter_icon.png")), "Monochrome filter", this
	);
	inversionFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/inversion_filter_icon.png")), "Inversion filter", this
	);
	blurFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/blur_filter_icon.png")), "Blur filter", this
	);
	sharpenFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/sharpen_filter_icon.png")), "Sharpen filter", this
	);
	embossingFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/embossing_filter_icon.png")), "Embossing filter", this
	);
	gammaCorrectionFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/gamma_correction_filter_icon.png")), "Gamma-correction filter", this
	);
	robertsFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/roberts_filter_icon.png")), "Roberts filter", this
	);
	sobelFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/sobel_filter_icon.png")), "Sobel filter", this
	);
	watercolorFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/watercolor_filter_icon.png")), "Watercolor filter", this
	);
	pencilFilterAction = new QAction(
				QIcon(QPixmap(":/resources/icons/filters/pencil_filter_icon.png")), "Pencil filter", this
	);

	// DITHERING

	floydDitheringAction = new QAction(
				QIcon(QPixmap(":/resources/icons/dithering/floyd_dithering_icon.png")), "Floyd dithering", this
	);
	orderedDitheringAction = new QAction(
				QIcon(QPixmap(":/resources/icons/dithering/ordered_dithering_icon.png")), "Ordered dithering", this
	);

	// NOT IMAGE-TRANSFORMING ACTIONS

	aboutAction = new QAction("&About", this);

	// ========== CONNECT ACTIONS =================================================================

	// FILESYSTEM

	QObject::connect(openFileAction, &QAction::triggered, this, [this]() -> void {
		QString filename = QFileDialog::getOpenFileName(this, "Open File", "/home", "Images (*.png *jpg *.jpeg *.gif *.bmp)");

		if (filename.isNull()) {
			return;
		}

		emit openFileActionTriggered(filename);
	});
	QObject::connect(saveFileAction, &QAction::triggered, this, [this]() -> void {
		QString filename = QFileDialog::getSaveFileName(this, "Save file", "/home/untitled.png", "Images (*.png)");

		if (filename.isNull()) {
			return;
		}

		emit saveFileActionTriggered(filename);
	});

	// STATES

	QObject::connect(undoAction, &QAction::triggered, this, [this]() -> void {
		emit undoActionTriggered();
	});
	QObject::connect(redoAction, &QAction::triggered, this, [this]() -> void {
		emit redoActionTriggered();
	});

	// DISPLAY MODES

	QObject::connect(showRealSizedImageAction, &QAction::triggered, this, [this]() -> void {
		showRealSizedImageAction->setEnabled(false);
		showFittedImageAction->setEnabled(true);
		emit showRealSizedImageActionTriggered();
	});
	QObject::connect(showFittedImageAction, &QAction::triggered, this, [this]() -> void {
		if (showFittedDialog->exec() != QDialog::Accepted) {
			return;
		}
		cv::InterpolationFlags interpolation = showFittedDialog->getChosenInterpolation();
		showRealSizedImageAction->setEnabled(true);
		showFittedImageAction->setEnabled(true);
		emit showFittedImageActionTriggered(interpolation);
	});

	// OTHER ACTIONS

	QObject::connect(rotateImageAction, &QAction::triggered, this, [this]() -> void {
		if (imageRotationDialog->exec() != QDialog::Accepted) {
			return;
		}
		int rotationAngle = imageRotationDialog->getChosenRotationAngle();
		emit rotateImageActionTriggered(rotationAngle);
	});

	// FILTERS

	QObject::connect(monochromeFilterAction, &QAction::triggered, this, [this]() -> void {
		emit monochromeFilterActionTriggered();
	});
	QObject::connect(inversionFilterAction, &QAction::triggered, this, [this]() -> void {
		emit inversionFilterActionTriggered();
	});
	QObject::connect(blurFilterAction, &QAction::triggered, this, [this]() -> void {
		if (blurFilterDialog->exec() != QDialog::Accepted) {
			return;
		}
		int blurFilterSize = blurFilterDialog->getChosenSize();
		emit blurFilterActionTriggered(blurFilterSize);
	});
	QObject::connect(sharpenFilterAction, &QAction::triggered, this, [this]() -> void {
		emit sharpenFilterActionTriggered();
	});
	QObject::connect(embossingFilterAction, &QAction::triggered, this, [this]() -> void {
		emit embossingFilterActionTriggered();
	});
	QObject::connect(gammaCorrectionFilterAction, &QAction::triggered, this, [this]() -> void {
		if (gammaCorrectionDialog->exec() != QDialog::Accepted) {
			return;
		}
		float gammaValue = gammaCorrectionDialog->getChosenGammaValue();
		emit gammaCorrectionFilterActionTriggered(gammaValue);
	});
	QObject::connect(robertsFilterAction, &QAction::triggered, this, [this]() -> void {
		if (robertsFilterDialog->exec() != QDialog::Accepted) {
			return;
		}
		float binarizationThreshold = robertsFilterDialog->getBinarizationThreshold();
		auto [firstColor, secondColor] = robertsFilterDialog->getChosenColors();
		emit robertsFilterActionTriggered(binarizationThreshold, firstColor, secondColor);
	});
	QObject::connect(sobelFilterAction, &QAction::triggered, this, [this]() -> void {
		if (sobelFilterDialog->exec() != QDialog::Accepted) {
			return;
		}
		float binarizationThreshold = robertsFilterDialog->getBinarizationThreshold();
		auto [firstColor, secondColor] = sobelFilterDialog->getChosenColors();
		emit sobelFilterActionTriggered(binarizationThreshold, firstColor, secondColor);
	});
	QObject::connect(watercolorFilterAction, &QAction::triggered, this, [this]() -> void {
		emit watercolorFilterActionTriggered();
	});
	QObject::connect(pencilFilterAction, &QAction::triggered, this, [this]() -> void {
		emit pencilFilterActionTriggered();
	});

	// DITHERING

	QObject::connect(floydDitheringAction, &QAction::triggered, this, [this]() -> void {
		if (floydDitheringDialog->exec() != QDialog::Accepted) {
			return;
		}
		auto [redQuantization, greenQuantization, blueQuantization] = floydDitheringDialog->getChosenQuantization();
		emit floydDitheringActionTriggered(redQuantization, greenQuantization, blueQuantization);
	});
	QObject::connect(orderedDitheringAction, &QAction::triggered, this, [this]() -> void {
		if (orderedDitheringDialog->exec() != QDialog::Accepted) {
			return;
		}
		auto [redQuantization, greenQuantization, blueQuantization] = orderedDitheringDialog->getChosenQuantization();
		emit orderedDitheringActionTriggered(redQuantization, greenQuantization, blueQuantization);
	});

	// ABOUT

	QObject::connect(aboutAction, &QAction::triggered, this, [this]() -> void {
		QMessageBox::about(
					this, "About",
					"The second practical work on the subject \"Computer graphics\".\n"
					"Done by Timur Letyagin (FIT NSU).\n"
					"March 2023");
	});


	// ========== TOOLBAR INITIALIZING ============================================================

	toolbar = addToolBar("Enable ToolBar");

	toolbar->addAction(openFileAction);
	toolbar->addAction(saveFileAction);

	toolbar->addSeparator();

	toolbar->addAction(undoAction);
	toolbar->addAction(redoAction);

	toolbar->addSeparator();

	toolbar->addAction(showRealSizedImageAction);
	toolbar->addAction(showFittedImageAction);

	toolbar->addSeparator();

	toolbar->addAction(rotateImageAction);

	toolbar->addSeparator();

	toolbar->addAction(monochromeFilterAction);
	toolbar->addAction(inversionFilterAction);
	toolbar->addAction(blurFilterAction);
	toolbar->addAction(sharpenFilterAction);
	toolbar->addAction(embossingFilterAction);
	toolbar->addAction(gammaCorrectionFilterAction);
	toolbar->addAction(robertsFilterAction);
	toolbar->addAction(sobelFilterAction);
	toolbar->addAction(watercolorFilterAction);
	toolbar->addAction(pencilFilterAction);

	toolbar->addSeparator();

	toolbar->addAction(floydDitheringAction);
	toolbar->addAction(orderedDitheringAction);

	toolbar->addSeparator();

	toolbar->addAction(aboutAction);

	// ========== MENU INITIALIZING ===============================================================

	QMenu *fileMenu = menuBar()->addMenu("&File");
	QMenu *editMenu = menuBar()->addMenu("&Edit");
	QMenu *viewMenu = menuBar()->addMenu("&View");
	QMenu *filterMenu = menuBar()->addMenu("&Filter");
	QMenu *ditheringMenu = menuBar()->addMenu("&Dithering");

	fileMenu->addAction(openFileAction);
	fileMenu->addAction(saveFileAction);

	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addAction(rotateImageAction);

	viewMenu->addAction(showRealSizedImageAction);
	viewMenu->addAction(showFittedImageAction);

	filterMenu->addAction(monochromeFilterAction);
	filterMenu->addAction(inversionFilterAction);
	filterMenu->addAction(blurFilterAction);
	filterMenu->addAction(sharpenFilterAction);
	filterMenu->addAction(embossingFilterAction);
	filterMenu->addAction(gammaCorrectionFilterAction);
	filterMenu->addAction(robertsFilterAction);
	filterMenu->addAction(sobelFilterAction);
	filterMenu->addAction(watercolorFilterAction);
	filterMenu->addAction(pencilFilterAction);

	ditheringMenu->addAction(floydDitheringAction);
	ditheringMenu->addAction(orderedDitheringAction);

	menuBar()->addAction(aboutAction);

	// ========== ACTION BLOCKING =================================================================

	saveFileAction->setEnabled(false);

	undoAction->setEnabled(false);
	redoAction->setEnabled(false);

	showRealSizedImageAction->setEnabled(false);
	showFittedImageAction->setEnabled(false);

	rotateImageAction->setEnabled(false);

	monochromeFilterAction->setEnabled(false);
	inversionFilterAction->setEnabled(false);
	blurFilterAction->setEnabled(false);
	sharpenFilterAction->setEnabled(false);
	embossingFilterAction->setEnabled(false);
	gammaCorrectionFilterAction->setEnabled(false);
	robertsFilterAction->setEnabled(false);
	sobelFilterAction->setEnabled(false);
	watercolorFilterAction->setEnabled(false);
	pencilFilterAction->setEnabled(false);

	floydDitheringAction->setEnabled(false);
	orderedDitheringAction->setEnabled(false);

	// ========== DISPLAY AREA ====================================================================

	displayArea = new DisplayArea(this);
	displayArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

	setCentralWidget(displayArea);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	emit resized();
}
