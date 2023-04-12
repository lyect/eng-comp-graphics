#include "mainwindow.h"

// ##############
// #   PUBLIC   #
// ##############

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent) {

	// INTERACTION POINT

	Settings &settings = Settings::getInstance();

	// Set minimum window size
	setMinimumWidth(settings.getMinWindowWidth());
	setMinimumHeight(settings.getMinWindowHeight());

	// Resize window to its defaults
	resize(settings.getDefaultWindowWidth(), settings.getDefaultWindowHeight());

	// Set title and icon
	setWindowIcon(QIcon(":/icon/icon_128.ico"));
	setWindowTitle("ECGPaint");

	// Initialize interface of the application
	initializeInterface();

	// Create paint area
	paintArea = new PaintArea(
			settings.getDefaultPaintAreaWidth(),
			settings.getDefaultPaintAreaHeight(),
			settings.getBackgroundColor()
	);
	setCentralWidget(paintArea);

	// Initialize user's data, selected tool, selected color, etc.
	selectedColor = settings.getBackgroundColor();
	selectedTool = ToolType::NO_TOOL;
}

// ###############
// #   PRIVATE   #
// ###############

void MainWindow::initializeInterface() {

	Settings &settings = Settings::getInstance();

	// ================================ DIALOG INITIALIZING ================================

	resizeDialog = new ResizeDialog(this);
	lineDialog = new LineDialog(this);
	polygonDialog = new PolygonDialog(this);
	starDialog = new StarDialog(this);

	QObject::connect(resizeDialog, &ResizeDialog::accepted, this, [this](){
		paintArea->resize(
					resizeDialog->selectedWidth(),
					resizeDialog->selectedHeight()
		);
	});

	// ================================ ACTION INITIALIZING ================================

	QAction *openFileAction = new QAction(
				QIcon(QPixmap(":/toolbar/filesystem/open_file_icon.png")), "Open file", this
	);
	QAction *saveFileAction = new QAction(
				QIcon(QPixmap(":/toolbar/filesystem/save_file_icon.png")), "Save file", this
	);

	QAction *resizeToolAction = new QAction(
				QIcon(QPixmap(":/toolbar/tools/resize_icon.png")), "Resize", this
	);
	QAction *lineToolAction = new QAction(
				QIcon(QPixmap(":/toolbar/tools/line_icon.png")), "Line tool", this
	);
	QAction *polygonToolAction = new QAction(
				QIcon(QPixmap(":/toolbar/tools/polygon_icon.png")), "Polygon tool", this
	);
	QAction *starToolAction = new QAction(
				QIcon(QPixmap(":/toolbar/tools/star_icon.png")), "Star tool", this
	);
	QAction *fillToolAction = new QAction(
				QIcon(QPixmap(":/toolbar/tools/fill_icon.png")), "Fill tool", this
	);
	QAction *clearToolAction = new QAction(
				QIcon(QPixmap(":/toolbar/tools/clear_icon.png")), "Clear", this
	);
	lineToolAction->setCheckable(true);
	polygonToolAction->setCheckable(true);
	starToolAction->setCheckable(true);
	fillToolAction->setCheckable(true);

	QPixmap redPixmap(settings.getIconWidth(), settings.getIconHeight());
	QPixmap yellowPixmap(settings.getIconWidth(), settings.getIconHeight());
	QPixmap	greenPixmap(settings.getIconWidth(), settings.getIconHeight());
	QPixmap cyanPixmap(settings.getIconWidth(), settings.getIconHeight());
	QPixmap bluePixmap(settings.getIconWidth(), settings.getIconHeight());
	QPixmap magentaPixmap(settings.getIconWidth(), settings.getIconHeight());
	redPixmap.fill(Qt::red);
	yellowPixmap.fill(Qt::yellow);
	greenPixmap.fill(Qt::green);
	cyanPixmap.fill(Qt::cyan);
	bluePixmap.fill(Qt::blue);
	magentaPixmap.fill(Qt::magenta);

	QAction *redAction = new QAction(QIcon(redPixmap), "Red", this);
	QAction *yellowAction = new QAction(QIcon(yellowPixmap), "Yellow", this);
	QAction *greenAction = new QAction(QIcon(greenPixmap), "Green", this);
	QAction *cyanAction = new QAction(QIcon(cyanPixmap), "Cyan", this);
	QAction *blueAction = new QAction(QIcon(bluePixmap), "Blue", this);
	QAction *magentaAction = new QAction(QIcon(magentaPixmap), "Magenta", this);
	QAction *customColorAction = new QAction(
				QIcon(QPixmap(":/toolbar/color_choose/custom_color_icon")), "Custom color", this
	);
	redAction->setCheckable(true);
	yellowAction->setCheckable(true);
	greenAction->setCheckable(true);
	cyanAction->setCheckable(true);
	blueAction->setCheckable(true);
	magentaAction->setCheckable(true);
	customColorAction->setCheckable(true);
	customColorAction->setChecked(true);

	QAction *menuInfoAction = new QAction("&About", this);
	QAction *toolBarInfoAction = new QAction(
				QIcon(QPixmap(":/toolbar/info/info_icon.png")), "About", this
	);

	// ================================ ACTION CONNECTING ================================

	auto uncheckTools = [=]() {
		lineToolAction->setChecked(false);
		polygonToolAction->setChecked(false);
		starToolAction->setChecked(false);
		fillToolAction->setChecked(false);
	};

	auto uncheckColors = [=]() {
		redAction->setChecked(false);
		yellowAction->setChecked(false);
		greenAction->setChecked(false);
		cyanAction->setChecked(false);
		blueAction->setChecked(false);
		magentaAction->setChecked(false);
		customColorAction->setChecked(false);
	};

	// INFO ACTIONS
	auto openInfo = [=]() {
		QMessageBox::about(this, "About",
						   "The first practical work on the subject \"Computer graphics\".\n"
						   "Done by Timur Letyagin (FIT NSU).\n"
						   "February 2023"
		);
	};

	// FILESYSTEM ACTION CONNECTIONS
	QObject::connect(openFileAction, &QAction::triggered, this, &MainWindow::onOpenFileAction);
	QObject::connect(saveFileAction, &QAction::triggered, this, &MainWindow::onSaveFileAction);

	// TOOL ACTION CONNECTIONS
	QObject::connect(resizeToolAction, &QAction::triggered, resizeDialog, &ResizeDialog::open);
	QObject::connect(lineToolAction, &QAction::triggered, this, [=](){
		uncheckTools();
		lineToolAction->setChecked(true);
		selectedTool = ToolType::LINE_TOOL;
		prevPoint = QPoint();
		lineDialog->open();
	});
	QObject::connect(polygonToolAction, &QAction::triggered, this, [=](){
		uncheckTools();
		polygonToolAction->setChecked(true);
		selectedTool = ToolType::POLYGON_TOOL;
		prevPoint = QPoint();
		polygonDialog->open();
	});
	QObject::connect(starToolAction, &QAction::triggered, this, [=](){
		uncheckTools();
		starToolAction->setChecked(true);
		selectedTool = ToolType::STAR_TOOL;
		prevPoint = QPoint();
		starDialog->open();
	});
	QObject::connect(fillToolAction, &QAction::triggered, this, [=](){
		uncheckTools();
		fillToolAction->setChecked(true);
		selectedTool = ToolType::FILL_TOOL;
		prevPoint = QPoint();
	});
	QObject::connect(clearToolAction, &QAction::triggered, this, [=](){
		paintArea->clear();
	});

	// COLOR ACTIONS
	QObject::connect(redAction, &QAction::triggered, this, [=](){
		uncheckColors();
		redAction->setChecked(true);
		selectedColor = Qt::red;
	});
	QObject::connect(yellowAction, &QAction::triggered, this, [=](){
		uncheckColors();
		yellowAction->setChecked(true);
		selectedColor = Qt::yellow;

	});
	QObject::connect(greenAction, &QAction::triggered, this, [=](){
		uncheckColors();
		greenAction->setChecked(true);
		selectedColor = Qt::green;
	});
	QObject::connect(cyanAction, &QAction::triggered, this, [=](){
		uncheckColors();
		cyanAction->setChecked(true);
		selectedColor = Qt::cyan;
	});
	QObject::connect(blueAction, &QAction::triggered, this, [=](){
		uncheckColors();
		blueAction->setChecked(true);
		selectedColor = Qt::blue;
	});
	QObject::connect(magentaAction, &QAction::triggered, this, [=](){
		uncheckColors();
		magentaAction->setChecked(true);
		selectedColor = Qt::magenta;
	});
	QObject::connect(customColorAction, &QAction::triggered, this, [=](){
		uncheckColors();
		customColorAction->setChecked(true);
		selectedColor = QColorDialog::getColor(selectedColor, this);
	});

	// INFO ACTIONS
	QObject::connect(toolBarInfoAction, &QAction::triggered, this, openInfo);
	QObject::connect(menuInfoAction, &QAction::triggered, this, openInfo);

	// ================================ TOOLBAR INITIALIZING ================================

	toolBar = addToolBar("Enable toolbar");

	toolBar->addAction(openFileAction);
	toolBar->addAction(saveFileAction);

	toolBar->addSeparator();

	toolBar->addAction(resizeToolAction);
	toolBar->addAction(lineToolAction);
	toolBar->addAction(polygonToolAction);
	toolBar->addAction(starToolAction);
	toolBar->addAction(fillToolAction);
	toolBar->addAction(clearToolAction);

	toolBar->addSeparator();

	toolBar->addAction(redAction);
	toolBar->addAction(yellowAction);
	toolBar->addAction(greenAction);
	toolBar->addAction(cyanAction);
	toolBar->addAction(blueAction);
	toolBar->addAction(magentaAction);
	toolBar->addAction(customColorAction);

	toolBar->addSeparator();

	toolBar->addAction(toolBarInfoAction);

	// ================================ MENU INITIALIZING ================================

	QMenu *fileMenu = menuBar()->addMenu("&File");
	QMenu *toolsMenu = menuBar()->addMenu("&Tools");
	QMenu *colorMenu = menuBar()->addMenu("&Color");

	fileMenu->addAction(openFileAction);
	fileMenu->addAction(saveFileAction);

	toolsMenu->addAction(resizeToolAction);
	toolsMenu->addAction(lineToolAction);
	toolsMenu->addAction(polygonToolAction);
	toolsMenu->addAction(starToolAction);
	toolsMenu->addAction(fillToolAction);
	toolsMenu->addAction(clearToolAction);

	colorMenu->addAction(redAction);
	colorMenu->addAction(yellowAction);
	colorMenu->addAction(greenAction);
	colorMenu->addAction(cyanAction);
	colorMenu->addAction(blueAction);
	colorMenu->addAction(magentaAction);
	colorMenu->addAction(customColorAction);

	menuBar()->addAction(menuInfoAction);
}

