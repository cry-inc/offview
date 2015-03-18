/**
 * @class MainWindow
 * @brief Provides the main application window
 *
 * This class offers a menu bar, a status bar and in the center a widget
 * that presents the 3D object.
 *
 * @author Markus Haecker
 * @date 2010-05-26
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ui_MainWindow.h"
#include "GlWidget.h"
#include "Version.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QString fileToOpen = QString(), QWidget* parent = 0);
	~MainWindow();

protected:
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;

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

	void createRenderModesMenu();
	void updateRenderModesMenu();
	void createLanguageMenu();
	void connectSignalsAndSlots();
	void parseFileAndShowObject(const QString & fileToOpen);
	void setMainWindowTitle(const QString & filename = "");
	void loadNativeLanguageFile();
	void retranslateUi();
	bool checkDragAndDropData(const QMimeData* mimeData, QString* filePath = 0) const;

signals:
	void triggered(int actionLanguageIndex);

private slots:
	void open();
	void close();
	void exit();
	void setBackgroundColor();
	void setObjectColor();
	void resetView();
	void toggleXzPlane();
	void toggleXyPlane();
	void toggleYzPlane();
	void toggleAxes();
	void setRenderMode();
	void about();
	void aboutQt();
	void syncMenu();
	void help();
	void setLanguage(int actionLanguageIndex);
};

#endif // MAINWINDOW_H
