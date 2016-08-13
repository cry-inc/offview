/**
 * @file OffScene.cpp
 * @see OffScene
 * @see IScene
 * @author B. Stauder
 * @author M. Caputo
 * @date 2010-03-17
 */

#include <QProgressDialog>

#include "OffScene.h"

/**
 * @brief Constructor
 *
 * Constructor of OffScene, delegates almost all work to parseFile().
 *
 * @param [in] fileName The OFF file which should get parsed
 */
OffScene::OffScene(const QString & fileName)
{
	colored = false;
	canceled = false;
	parseFile(fileName);
}

/**
 * @brief Destructor
 *
 * Destructor of OffScene
 */
OffScene::~OffScene()
{
	cleanup();
}

bool OffScene::isColored() const
{
	return colored;
}

int OffScene::polygonsCount() const
{
	return polygons.size();
}

const CPolygon* OffScene::polygon(int i) const
{
	return polygons[i];
}

int OffScene::verticesCount() const
{
	return vertices.size();
}

const CVertex* OffScene::vertex(int i) const
{
	return vertices[i];
}

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
QString OffScene::readNextLine(QTextStream *stream)
{
	QString str;
	
	while (str.isEmpty()) {
		
		if (stream->atEnd()) {
			throw tr("Unexpected end of file!");
		}
		
		str = stream->readLine();
		
		int commentStart = str.indexOf("#");
		if (commentStart != -1) {
			str.truncate(commentStart);
		}
		str = str.trimmed();
	}
	
	return str;
}

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
QStringList OffScene::split2Token(QString *string)
{
	string->replace('\t', ' ');
	return string->split(' ', QString::SkipEmptyParts);
}

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
void OffScene::levelColorValues(double *values, int count)
{
	double max = 1;
	for(int i=0; i<count; i++) {
		if (values[i] > max) {
			max = values[i];
		}
	}
	
	if (max > 1) {
		for(int i=0; i<count; i++) {
			values[i] /= 255.0;
		}
	}
}

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
QColor OffScene::readColor(QStringList *tokens, int offset)
{
	QColor color;
	
	int colorToken = tokens->size() - offset;
	if (colorToken < 3 || colorToken > 4) {
		return color;
	}

	bool ok;
	double values[4];
	for(int i=0; i<colorToken; i++) {
		values[i] = tokens->at(offset+i).toDouble(&ok);
		if (!ok || values[i] < 0 || values[i] > 255) {
			throw tr("Invalid color value!");
		}
	}
	
	levelColorValues(values, colorToken);
	color.setRedF(values[0]);
	color.setGreenF(values[1]);
	color.setBlueF(values[2]);
	
	if (colorToken > 3) {
		color.setAlphaF(values[3]);
	} else {
		color.setAlphaF(1.0);
	}

	return color;
}

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
CVertex* OffScene::readVertex(QStringList *tokens)
{
	if (tokens->size() < 3) {
		throw tr("Can't find all three vertex components!");
	}
	
	float xyz[3];
	bool ok;
	
	for(int i=0; i<3; i++) {
		xyz[i] = tokens->at(i).toDouble(&ok);
		if (!ok) {
			throw tr("Can't parse vertex data!");
		}
	}
	
	CVertex *vertex = new CVertex(xyz);
	
	QColor color = readColor(tokens, 3);
	if (color.isValid()) {
		vertex->setColor(color);
		colored = true;
	}
	
	return vertex;
}

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
CPolygon* OffScene::readPolygon(QStringList *tokens, const QVector<CVertex*> vertices)
{
	if (tokens->size() < 4) {
		throw tr("A polygon line has less than 4 components!");
	}
	
	bool ok;
	int vCount = tokens->at(0).toInt(&ok);
	if (!ok || vCount < 3) {
		throw tr("A polygon references less than 3 vertices!");
	}
	
	CPolygon *polygon = new CPolygon();
	for(int i=0; i<vCount; i++) {
		int index = tokens->at(1+i).toInt(&ok);
		if (!ok) {
			throw tr("Can't parse polygon data!");
		}
		polygon->addVertex(vertices[index]);
		hintlist[index].append(polygon);
	}
	
	QColor color = readColor(tokens, vCount+1);
	if (color.isValid()) {
		polygon->setColor(color);
		colored = true;
	}

	return polygon;
}

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
void OffScene::parseFile(const QString & fileName)
{
	QFile file(fileName);
    QTextStream stream(&file);
	
    // Open the file in read only mode
    if(!file.open(QIODevice::ReadOnly)) {
        throw tr("Unable to open file ") + fileName;
    }

    // Check if the file has "OFF" at the beginning
    QString line = readNextLine(&stream);
    if(line != "OFF" && line != "COFF") {
        throw tr("Wrong file format!");
    }
	
	// Read the line with the vertex and polygon numbers
	line = readNextLine(&stream);
	QStringList tokens = split2Token(&line);
	if (tokens.size() != 3) {
		throw tr("Can't read vertex, polygon and edge numbers!");
	}
	
	// Convert vertex and polygon numbers to int and check
	int vCount = tokens[0].toInt();
	int pCount = tokens[1].toInt();
	if (vCount == 0 || pCount == 0) {
		throw tr("Invalid vertex or polygon number!");
	}

	// Step size for progress bar
	int stepSize = (vCount+pCount)/50;
	stepSize = stepSize<1 ? 1 : stepSize;
	
	// Create a progress dialog
	QProgressDialog progress(tr("Loading vertices..."), tr("Cancel"), 0, vCount+pCount);
	progress.setWindowTitle(tr("Loading OFF file"));
	progress.setWindowModality(Qt::ApplicationModal);
	progress.setWindowFlags(Qt::Tool);
	connect(&progress, SIGNAL(canceled()), this, SLOT(cancel()));

	// Read all vertices into an temporary field
	for(int i=0; i<vCount && !canceled; i++) {
		line = readNextLine(&stream);
		tokens = split2Token(&line);
		vertices.append(readVertex(&tokens));
		hintlist.append(QVector<CPolygon*>());
		if (i%stepSize == 0) {
			progress.setValue(i);
		}
	}

	// Construct the polygons with the indices for the vertices
	progress.setLabelText(tr("Loading polygons..."));
	for(int i=0; i<pCount && !canceled; i++) {
		line = readNextLine(&stream);
		tokens = split2Token(&line);
		polygons.append(readPolygon(&tokens, vertices));
		if (i%stepSize == 0) {
			progress.setValue(vCount+i);
		}
	}
	
	file.close();
	if (canceled) {
		cleanup();
		throw tr("Aborted file loading!");
	} else {
		finalize();
	}
	progress.setValue(vCount+pCount);
}

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
bool OffScene::alphaChannelCompare(const CPolygon *p1, const CPolygon *p2)
{
	if (p1->isColored() && p2->isColored()) {
		return p1->color().alpha() > p2->color().alpha();
	}
	
	// Needed to sort polygons without color before transparent ones
	if (p2->isColored()) {
		return true;
	}

	return false;
}

