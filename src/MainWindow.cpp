/**
 * @file MainWindow.cpp
 * @see MainWindow
 *
 * @author Markus Haecker
 * @date 2010-05-25
 */

#include <QFileInfo>

#include "MainWindow.h"
#include "SceneFactory.h"

/**
 * @brief Constructor of class MainWindow.
 *
 * The class is derived from the base class QMainWindow (public).
 * The layout has been created with Qt Designer and is loaded from file MainWindow.ui.
 *
 * @param[in] fileToOpen String with a file to load, may be empty (default = "").
 * @param[in] parent Parent widget (default = 0).
 */
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

/**
 * @brief Destructor of class MainWindow.
 */
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

/**
 * @brief Processes a drag enter event.
 *
 * @param[in,out] event Drag enter event.
 */
void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	QString filePath;
	if (checkDragAndDropData(event->mimeData())) {
		event->acceptProposedAction();
	}
}

/**
 * @brief Processes a drop event.
 *
 * @param[in,out] event Drop event.
 */
void MainWindow::dropEvent(QDropEvent* event)
{
	QString filePath;
	if (checkDragAndDropData(event->mimeData(), &filePath)) {
		event->acceptProposedAction();
		parseFileAndShowObject(filePath);
	}
}

/**
 * @brief Open, parse and show an OFF-File.
 *
 * @param[in] fileToOpen Path and name of the OFF-File.
 */
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

/**
 * @brief Set the main window title.
 *
 * The window title consists of the program name followed by an optional filename,
 * that can be passed as parameter.
 *
 * @warning DO NOT set the window title with Qt Designer! Otherwise every time
 * ui.retranslateUi() is called, the window title would be set to the title
 * assigned with Qt Designer.
 *
 * @param[in] filename Optional filename as QString.
 *
 */
void MainWindow::setMainWindowTitle(const QString & filename)
{
	if (filename.isEmpty()) {
		setWindowTitle("OffView");
	} else {
		setWindowTitle("OffView - " + filename);
	}
}

/**
 * @brief Connect menu items to methods.
 *
 * This function assigns menu items (generated from MainWindow.ui) to
 * methods (slots).
 * Menu items for choosing a render mode and for switching the language are
 * generated and connected dynamically by createRenderModesMenu() and
 * createLanguageMenu() (both called from constructor).
 */
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

/**
 * @brief Display all available render modes in menu "View" -> "Mode".
 *
 * The render modes are provided in a QStringList by listRenderModes().
 * Render modes are shown in an alignment group, each one with a radio button,
 * so that only one mode can be marked as active.
 * Render mode items are connected with setRenderMode(), which is competent
 * to initiate the mode change.
 */
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

/**
 * @brief Display all available languages in menu "Language".
 *
 * The menu is build dynamically and depends on the .qm translation files that
 * can be found in :/lang.
 *
 * For every translation file there is one menu item, where the item's text is
 * the translation of the string "LanguageMenuItem". Examples: Translation should
 * be "English" in an English translation and "Deutsch" in a German translation.
 *
 * With the translation of "LanguageMenuIcon" there is also the posibility to specify
 * an icon (has to be in resource folder :/img/menu/).
 *
 */
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

/**
 * @brief Translate the render modes in menu "View" -> "Mode".
 *
 * This function is called every time a user changes the application's
 * language using the language menu.
 */
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

/**
 * @brief Loads a language file.
 *
 * When an item from the language menu is selected, the signal
 * triggered(int actionLanguageIndex) is emitted, which is connected to
 * this slot. The signal gives us the parameter actionLanguageIndex, so we
 * know which language the user has chosen. It than can be accessed
 * with the QVector actionLanguage.
 *
 * setLanguage() is also called by loadNativeLanguageFile() on program's startup.
 *
 * @param[in] actionLanguageIndex Index of language in QVector actionLanguage.
 */
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

/**
 * @brief Retranslate the user interface.
 *
 * This method is called every time a user changes the application's
 * language using the Language menu.
 */
void MainWindow::retranslateUi()
{
	updateRenderModesMenu(); // Translates the render modes in menu "View" -> "Mode"
	ui.retranslateUi(this); // Translates all the rest
}

/**
 * @brief Check drag and drop MIME data for a valid file path.
 *
 * @param[in] mimeData MIME data of the drag and drop event.
 * @param[out] filePath Optional file path output, if the MIME data contained a valid one.
 *
 * @return	True, if the MIME data contained a valid file path, false otherwise.
 */
bool MainWindow::checkDragAndDropData(const QMimeData* mimeData, QString* filePath)
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

/**
 * @brief Set a language suitable to the system's settings.
 *
 * This method is called from the MainWindow constructor and has to
 * be executed AFTER createLanguageMenu()!
 *
 * Note: A language file must have the format something_XX.qm, where XX is a
 * lowercase, two-letter ISO 639 language code.
 */
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

