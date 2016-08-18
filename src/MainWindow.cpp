#include <QFileInfo>

#include "MainWindow.h"
#include "SceneFactory.h"

MainWindow::MainWindow(QString fileToOpen, QWidget* parent)
	: QMainWindow(parent)
{
	scene = 0;
	renderModesAlignmentGroup = 0;
	signalMapper = 0;

	ui.setupUi(this);
	setMainWindowTitle();

	// Enable drag and drop for opening files
	QMainWindow::setAcceptDrops(true);

	glWidget = new GlWidget(); // QMainWindow takes ownership of the glWidget
		// pointer and deletes it at the appropriate time
	setCentralWidget(glWidget);

	createLanguageMenu();
	loadNativeLanguageFile(); // has to be executed after createLanguageMenu()!

	createRenderModesMenu();
	syncMenu();
	connectSignalsAndSlots();

	statusBar()->showMessage(tr("Select \"Open File\" from the \"File\" menu to load an object"));

	// If a file was given as command line argument, try to parse it
	if (!fileToOpen.isEmpty()) {
		parseFileAndShowObject(fileToOpen);
	}
}

MainWindow::~MainWindow()
{
	if (renderModesAlignmentGroup) {
		delete renderModesAlignmentGroup;
	}

	if (signalMapper) {
		delete signalMapper;
	}

	if (scene) {
		delete scene;
	}

	for (int i = 0; i < actionRenderMode.size(); ++i) {
		delete actionRenderMode.at(i);
	}
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	QString filePath;
	if (checkDragAndDropData(event->mimeData())) {
		event->acceptProposedAction();
	}
}

void MainWindow::dropEvent(QDropEvent* event)
{
	QString filePath;
	if (checkDragAndDropData(event->mimeData(), &filePath)) {
		event->acceptProposedAction();
		parseFileAndShowObject(filePath);
	}
}

void MainWindow::parseFileAndShowObject(const QString & fileToOpen)
{
	try {
		IScene* newScene = SceneFactory::openFile(fileToOpen);

		// Save path and name of file and show filename in window title
		openedFile.setFile(fileToOpen);
		setMainWindowTitle(openedFile.fileName());

		glWidget->setScene(0);
		if (scene) {
			delete scene;
		}
		scene = newScene;
		glWidget->setScene(scene);
		syncMenu();

		// Select the best available render mode
		int mode;
		if (scene->isColored()) {
			mode = glWidget->modeForColoredScenes();
		} else {
			mode = glWidget->modeForUncoloredScenes();
		}
		actionRenderMode.at(mode)->setChecked(true);

		// Show filename in status bar
		statusBar()->showMessage(tr("File %1 was loaded").arg(openedFile.fileName()));
	}
	catch(QString & message) {
		QMessageBox::warning(this, tr("Error"), tr("An error occured while "
				"processing file ") + fileToOpen + "<br><br>" + message);
	}
}

void MainWindow::setMainWindowTitle(const QString & filename)
{
	if (filename.isEmpty()) {
		setWindowTitle("OffView");
	} else {
		setWindowTitle("OffView - " + filename);
	}
}

