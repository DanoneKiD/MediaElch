#ifndef TVSHOWWIDGETEPISODE_H
#define TVSHOWWIDGETEPISODE_H

#include <QLabel>
#include <QPointer>
#include <QTableWidgetItem>
#include <QWidget>
#include "data/TvShowEpisode.h"
#include "globals/DownloadManager.h"

namespace Ui {
class TvShowWidgetEpisode;
}

/**
 * @brief The TvShowWidgetEpisode class
 */
class TvShowWidgetEpisode : public QWidget
{
    Q_OBJECT

public:
    explicit TvShowWidgetEpisode(QWidget *parent = 0);
    ~TvShowWidgetEpisode();
    void setEpisode(TvShowEpisode *episode);

public slots:
    void onSetEnabled(bool enabled);
    void onClear();
    void onSaveInformation();
    void onStartScraperSearch();

signals:
    void sigSetActionSearchEnabled(bool, MainWidgets);
    void sigSetActionSaveEnabled(bool, MainWidgets);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onAddDirector();
    void onRemoveDirector();
    void onAddWriter();
    void onRemoveWriter();
    void onChooseThumbnail();
    void onDeleteThumbnail();
    void onPosterDownloadFinished(DownloadManagerElement elem);
    void onLoadDone();
    void onRevertChanges();

    void onNameChange(QString text);
    void onShowTitleChange(QString text);
    void onSeasonChange(int value);
    void onEpisodeChange(int value);
    void onDisplaySeasonChange(int value);
    void onDisplayEpisodeChange(int value);
    void onRatingChange(double value);
    void onCertificationChange(QString text);
    void onFirstAiredChange(QDate date);
    void onPlayCountChange(int value);
    void onLastPlayedChange(QDateTime dateTime);
    void onStudioChange(QString text);
    void onEpBookmarkChange(QTime time);
    void onOverviewChange();
    void onDirectorEdited(QTableWidgetItem *item);
    void onWriterEdited(QTableWidgetItem *item);
    void onStreamDetailsEdited();
    void onReloadStreamDetails();
    void updateStreamDetails(bool reloadFromFile = false);

private:
    Ui::TvShowWidgetEpisode *ui;
    QPointer<TvShowEpisode> m_episode;
    QLabel *m_savingWidget;
    QMovie *m_loadingMovie;
    DownloadManager *m_posterDownloadManager;
    QList<QWidget*> m_streamDetailsWidgets;
    QList< QList<QLineEdit*> > m_streamDetailsAudio;
    QList< QList<QLineEdit*> > m_streamDetailsSubtitles;

    void updateEpisodeInfo();
};

#endif // TVSHOWWIDGETEPISODE_H
