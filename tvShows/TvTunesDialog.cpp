#include "TvTunesDialog.h"
#include "ui_TvTunesDialog.h"

#include <QMessageBox>
#include "phonon/AudioOutput"

#include "globals/Manager.h"

TvTunesDialog::TvTunesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TvTunesDialog)
{
    ui->setupUi(this);

    m_totalTime = 0;

#if QT_VERSION >= 0x050000
    ui->results->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    ui->results->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    ui->searchString->setType(MyLineEdit::TypeLoading);

#ifdef Q_OS_MAC
    setWindowFlags((windowFlags() & ~Qt::WindowType_Mask) | Qt::Sheet);
#else
    setWindowFlags((windowFlags() & ~Qt::WindowType_Mask) | Qt::Dialog);
#endif

#ifdef Q_OS_MAC
    QFont font = ui->time->font();
    font.setPointSize(font.pointSize()-1);
    ui->time->setFont(font);
#endif

    m_qnam = new QNetworkAccessManager(this);

    connect(ui->btnClose, SIGNAL(clicked()), this, SLOT(onClose()));
    connect(ui->searchString, SIGNAL(returnPressed()), this, SLOT(onSearch()));
    connect(ui->results, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onResultClicked(QTableWidgetItem*)));
    connect(ui->buttonDownload, SIGNAL(clicked()), this, SLOT(startDownload()));
    connect(ui->buttonCancelDownload, SIGNAL(clicked()), this, SLOT(cancelDownload()));
    connect(Manager::instance()->tvTunes(), SIGNAL(sigSearchDone(QList<ScraperSearchResult>)), this, SLOT(onShowResults(QList<ScraperSearchResult>)));

    m_mediaObject = new Phonon::MediaObject(this);
    m_mediaObject->setTickInterval(1000);
    Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(m_mediaObject, audioOutput);
    ui->seekSlider->setMediaObject(m_mediaObject);

    connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(onStateChanged(Phonon::State)));
    connect(m_mediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(onNewTotalTime(qint64)));
    connect(m_mediaObject, SIGNAL(tick(qint64)), this, SLOT(onTick(qint64)));
    connect(ui->btnPlayPause, SIGNAL(clicked()), this, SLOT(onPlayPause()));
}

TvTunesDialog::~TvTunesDialog()
{
    delete ui;
}

TvTunesDialog *TvTunesDialog::instance(QWidget *parent)
{
    static TvTunesDialog *m_instance = 0;
    if (!m_instance)
        m_instance = new TvTunesDialog(parent);
    return m_instance;
}

void TvTunesDialog::clear()
{
    m_mediaObject->stop();
    ui->btnPlayPause->setEnabled(false);
    ui->buttonDownload->setEnabled(false);
    ui->results->clearContents();
    ui->results->setRowCount(0);
}

void TvTunesDialog::setTvShow(TvShow *show)
{
    m_show = show;
}

int TvTunesDialog::exec()
{
    adjustSize();
    ui->buttonCancelDownload->setVisible(false);
    ui->buttonDownload->setVisible(true);
    ui->progress->clear();
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    m_downloadInProgress = false;
    m_fileDownloaded = false;
    ui->searchString->setText(m_show->name());
    onSearch();
    return QDialog::exec();
}

void TvTunesDialog::onSearch()
{
    clear();
    ui->searchString->setLoading(true);
    Manager::instance()->tvTunes()->search(ui->searchString->text());
}

void TvTunesDialog::onShowResults(QList<ScraperSearchResult> results)
{
    ui->searchString->setLoading(false);
    ui->searchString->setFocus();
    foreach (const ScraperSearchResult &result, results) {
        QTableWidgetItem *item = new QTableWidgetItem(QString("%1").arg(result.name));
        item->setData(Qt::UserRole, result.id);
        int row = ui->results->rowCount();
        ui->results->insertRow(row);
        ui->results->setItem(row, 0, item);
    }
}

void TvTunesDialog::onResultClicked(QTableWidgetItem *item)
{
    m_mediaObject->stop();
    QString url = item->data(Qt::UserRole).toString();
    m_themeUrl = url;
    Phonon::MediaSource source(url);
    m_mediaObject->setCurrentSource(source);
    m_mediaObject->play();
    ui->btnPlayPause->setEnabled(true);
    ui->buttonDownload->setEnabled(true);
}

void TvTunesDialog::onNewTotalTime(qint64 totalTime)
{
    m_totalTime = totalTime;
    updateTime(m_mediaObject->currentTime());
}

void TvTunesDialog::onTick(qint64 time)
{
    updateTime(time);
}

