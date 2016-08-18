#pragma once

#include <QtCore>

#include "IScene.h"
#include "CPolygon.h"

/**
 * @brief Parser and IScene implementation for OFF files
 *
 * This class contains all functions needed to parse and represent a given
 * OFF file. OffScene is one of many possible IScene implementations.
 * IScene is our abstract model of a scene and is used by GlWidget to draw it.
 *
 * @see IScene
 * @author B. Stauder
 * @author M. Caputo
 * @date 2010-03-17
 */
class OffScene : public QObject, public IScene
{
	Q_OBJECT

public:
	/**
	 * @brief Constructor
	 *
	 * Constructor of OffScene, delegates almost all work to parseFile().
	 *
	 * @param [in] fileName The OFF file which should get parsed
	 */
	OffScene(const QString & fileName);

	/**
	 * @brief Destructor
	 *
	 * Destructor of OffScene
	 */
	~OffScene();
	
	bool isColored() const override;
	int polygonsCount() const override;
	const CPolygon* polygon(int i) const override;
	int verticesCount() const override;
	const CVertex* vertex(int i) const override;

private slots:
	/**
	 * @brief Cancels the loading process with an exception
	 *
	 * If the user clicks on the cancel button during a file loading
	 * process, this method is called and will cancel the loading process.
	 */
	void cancel();
	
private:

	/**
	 * @brief Opens a file to create a OffScene
	 *
	 * Opens a OFF file and reads from it to create our scene. Steps involved:
	 *
	 * 1. Open the file and create a stream to read it
	 *
	 * 2. Read the string "OFF" which should be the start of all valid OFF files
	 *
	 * 3. Read the next line with the vertex, polygon and edge numbers
	 *
	 * 4. Read the next n lines and try to parse it as vertices, where n is the
	 *    number of vertices from the line above. Each vertex line should contain
	 *    the x, y and z coordinate and a optional RGB or RGBA color value.
	 *    The color values can be floats between 0 and 1 or integers between 0 and 255!
	 *
	 * 5. Read the next m lines and try to parse it into polygons, where m is the
	 *    number of polygons. Each polygon line should consist of the vertex counter
	 *    followed by the vertex indices and an another optional color value.
	 *
	 * 6. Sort the polygons by the transparency value to archieve a better result
	 *    when rendering the scene in OpenGL. (Details are provided in the German
	 *    OffView documentation!)
	 *
	 * 7. Precalculate the normal vectors for all polygon surfaces. They are used
	 *    when rendering the scene in OpenGL with lights. (Details are provided in 
	 *    the German OffView documentation!)
	 *
	 * @param [in] fileName Contains the path to the file which will be parsed
	 */
	void parseFile(const QString & fileName);

	/**
	 * @brief Skips empty lines and comments and reads the next line
	 *
	 * Reads the next non empty and comment free line of of the supplied
	 * QTextStream. The line is also trimmed, so it has no whitespaces at the
	 * beginning or end. Throws an QString exception if there is no more valid
	 * line to read. 
	 *
	 * @param [in] stream A stream to read the next valid line
	 * @return The next not empty line free from comments
	 */
	QString readNextLine(QTextStream *stream);

	/**
	 * @brief Splits at tab and space, skips the empty parts
	 *
	 * Splits a string at all tabs and spaces and omits the empty parts.
	 * 
	 * Example: 
	 * "    This \t\t is\t   a  \t  test " will transformed to:
	 * "This", "is", "a" and "test"
	 *
	 * @param string The string which should be splitted
	 * @return A list of strings
	 */
	QStringList split2Token(QString *string);
	
	/**
	 * @brief Create a (colored) vertex from a list of tokens
	 *
	 * Creates a (colored) vertex from a list of tokens. Throws an exception if
	 * there is not enough or invalid data in the token list. If a valid color
	 * is supplied in the token list, it will be parsed also.
	 *
	 * @param [in] tokens A list of strings which contains the vertex information
	 * @return A vertex object which may be colored
	 */
	CVertex* readVertex(QStringList *tokens);

	/**
	 * @brief Create a (colored) polygon from a list of tokens
	 *
	 * Creates a (colored) polygon from a list of tokens. It reads the index
	 * numbers from the token and combines it whith the existing list of
	 * vertices. Throws an exception if there is not enough or invalid data
	 * in the token list. Reads also optional color information from the tokens.
	 *
	 * @param [in] tokens A string list which contains the polygon information
	 * @param [in] vertices A field of CVertex pointers
	 * @return A polygon object which may be colored
	 */
	CPolygon* readPolygon(QStringList *tokens, const QVector<CVertex*> vertices);

	/**
	 * @brief Read RGB colors with optional alpha channel
	 *
	 * Reads a color with optional alpha channel from a list of tokens,
	 * starting at offset. Throws an exception if an invalid color value
	 * will found, meaning not a number or a number smaller than 0 or
	 * greater than 255. If there are not enough valid tokens for a color,
	 * a invalid/empty color object will be returned.
	 *
	 * @param [in] tokens A tokens list containing strings
	 * @param [in] offset The offset at which the color information is assumed
	 * @return A color object which may be invalid or empty
	 */
	QColor readColor(QStringList *tokens, int offset);

	/**
	 * @brief Levels a list of double values
	 *
	 * This method is used to bring all color values to a defined range.
	 * It levels a list of double values between 0 and 255 to a range between
	 * 0 and 1. If all values are already smaller or equal than 1 nothing will
	 * be changed.
	 *
	 * Examples:	{255, 0, 255} would became {1, 0, 1} and
	 * 				{0.5, 1.0, 0.0} will remain unchanged
	 *
	 * @param [in, out] values A array of double values which shall be leveled
	 * @param [in] count Number of double values in the supplied field
	 */
	void levelColorValues(double *values, int count);

	/**
	 * @brief Comparison method for transparency sorting
	 *
	 * Compares the transparency channel of two polygons. Returns true if
	 * p1 has a greater alpha value than p2.
	 *
	 * @param [in] p1 Polygon 1
	 * @param [in] p2 Polygon 2
	 * @return Alpha channel of p1 > Alpha channel of p2?
	 */
	static bool alphaChannelCompare(const CPolygon *p1, const CPolygon *p2);

	/**
	 * @brief Additional calculations after data parsing
	 *
	 * For advanced features like transparency and (smooth) shading we
	 * need to calculate some additional things like polygon and vertex normals.
	 * We do also sort the polygons for a better transparency effect.
	 */
	void finalize();

	/**
	 * @brief Deletes all vertices and polygons
	 *
	 * Used in the destructor and error cases to clean
	 * up after the loading/parsing of the file was aborted.
	 */
	void cleanup();

	/**
	 * @brief Is the OffScene colored?
	 */
	bool colored;

	/**
	 * @brief Contains the polygons of the OffScene
	 */
	QVector<CPolygon*> polygons;
	
	/**
	 * @brief Contains all vertices of the OffScene
	 */
	QVector<CVertex*> vertices;
	
	/**
	 * @brief Remembers for each vertex the the connected polygons
	 */
	QVector<QVector<CPolygon*> > hintlist;

	/**
	 * @brief Was the loading process of the file canceled?
	 */
	bool canceled;
};
