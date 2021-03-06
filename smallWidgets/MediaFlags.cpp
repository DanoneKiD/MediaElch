#include "MediaFlags.h"
#include "ui_MediaFlags.h"

/**
 * @brief MediaFlags::MediaFlags
 * @param parent
 */
MediaFlags::MediaFlags(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaFlags)
{
    ui->setupUi(this);
    m_height = 14;
}

/**
 * @brief MediaFlags::~MediaFlags
 */
MediaFlags::~MediaFlags()
{
    delete ui;
}

/**
 * @brief MediaFlags::setStreamDetails
 * @param streamDetails
 */
void MediaFlags::setStreamDetails(StreamDetails *streamDetails)
{
    setupResolution(streamDetails);
    setupAspect(streamDetails);
    setupCodec(streamDetails);
    setupAudio(streamDetails);
    setupChannels(streamDetails);
}

/**
 * @brief MediaFlags::setupResolution
 * @param streamDetails
 */
void MediaFlags::setupResolution(StreamDetails *streamDetails)
{
    QString heightFlag;
    int height = streamDetails->videoDetails().value("height").toInt();
    int width = streamDetails->videoDetails().value("width").toInt();
    if (height >= 1072 || width >= 1912) {
        heightFlag = "1080";
    } else if (height >= 712 || width >= 1272) {
        heightFlag = "720";
    } else if (height >= 576) {
        heightFlag = "576";
    } else if (height >= 540) {
        heightFlag = "540";
    } else if (height >= 480) {
        heightFlag = "480";
    }

    ui->mediaFlagResolution->setVisible(heightFlag != "");
    if (heightFlag != "")
        ui->mediaFlagResolution->setPixmap(QPixmap(":/media/resolution/" + heightFlag).scaledToHeight(m_height, Qt::SmoothTransformation));
}

/**
 * @brief MediaFlags::setupAspect
 * @param streamDetails
 */
void MediaFlags::setupAspect(StreamDetails *streamDetails)
{
    QStringList availableAspects = QStringList() << "1.33" << "1.66" << "1.78" << "1.85" << "2.35" << "2.39";
    double aspect = streamDetails->videoDetails().value("aspect").toDouble();
    QString aspectFlag = QString::number(aspect, 'f', 2);
    ui->mediaFlagAspect->setVisible(availableAspects.contains(aspectFlag));
    if (availableAspects.contains(aspectFlag))
        ui->mediaFlagAspect->setPixmap(QPixmap(":/media/aspect/" + aspectFlag).scaledToHeight(m_height, Qt::SmoothTransformation));
}

/**
 * @brief MediaFlags::setupCodec
 * @param streamDetails
 */
void MediaFlags::setupCodec(StreamDetails *streamDetails)
{
    QStringList availableCodecs = QStringList() << "avc1" << "avchd" << "divx" << "flv" << "h264" << "xvid";
    QString codec = streamDetails->videoDetails().value("codec").toLower();
    if (codec.startsWith("divx"))
        codec = "divx";
    if (availableCodecs.contains(codec))
        ui->mediaFlagCodec->setPixmap(QPixmap(":/media/codec/" + codec).scaledToHeight(m_height, Qt::SmoothTransformation));
    ui->mediaFlagCodec->setVisible(availableCodecs.contains(codec));
}

/**
 * @brief MediaFlags::setupAudio
 * @param streamDetails
 */
void MediaFlags::setupAudio(StreamDetails *streamDetails)
{
    bool visible = false;
    QStringList availableCodecs = QStringList() << "dtshdma" << "dolbytruehd" << "dts" << "dolbydigital" << "flac" << "vorbis" << "mp3" << "mp2";
    if (streamDetails->audioDetails().count() > 0) {
        QString codec = streamDetails->audioDetails().at(0).value("codec").toLower();
        if (codec == "dts-hd")
            codec = "dtshdma";
        if (codec == "ac3")
            codec = "dolbydigital";

        if (availableCodecs.contains(codec)) {
            ui->mediaFlagAudio->setPixmap(QPixmap(":/media/audio/" + codec).scaledToHeight(m_height, Qt::SmoothTransformation));
            visible = true;
        }
    }
    ui->mediaFlagAudio->setVisible(visible);
}

/**
 * @brief MediaFlags::setupChannels
 * @param streamDetails
 */
void MediaFlags::setupChannels(StreamDetails *streamDetails)
{
    int channels = -1;
    for (int i=0, n=streamDetails->audioDetails().count() ; i<n ; ++i ) {
        if (streamDetails->audioDetails().at(i).value("channels").toInt() > channels)
            channels = streamDetails->audioDetails().at(i).value("channels").toInt();
    }

    if (channels > 8 || channels < 2 || channels == 3 || channels == 4)
        channels = -1;

    if (channels != -1)
        ui->mediaFlagChannels->setPixmap(QPixmap(QString(":/media/channels/%1").arg(channels)).scaledToHeight(m_height, Qt::SmoothTransformation));
    ui->mediaFlagChannels->setVisible(channels != -1);
}