void TvTunesDialog::updateTime(qint64 currentTime)
{
    QString tTime = QString("%1:%2").arg(m_totalTime/1000/60).arg((m_totalTime/1000)%60, 2, 10, QChar('0'));
    QString cTime = QString("%1:%2").arg(currentTime/1000/60).arg((currentTime/1000)%60, 2, 10, QChar('0'));
    ui->time->setText(QString("%1 / %2").arg(cTime).arg(tTime));
}

void TvTunesDialog::onStateChanged(Phonon::State newState)
{
    switch (newState) {
    case Phonon::PlayingState:
    case Phonon::LoadingState:
    case Phonon::BufferingState:
        ui->btnPlayPause->setIcon(QIcon(":/img/video_pause_64.png"));
        break;
    case Phonon::StoppedState:
    case Phonon::PausedState:
    case Phonon::ErrorState:
        ui->btnPlayPause->setIcon(QIcon(":/img/video_play_64.png"));
        break;
    }
}

void TvTunesDialog::onPlayPause()
{
    switch (m_mediaObject->state()) {
    case Phonon::PlayingState:
    case Phonon::LoadingState:
    case Phonon::BufferingState:
        m_mediaObject->pause();
        break;
    case Phonon::StoppedState:
    case Phonon::PausedState:
    case Phonon::ErrorState:
        m_mediaObject->play();
        break;
    }
}

void TvTunesDialog::startDownload()
{
    if (m_show->dir().isEmpty())
        return;

    m_output.setFileName(m_show->dir() + "/theme.mp3.download");

    if (!m_output.open(QIODevice::WriteOnly)) {
        return;
    }

    ui->buttonDownload->setVisible(false);
    ui->buttonCancelDownload->setVisible(true);
    ui->btnClose->setEnabled(false);
    ui->progressBar->setVisible(true);
    ui->searchString->setEnabled(false);
    ui->progress->clear();

    m_downloadInProgress = true;
    m_downloadReply = m_qnam->get(QNetworkRequest(m_themeUrl));
    connect(m_downloadReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(m_downloadReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(m_downloadReply, SIGNAL(readyRead()), SLOT(downloadReadyRead()));
    m_downloadTime.start();
}

void TvTunesDialog::cancelDownload()
{
    ui->buttonDownload->setVisible(true);
    ui->buttonCancelDownload->setVisible(false);
    ui->btnClose->setEnabled(true);
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    ui->progress->clear();
    ui->searchString->setEnabled(true);

    m_downloadReply->abort();
    m_downloadReply->deleteLater();

    m_output.close();
    m_downloadInProgress = false;
}

void TvTunesDialog::downloadProgress(qint64 received, qint64 total)
{
    ui->progressBar->setRange(0, total);
    ui->progressBar->setValue(received);

    double speed = received * 1000.0 / m_downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }
    ui->progress->setText(QString("%1 %2").arg(speed, 3, 'f', 1).arg(unit));
}

void TvTunesDialog::downloadFinished()
{
    if (m_downloadReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 302 ||
        m_downloadReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 301) {
        qDebug() << "Got redirect" << m_downloadReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        m_downloadReply = m_qnam->get(QNetworkRequest(m_downloadReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl()));
        connect(m_downloadReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
        connect(m_downloadReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        connect(m_downloadReply, SIGNAL(readyRead()), SLOT(downloadReadyRead()));
        return;
    }

    m_downloadInProgress = false;
    m_output.close();

    QFile file(m_show->dir() + "/theme.mp3.download");
    if (m_downloadReply->error() == QNetworkReply::NoError) {
        ui->progress->setText(tr("Download Finished"));
        QString newFileName = m_show->dir() + "/theme.mp3";
        QFileInfo fi(newFileName);
        if (fi.exists()) {
            QMessageBox msgBox;
            msgBox.setText(tr("The file %1 already exists.").arg(fi.fileName()));
            msgBox.setInformativeText(tr("Do you want to overwrite it?"));
            msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
            msgBox.setDefaultButton(QMessageBox::Yes);
            if (msgBox.exec() == QMessageBox::Yes) {
                QFile oldFile(newFileName);
                oldFile.remove();
                file.rename(newFileName);
                m_fileDownloaded = true;
            }
        } else {
            file.rename(newFileName);
            m_fileDownloaded = true;
        }
    } else {
        ui->progress->setText(tr("Download Canceled"));
        file.remove();
    }

    ui->buttonDownload->setVisible(true);
    ui->buttonCancelDownload->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->btnClose->setEnabled(true);
    ui->searchString->setEnabled(true);

    m_downloadReply->deleteLater();
}

void TvTunesDialog::downloadReadyRead()
{
    m_output.write(m_downloadReply->readAll());
}

void TvTunesDialog::onClose()
{
    m_mediaObject->stop();
    if (m_downloadInProgress)
        cancelDownload();
    if (m_fileDownloaded)
        QDialog::accept();
    else
        QDialog::reject();
}
