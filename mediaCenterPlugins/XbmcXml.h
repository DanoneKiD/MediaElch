#ifndef XBMCXML_H
#define XBMCXML_H

#include <QDomDocument>
#include <QObject>
#include <QXmlStreamWriter>

#include "data/Concert.h"
#include "data/MediaCenterInterface.h"
#include "movies/Movie.h"
#include "data/TvShow.h"
#include "data/TvShowEpisode.h"

/**
 * @brief The XbmcXml class
 */
class XbmcXml : public MediaCenterInterface
{
    Q_OBJECT
public:
    explicit XbmcXml(QObject *parent = 0);
    ~XbmcXml();

    bool saveMovie(Movie *movie);
    bool loadMovie(Movie *movie, QString initialNfoContent = "");
    bool saveConcert(Concert *concert);
    bool loadConcert(Concert *concert, QString initialNfoContent = "");
    void loadConcertImages(Concert *concert);
    bool loadTvShow(TvShow *show, QString initialNfoContent = "");
    bool loadTvShowEpisode(TvShowEpisode *episode, QString initialNfoContent = "");
    bool saveTvShow(TvShow *show);
    bool saveTvShowEpisode(TvShowEpisode *episode);
    bool hasFeature(int feature);
    QStringList extraFanartNames(Movie *movie);
    QStringList extraFanartNames(Concert *concert);
    QStringList extraFanartNames(TvShow *show);
    QImage movieSetPoster(QString setName);
    QImage movieSetBackdrop(QString setName);
    void saveMovieSetPoster(QString setName, QImage poster);
    void saveMovieSetBackdrop(QString setName, QImage backdrop);

    QString actorImageName(Movie *movie, Actor actor);
    QString actorImageName(TvShow *show, Actor actor);

    QString imageFileName(Movie *movie, int type, QList<DataFile> dataFiles = QList<DataFile>(), bool constructName = false);
    QString imageFileName(Concert *concert, int type, QList<DataFile> dataFiles = QList<DataFile>(), bool constructName = false);
    QString imageFileName(TvShowEpisode *episode, int type, QList<DataFile> dataFiles = QList<DataFile>(), bool constructName = false);
    QString imageFileName(TvShow *show, int type, int season = -1, QList<DataFile> dataFiles = QList<DataFile>(), bool constructName = false);

    QString nfoFilePath(Movie *movie);
    QString nfoFilePath(Concert *concert);
    QString nfoFilePath(TvShowEpisode *episode);

private:
    void writeMovieXml(QXmlStreamWriter &xml, Movie *movie);
    void writeConcertXml(QXmlStreamWriter &xml, Concert *concert);
    void writeTvShowXml(QXmlStreamWriter &xml, TvShow *show);
    void writeTvShowEpisodeXml(QXmlStreamWriter &xml, TvShowEpisode *episode);
    void writeStreamDetails(QXmlStreamWriter &xml, StreamDetails *streamDetails);
    bool loadStreamDetails(StreamDetails *streamDetails, QDomDocument domDoc);
    void loadStreamDetails(StreamDetails *streamDetails, QDomElement elem);
    bool saveFile(QString filename, QByteArray data);
    QString getPath(Movie *movie);
    QString getPath(Concert *concert);
    QString movieSetFileName(QString setName, DataFile *dataFile);
};

#endif // XBMCXML_H
