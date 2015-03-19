/**
 * @class OffScene
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

#ifndef OFFSCENE_H
#define OFFSCENE_H

#include <QtCore>

#include "IScene.h"
#include "CPolygon.h"

class OffScene : public QObject, public IScene
{

	Q_OBJECT

public:
	OffScene(const QString & fileName);
	~OffScene();
	
	bool isColored() const override;
	int polygonsCount() const override;
	const CPolygon* polygon(int i) const override;
	int verticesCount() const override;
	const CVertex* vertex(int i) const override;

private slots:
	void cancel();
	
private:

	void parseFile(const QString & fileName);
	QString readNextLine(QTextStream *stream);
	QStringList split2Token(QString *string);
	CVertex* readVertex(QStringList *tokens);
	CPolygon* readPolygon(QStringList *tokens, const QVector<CVertex*> vertices);
	QColor readColor(QStringList *tokens, int offset);
	void levelColorValues(double *values, int count);
	static bool alphaChannelCompare(const CPolygon *p1, const CPolygon *p2);
	void finalize();
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

#endif