void MainWindow::mousePressEvent(QMouseEvent *e) {

	if (e->button() != Qt::LeftButton) {
		return;
	}

	QPoint p = paintArea->mapToScene(paintArea->mapFromGlobal(e->scenePosition().toPoint())).toPoint();

	if (p.x() < 0 || p.y() < 0) {
		return;
	}

	if (p.x() > resizeDialog->selectedWidth() || p.y() > resizeDialog->selectedHeight()) {
		return;
	}

	switch (selectedTool) {
		case ToolType::LINE_TOOL: {
			if (prevPoint.isNull()) {
				prevPoint = p;
				return;
			}

			paintArea->drawLine(
						prevPoint, p,
						lineDialog->selectedThickness(),
						selectedColor
			);
			break;
		}
		case ToolType::POLYGON_TOOL: {
			paintArea->drawPolygon(
						p,
						polygonDialog->selectedThickness(),
						polygonDialog->selectedVertices(),
						polygonDialog->selectedRadius(),
						polygonDialog->selectedAngle(),
						selectedColor
			);
			break;
		}
		case ToolType::STAR_TOOL: {
			paintArea->drawStar(
						p,
						starDialog->selectedThickness(),
						starDialog->selectedVertices(),
						starDialog->selectedRadius(),
						starDialog->selectedAngle(),
						selectedColor
			);
			break;
		}
		case ToolType::FILL_TOOL: {
			paintArea->fill(
						p,
						selectedColor
			);
		}
		default: {
			break;
		}
	}

	prevPoint = QPoint();
}

// #####################
// #   PRIVATE SLOTS   #
// #####################

void MainWindow::onOpenFileAction() {
	QString filename = QFileDialog::getOpenFileName(this, "Open File", "/home", "Images (*.png *jpg *.gif *.bmp)");
	QImage image;

	if (filename.isNull()) {
		return;
	}

	if (!image.load(filename)) {
		QMessageBox::warning(this, "Warning", "Failed to open file!");
		return;
	}

	if (!paintArea->canFit(image)) {
		QMessageBox::warning(this, "Warning", "Opened picture is too large!");
		return;
	}

	paintArea->drawImage(image);
	resizeDialog->setWidth(image.width());
	resizeDialog->setHeight(image.height());
}

void MainWindow::onSaveFileAction() {
	QImage image = paintArea->getImage();

	QString filename = QFileDialog::getSaveFileName(this, "Save File", "/home/untitled.png", "Images (*.png)");

	if (filename.isNull()) {
		return;
	}

	if (!image.save(filename, "png")) {
		QMessageBox::warning(this, "Warning", "Failed to save file!");
		return;
	}
}