void MainWindow::connectSignalsAndSlots()
{
	connect(ui.actionOpen_File,	SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionClose_File, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(exit()));
	connect(ui.actionXz_Plane, SIGNAL(triggered()), this, SLOT(toggleXzPlane()));
	connect(ui.actionXy_Plane, SIGNAL(triggered()), this, SLOT(toggleXyPlane()));
	connect(ui.actionYz_Plane, SIGNAL(triggered()), this, SLOT(toggleYzPlane()));
	connect(ui.actionChoose_Background_Color, SIGNAL(triggered()), this,
			SLOT(setBackgroundColor()));
	connect(ui.actionChoose_Object_Color, SIGNAL(triggered()), this,
			SLOT(setObjectColor()));
	connect(ui.actionShow_Coordinate_System, SIGNAL(triggered()), this,
			SLOT(toggleAxes()));
	connect(ui.actionReset_View, SIGNAL(triggered()), this, SLOT(resetView()));
	connect(ui.actionHelp_Content, SIGNAL(triggered()), this, SLOT(help()));
	connect(ui.actionAbout_OffView, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

void MainWindow::createRenderModesMenu()
{
	QStringList nameOfRenderModes = glWidget->listRenderModes();

	int numberOfRenderModes = nameOfRenderModes.size();
	renderModesAlignmentGroup = new QActionGroup(this);

	for (int i = 0; i < numberOfRenderModes; ++i) {
		actionRenderMode.append(new QAction("&" + QString().setNum(i + 1) + " "
				+ nameOfRenderModes.at(i), this));
		actionRenderMode[i]->setCheckable(true);
		connect(actionRenderMode[i], SIGNAL(toggled(bool)), this, SLOT(setRenderMode()));

		renderModesAlignmentGroup->addAction(actionRenderMode[i]);
		ui.menuMode->addAction(actionRenderMode[i]);
	}

	int renderMode = glWidget->renderMode();
	actionRenderMode[renderMode]->setChecked(true); // Mark radio button of active mode
}

void MainWindow::createLanguageMenu()
{
	QString qmPath(":/lang");

	QDir dir(qmPath);
	QStringList filenames = dir.entryList(QStringList("offview_*.qm"));
	if (filenames.isEmpty()) {
		return; // No matching .qm language files found
	}

	signalMapper = new QSignalMapper(this);
	QTranslator translator;
	int j = -1;

	for (int i = 0; i < filenames.size(); ++i) {
		if (!translator.load(filenames[i], qmPath)) {
			// The translation was not successfully loaded
			continue;
		}

		j++; // Do only increase, if translation could be loaded

		// The translation of "LanguageMenuItem" is used as menu item text
		QString language = translator.translate("MainWindow", "LanguageMenuItem");
		if (language.isEmpty()) {
			language = tr("Unknown language");
		}

		actionLanguage.append(new QAction(tr("&%1 %2").
				arg(j + 1).arg(language), this));

		// Set up menu icon (given by the translation of "LanguageMenuIcon")
		QString icon = translator.translate("MainWindow", "LanguageMenuIcon");
		actionLanguage[j]->setIcon(QIcon(":/img/menu/" + icon));
		actionLanguage[j]->setIconVisibleInMenu(true);

		// Set the action's internal data to the language filename, so that
		// setLanguage() and loadNativeLanguageFile() will be able to load the right one
		actionLanguage[j]->setData(filenames[i]);

		connect(actionLanguage[j], SIGNAL(triggered()), signalMapper, SLOT(map()));
		signalMapper->setMapping(actionLanguage[j], j);

		ui.menuLanguage->addAction(actionLanguage[j]);
	}

	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(setLanguage(int)));
}

void MainWindow::updateRenderModesMenu()
{
	if (!renderModesAlignmentGroup) {
		return;
	}

	QList<QAction*> actions = renderModesAlignmentGroup->actions();
	if (!actions.isEmpty()) {
		QStringList nameOfRenderModes = glWidget->listRenderModes();
		for (int i = 0; i < actions.size(); ++i) {
			actions.at(i)->setText("&" + QString().setNum(i + 1) + " "
					+ nameOfRenderModes.at(i));
		}
	}
}

void MainWindow::setLanguage(int actionLanguageIndex)
{
	if ((actionLanguageIndex >= actionLanguage.size()) ||
			(actionLanguageIndex < 0)) {
		QMessageBox::warning(this, tr("Error"), tr("Couldn't set language"));
		return;
	}

	QString qmPath = ":/lang";
	QString qmFile = actionLanguage[actionLanguageIndex]->data().toString();

	QCoreApplication::removeTranslator(&translator);

	if (!translator.load(qmFile, qmPath)) {
		QMessageBox::warning(this, tr("Error"), tr("Couldn't load language "
				"file ") + qmPath + "/" + qmFile);
		return;
	}

	// Set the font's weight of the selected language menu item to bold,
	// all others to the application's default font (non-bold)
	QFont font;
	for (int i = 0; i < actionLanguage.size(); ++i) {
		actionLanguage[i]->setFont(font);
	}
	font.setBold(true);
	actionLanguage[actionLanguageIndex]->setFont(font);

	QCoreApplication::installTranslator(&translator);
	retranslateUi();
}

void MainWindow::retranslateUi()
{
	updateRenderModesMenu(); // Translates the render modes in menu "View" -> "Mode"
	ui.retranslateUi(this); // Translates all the rest
}

bool MainWindow::checkDragAndDropData(const QMimeData* mimeData, QString* filePath) const
{
	if (!mimeData->hasUrls()) {
		return false;
	}

	QList<QUrl> urlList = mimeData->urls();
	if (urlList.length() != 1) {
		return false;
	}

	QFileInfo fileInfo(urlList[0].toLocalFile());
	if (!fileInfo.exists() || !fileInfo.isFile()) {
		return false;
	}

	if (filePath) {
		*filePath = fileInfo.filePath();
	}
	return true;
}

void MainWindow::loadNativeLanguageFile()
{
	QString locale = QLocale::system().name(); // e.g. de_DE
	QString translation;
	int actionLanguageIndex = -1;

	for (int i = 0; i < actionLanguage.size(); ++i) {
		translation = actionLanguage[i]->data().toString();
		translation.remove(0, translation.indexOf('_') + 1); // lang_de.qm -> de.qm
		translation.truncate(translation.lastIndexOf('.')); // de.qm -> de
		if (locale.contains(translation, Qt::CaseSensitive)) {
			actionLanguageIndex = i;
			break;
		}
	}

	if (actionLanguageIndex >= 0) {
		setLanguage(actionLanguageIndex);
	} else {
		if (!actionLanguage.isEmpty()) {
			setLanguage(0);
		}
	}
}

void MainWindow::open()
{
	QString file = QFileDialog::getOpenFileName(
		this, tr("Open File"),
		openedFile.absoluteFilePath(),
		SceneFactory::openFileString()
	);

	if(!file.isNull()) {
		parseFileAndShowObject(file);
	}
}

void MainWindow::close()
{
	glWidget->setScene(0);
	if (scene) {
		delete scene;
	}
	scene = 0;
	syncMenu();
	setMainWindowTitle(); // Remove filename from window title
}

void MainWindow::exit()
{
	QApplication::exit();
}

void MainWindow::setBackgroundColor()
{
	QColor color = glWidget->backgroundColor();
	color = QColorDialog::getColor(color, this, tr("Choose background color"));
	if (color.isValid()) {
		glWidget->setBackgroundColor(color);
	}
}

void MainWindow::setObjectColor()
{
	QColor color = glWidget->objectColor();
	color = QColorDialog::getColor(color, this, tr("Choose object color"));
	if (color.isValid()) {
		glWidget->setObjectColor(color);
	}
}

void MainWindow::resetView()
{
	glWidget->reset();
	syncMenu();
}

void MainWindow::toggleAxes()
{
	if (ui.actionShow_Coordinate_System->isChecked()) {
		glWidget->setAxes(true);
	} else {
		glWidget->setAxes(false);
	}
}

void MainWindow::toggleXzPlane()
{
	if (ui.actionXz_Plane->isChecked()) {
		glWidget->setXzPlane(true);
	} else {
		glWidget->setXzPlane(false);
	}
}

void MainWindow::toggleXyPlane()
{
	if (ui.actionXy_Plane->isChecked()) {
		glWidget->setXyPlane(true);
	} else {
		glWidget->setXyPlane(false);
	}
}

void MainWindow::toggleYzPlane()
{
	if (ui.actionYz_Plane->isChecked()) {
		glWidget->setYzPlane(true);
	} else {
		glWidget->setYzPlane(false);
	}
}

void MainWindow::syncMenu()
{
	ui.actionShow_Coordinate_System->setChecked(glWidget->axes());
	ui.actionXz_Plane->setChecked(glWidget->xzPlane());
	ui.actionXy_Plane->setChecked(glWidget->xyPlane());
	ui.actionYz_Plane->setChecked(glWidget->yzPlane());
}

void MainWindow::help()
{
	QString path = QString("file:///") + 
		QApplication::applicationDirPath() + QString("/doc/") +
		tr("help_en.html");
	QDesktopServices::openUrl(QUrl(path));
}

void MainWindow::setRenderMode()
{
	for (int i = 0; i < actionRenderMode.size(); ++i) {
		if (actionRenderMode.at(i)->isChecked()) {
			glWidget->setRenderMode(i);
			break;
		}
	}
	statusBar()->showMessage(glWidget->renderModeName() + tr(" activated"));
}

void MainWindow::about()
{
	QString body = tr("<h3>About OffView version %1</h3>"
			"<p>This program is for viewing Object File Format (.off) files with OpenGL.</p>"
			"<p>Object File Format files are used to represent the geometry of a "
			"model by specifying the polygons of the model's surface. The "
			"polygons can have any number of vertices.</p>"
			"<p>OffView was written by four students of Software Engineering at "
			"the University of Applied Sciences in Constance:</p>"
			"<ul>"
			"<li>Manuel Caputo,</li>"
			"<li>Markus Haecker,</li>"
			"<li>Daniel Fritz and</li>"
			"<li>Benjamin Stauder.</li>"
			"</ul>"
			"<p>The project is hosted on GitHub and can be found under "
			"<a href=\"https://github.com/cry-inc/offview\">https://github.com/cry-inc/offview</a>. "
			"It's free software under the conditions of version 3 of the GNU General "
			"Public License (<a href=\"http://www.gnu.org/licenses/gpl-3.0.html\">GPLv3</a>). "
			"</p>").arg(OFFVIEW_VERSION);

	QMessageBox::about(this, tr("About OffView"), body);
}

void MainWindow::aboutQt()
{
	QApplication::aboutQt();
}