/**
 * @brief Cancels the loading process with an exception
 *
 * If the user clicks on the cancel button during a file loading
 * process, this method is called and will cancel the loading process.
 */
void OffScene::cancel()
{
	canceled = true;
}

/**
 * @brief Additional calculations after data parsing
 *
 * For advanced features like transparency and (smooth) shading we
 * need to calculate some additional things like polygon and vertex normals.
 * We do also sort the polygons for a better transparency effect.
 */
void OffScene::finalize()
{
	// Sort polygons for a better (but not perfect) transparency effect
	// -> Solid polygons should be drawn first
	qSort(polygons.begin(), polygons.end(), alphaChannelCompare);
	
	// calculate normal vectors for all polygons
	int pCount = polygons.size();
	for(int i=0; i<pCount; i++) {
		polygons[i]->calculateNormal();
	}

	// calculate normal vectors for each vertex. the normal vector for a 
	// vertex is the average of all connected polygon normal vectors
	int vCount = vertices.size();
	for(int i=0; i<vCount; i++) {
		float normal[3] = {0.0f, 0.0f, 0.0f};
		int count = hintlist[i].size();
		for(int j=0; j<count; j++) {
			const float *nv = hintlist[i].at(j)->normal();
			normal[0] += nv[0];
			normal[1] += nv[1];
			normal[2] += nv[2];
		}
		normal[0] /= count;
		normal[1] /= count;
		normal[2] /= count;
		vertices[i]->setNormal(normal);
	}
}

/**
 * @brief Deletes all vertices and polygons
 *
 * Used in the destructor and error cases to clean
 * up after the loading/parsing of the file was aborted.
 */
void OffScene::cleanup()
{
	for(int i=0; i<polygons.size(); i++) {
		delete polygons[i];
	}
	polygons.clear();

	for(int i=0; i<vertices.size(); i++) {
		delete vertices[i];
	}
	vertices.clear();
}
