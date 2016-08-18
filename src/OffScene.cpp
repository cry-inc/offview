#include <QProgressDialog>

#include "OffScene.h"

OffScene::OffScene(const QString & fileName)
{
	colored = false;
	canceled = false;
	parseFile(fileName);
}

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

QStringList OffScene::split2Token(QString *string)
{
	string->replace('\t', ' ');
	return string->split(' ', QString::SkipEmptyParts);
}

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

void OffScene::cancel()
{
	canceled = true;
}

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
