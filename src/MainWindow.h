#pragma once

#include <QtGui>

#include "ui_MainWindow.h"
#include "GlWidget.h"
#include "Version.h"

/**
 * @brief Provides the main application window
 *
 * This class offers a menu bar, a status bar and in the center a widget
 * that presents the 3D object.
 *
 * @author Markus Haecker
 * @date 2010-05-26
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor of class MainWindow.
	 *
	 * The class is derived from the base class QMainWindow (public).
	 * The layout has been created with Qt Designer and is loaded from file MainWindow.ui.
	 *
	 * @param[in] fileToOpen String with a file to load, may be empty (default = "").
	 * @param[in] parent Parent widget (default = 0).
	 */
	MainWindow(QString fileToOpen = QString(), QWidget* parent = 0);

	/**
	 * @brief Destructor of class MainWindow.
	 */
	~MainWindow();

protected:
	/**
	 * @brief Processes a drag enter event.
	 *
	 * @param[in,out] event Drag enter event.
	 */
	void dragEnterEvent(QDragEnterEvent* event) override;

	/**
	 * @brief Processes a drop event.
	 *
	 * @param[in,out] event Drop event.
	 */
	void dropEvent(QDropEvent* event) override;

signals:
	void triggered(int actionLanguageIndex);

private:
	/**
	 * @brief Display all available render modes in menu "View" -> "Mode".
	 *
	 * The render modes are provided in a QStringList by listRenderModes().
	 * Render modes are shown in an alignment group, each one with a radio button,
	 * so that only one mode can be marked as active.
	 * Render mode items are connected with setRenderMode(), which is competent
	 * to initiate the mode change.
	 */
	void createRenderModesMenu();

	/**
	 * @brief Translate the render modes in menu "View" -> "Mode".
	 *
	 * This function is called every time a user changes the application's
	 * language using the language menu.
	 */
	void updateRenderModesMenu();

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
	void createLanguageMenu();

	/**
	 * @brief Connect menu items to methods.
	 *
	 * This function assigns menu items (generated from MainWindow.ui) to
	 * methods (slots).
	 * Menu items for choosing a render mode and for switching the language are
	 * generated and connected dynamically by createRenderModesMenu() and
	 * createLanguageMenu() (both called from constructor).
	 */
	void connectSignalsAndSlots();
	
	/**
	 * @brief Open, parse and show an OFF-File.
	 *
	 * @param[in] fileToOpen Path and name of the OFF-File.
	 */
	void parseFileAndShowObject(const QString & fileToOpen);

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
	void setMainWindowTitle(const QString & filename = "");

	/**
	 * @brief Set a language suitable to the system's settings.
	 *
	 * This method is called from the MainWindow constructor and has to
	 * be executed AFTER createLanguageMenu()!
	 *
	 * Note: A language file must have the format something_XX.qm, where XX is a
	 * lowercase, two-letter ISO 639 language code.
	 */
	void loadNativeLanguageFile();

	/**
	 * @brief Retranslate the user interface.
	 *
	 * This method is called every time a user changes the application's
	 * language using the Language menu.
	 */
	void retranslateUi();

	/**
	 * @brief Check drag and drop MIME data for a valid file path.
	 *
	 * @param[in] mimeData MIME data of the drag and drop event.
	 * @param[out] filePath Optional file path output, if the MIME data contained a valid one.
	 *
	 * @return	True, if the MIME data contained a valid file path, false otherwise.
	 */
	bool checkDragAndDropData(const QMimeData* mimeData, QString* filePath = 0) const;

private slots:
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
	void open();

	/**
	 * @brief Close a previous loaded .off file.
	 *
	 * Clear the central widget, unload any 3D model shown in it.
	 */
	void close();

	/**
	 * @brief Tells the application to exit.
	 */
	void exit();

	/**
	 * @brief Set the background color.
	 *
	 * Pop up a color dialog and let the user choose a color. The background will be
	 * filled with the chosen color.
	 */
	void setBackgroundColor();

	/**
	 * @brief Set the object color.
	 *
	 * Pop up a color dialog and let the user choose a color. The chosen color
	 * is used for the object.
	 */
	void setObjectColor();

	/**
	 * @brief Reset the view.
	 *
	 * All settings a user has made are cleared and are set to default.
	 */
	void resetView();

	/**
	 * @brief Toggle xz Plane visible/hidden.
	 */
	void toggleXzPlane();

	/**
	 * @brief Toggle xy Plane visible/hidden.
	 */
	void toggleXyPlane();

	/**
	 * @brief Toggle yz Plane visible/hidden.
	 */
	void toggleYzPlane();

	/**
	 * @brief Toggle coordinate axes visible/hidden.
	 */
	void toggleAxes();

	/**
	 * @brief Set new render mode.
	 *
	 * In the menu bar there is a dynamically generated menu for changing the
	 * render mode. Available render modes are shown as radio buttons. If the user
	 * select one, this function checks which it was and update the window.
	 *
	 * @see GlWidget::setRenderMode()
	 */
	void setRenderMode();

	/**
	 * @brief Display a simple message box about our cool viewer...
	 *
	 * The message box includes the version number of OffView, provided by the macro
	 * OFFVIEW_VERSION in Version.h.
	 *
	 * @see Version.h
	 */
	void about();

	/**
	 * @brief Display a simple message box about Qt.
	 *
	 * The message box includes the version number of Qt being used by the application.
	 */
	void aboutQt();

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
	void syncMenu();

	/**
	 * @brief Open a HTML helpfile with a suitable application.
	 *
	 * Which helpfile will be loaded depends on the loaded language and the
	 * translation of "help_en.html" in the correspondending language file.
	 * The helpfile has to be located in the folder "doc" in the applications
	 * directory path.
	 */
	void help();

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
	void setLanguage(int actionLanguageIndex);

private:
	/**
	 * @brief Grant access to the user interface.
	 *
	 * The layout of our main window has been created with Qt Designer and is
	 * saved in MainWindow.ui. MainWindowClass is generated from the MainWindow.ui
	 * file. With the instance "ui" we get access to the elements of the GUI.
	 */
	Ui::MainWindowClass ui;

	/**
	 * @brief Pointer to our central widget.
	 *
	 * @see GlWidget
	 */
	GlWidget* glWidget;

	/**
	 * @brief Pointer to a scene, which is shown in our central widget.
	 *
	 * The implementation of the IScene interface contains all functions needed
	 * to parse and represent a given .off file.
	 *
	 * @see IScene
	 * @see OffScene
	 * @see GlWidget::setScene()
	 */
	IScene* scene;

	/**
	 * @brief Saves all available render modes from menu "View" -> "Mode".
	 *
	 * @see createRenderModesMenu()
	 * @see setRenderMode()
	 */
	QVector<QAction*> actionRenderMode;

	/**
	 * @brief Saves all available languages from menu "Language".
	 *
	 * @see createLanguageMenu()
	 * @see setLanguage()
	 * @see loadNativeLanguageFile()
	 */
	QVector<QAction*> actionLanguage;

	/**
	 * @brief Render modes from menu "View" -> "Mode" are shown in this alignment group.
	 */
	QActionGroup* renderModesAlignmentGroup;

	/**
	 * @brief Contains a set of translations to a specific target language.
	 *
	 * @see setLanguage()
	 */
	QTranslator translator;

	/**
	 * @brief Saves path and name of last opened file.
	 *
	 * @see open()
	 */
	QFileInfo openedFile;

	/**
	 * @brief To make out which item in the dynamically created Language menu has been selected.
	 *
	 * @see createLanguageMenu()
	 */
	QSignalMapper* signalMapper;
};
