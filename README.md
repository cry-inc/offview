# OffView

OffView is a simple tool for viewing OFF files with OpenGL.
It's based on the Qt library and is written in C++.

![Screenshot](/img/screenshot.png)

The OFF file format is a simple format to store 3D objects as polygon meshes.
OffView uses Qt for the GUI and OpenGL as 3D API to draw the objects.

This software was originally written by a group of four students in the context
of a graded software project at the University of Applied Sciences in Constance. 

The project was located at http://code.google.com/p/offview/, but since Google
Code will be turned off, this project was moved to GitHub at
https://github.com/cry-inc/offview.


## How to build OffView from source code

### Windows:

1. Install the latest Qt Community Edition with MingGW from http://www.qt.io/
2. Open the OffView project file (offview.pro) in the Qt Creator
3. Set the build configuration to 'Release'
4. Build it
5. The compiled binary should now be located in your release folder
  
If you plan to deploy OffView, do not forget to include the necessary Qt
and MinGW DLLs! See the Qt Wiki for more information on deploying: 
http://wiki.qt.io/Deploying-Windows-Applications
  
Currently you need the following DLLs when compiling with Qt 5.4:

* icudt53.dll, icuin53.dll, icuuc53.dll (Unicode support)
* libgcc_s_dw2-1.dll, libstdc++-6.dll, libwinpthread-1.dll (MinGW)
* Qt5Core.dll, Qt5Gui.dll, Qt5Widgets.dll, Qt5OpenGL.dll (Qt Modules)


### Linux:

1. Install Qt (5.4 recommended)
2. Open a new terminal window
3. cd /directory/with/offview-source
4. Type 'qmake -config release'
5. Type 'make'
6. You should now have a binary called "offview" in the current folder

### Mac OS X:

Volunteers required :)


## How to update the translation files

1. Open a terminal an change the directory to the OffView source folder
2. Run 'lupdate offview.pro' to extract and update all strings
3. Edit the translation files (lang/offview_*.ts) with Qt Linguist
4. Run 'lrelease offview.pro' to build the compressed translation files
5. Rebuild OffView. That's necessary because the translations are embedded
   in the OffView binary!


## How to add a new translation

1. Add the new translation to the file offview.pro. See the TRANSLATIONS variable.
   Example: 'TRANSLATIONS += lang/offview_<xy>.ts'
2. Open a terminal an change the directory to the OffView source folder
3. Run 'lupdate offview.pro' to create the new translation file
4. Copy a language icon with the size of 16 x 11 pixels into img/menu/
5. Add the icon to the resource file offview.qrc
6. Translate the translation file (lang/offview_<xy>.ts) with Qt Linguist.
   There are two items which must be translated, all other items are optional:
   MainWindow::LanguageMenuItem (Should contain the native name of the language)
   MainWindow::LanguageMenuIcon (Should contain the name of the language icon file)
7. Run 'lrelease offview.pro' to build the compressed translation file
8. Add the file lang/offview_<xy>.qm to the resource file offview.qrc
9. Rebuild OffView. That's necessary because the translations and icons are embedded
   in the OffView binary!


## Files

	doc/           The code documentation and the end user help files
	examples/      Some very simple OFF files for testing
	img/           Icons and other included images and screenshots
	lang/          The translation files
	src/           The C++ source code
	.gitignore     Filter for stuff that should not be under version control
	Doxyfile       Doxygen configuration file
	LICENSE        A copy of the GPL version 3
	offview.pro    The Qt project file
	offview.qrc    Qt resource file to embed images and translations
	offview.rc     Windows specific resource file for the Windows application icons
	README.md      This file


## Contributors

* Manuel Caputo
* Markus Haecker
* Daniel Fritz
* Benjamin Stauder


## Acknowledgments

The menu icons are taken from the Silk icon set, located at
http://www.famfamfam.com/lab/icons/silk. Many thanks to the author!