/**
 * @brief Shows an "Open File" dialog.
 *
 * The dialog shows the directory of an previous loaded file. If there is
 * none, the application's current directory is shown.
 *
 * In the dialog, users can select an .off file. After clicking "open" it is attempted to open
 * and parse the file and show the 3D model in the window.
 *
 * @see parseFileAndShowObject()
 */
void MainWindow::open()
{
	QString file = QFileDialog::getOpenFileName(
		         this, tr("Open File"),
		         openedFile.absoluteFilePath(), SceneFactory::openFileString());

	if(!file.isNull()) {
		parseFileAndShowObject(file);
	}
}

/**
 * @brief Close a previous loaded .off file.
 *
 * Clear the central widget, unload any 3D model shown in it.
 */
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

/**
 * @brief Tells the application to exit.
 */
void MainWindow::exit()
{
	QApplication::exit();
}

/**
 * @brief Set the background color.
 *
 * Pop up a color dialog and let the user choose a color. The background will be
 * filled with the chosen color.
 */
void MainWindow::setBackgroundColor()
{
	QColor color = glWidget->backgroundColor();
	#if QT_VERSION >= 0x040500
		color = QColorDialog::getColor(color, this, tr("Choose background color"));
	#else
		color = QColorDialog::getColor(color, this);
	#endif
	if (color.isValid()) {
		glWidget->setBackgroundColor(color);
	}
}

/**
 * @brief Set the object color.
 *
 * Pop up a color dialog and let the user choose a color. The chosen color
 * is used for the object.
 */
void MainWindow::setObjectColor()
{
	QColor color = glWidget->objectColor();
	#if QT_VERSION >= 0x040500
		color = QColorDialog::getColor(color, this, tr("Choose object color"));
	#else
		color = QColorDialog::getColor(color, this);
	#endif
	if (color.isValid()) {
		glWidget->setObjectColor(color);
	}
}

/**
 * @brief Reset the view.
 *
 * All settings a user has made are cleared and are set to default.
 */
void MainWindow::resetView()
{
	glWidget->resetView();
	syncMenu();
}

/**
 * @brief Toggle coordinate axes visible/hidden.
 */
void MainWindow::toggleAxes()
{
	if (ui.actionShow_Coordinate_System->isChecked()) {
		glWidget->setAxes(true);
	} else {
		glWidget->setAxes(false);
	}
}

/**
 * @brief Toggle xz Plane visible/hidden.
 */
void MainWindow::toggleXzPlane()
{
	if (ui.actionXz_Plane->isChecked()) {
		glWidget->setXzPlane(true);
	} else {
		glWidget->setXzPlane(false);
	}
}

/**
 * @brief Toggle xy Plane visible/hidden.
 */
void MainWindow::toggleXyPlane()
{
	if (ui.actionXy_Plane->isChecked()) {
		glWidget->setXyPlane(true);
	} else {
		glWidget->setXyPlane(false);
	}
}

/**
 * @brief Toggle yz Plane visible/hidden.
 */
void MainWindow::toggleYzPlane()
{
	if (ui.actionYz_Plane->isChecked()) {
		glWidget->setYzPlane(true);
	} else {
		glWidget->setYzPlane(false);
	}
}

/**
 * @brief Synchronize the menu's check buttons.
 *
 * Set or unset the check buttons in menus "Show Planes" and "Show Coordinate
 * System", depending on the values returned by the GLWidget methods.
 *
 * Values are set initial in GlWidget::resetView().
 *
 * @see resetView()
 */
void MainWindow::syncMenu()
{
	ui.actionShow_Coordinate_System->setChecked(glWidget->axes());
	ui.actionXz_Plane->setChecked(glWidget->xzPlane());
	ui.actionXy_Plane->setChecked(glWidget->xyPlane());
	ui.actionYz_Plane->setChecked(glWidget->yzPlane());
}

/**
 * @brief Open a HTML helpfile with a suitable application.
 *
 * Which helpfile will be loaded depends on the loaded language and the
 * translation of "help_en.html" in the correspondending language file.
 * The helpfile has to be located in the folder "doc" in the applications
 * directory path.
 */
void MainWindow::help()
{
	QString path = QString("file:///") + 
		QApplication::applicationDirPath() + QString("/doc/") +
		tr("help_en.html");
	QDesktopServices::openUrl(QUrl(path));
}

/**
 * @brief Set new render mode.
 *
 * In the menu bar there is a dynamically generated menu for changing the
 * render mode. Available render modes are shown as radio buttons. If the user
 * select one, this function checks which it was and update the window.
 *
 * @see GlWidget::setRenderMode()
 */
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

/**
 * @brief Display a simple message box about our cool viewer...
 *
 * The message box includes the version number of OffView, provided by the macro
 * OFFVIEW_VERSION in Version.h.
 *
 * @see Version.h
 */
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

/**
 * @brief Display a simple message box about Qt.
 *
 * The message box includes the version number of Qt being used by the application.
 */
void MainWindow::aboutQt()
{
	QApplication::aboutQt();
}
