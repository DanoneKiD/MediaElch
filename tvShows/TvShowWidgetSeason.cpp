#include "TvShowWidgetSeason.h"
#include "ui_TvShowWidgetSeason.h"

#include <QPainter>
#include "data/ImageCache.h"
#include "globals/Helper.h"
#include "globals/ImageDialog.h"
#include "globals/ImagePreviewDialog.h"
#include "globals/Manager.h"
#include "main/MessageBox.h"

TvShowWidgetSeason::TvShowWidgetSeason(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TvShowWidgetSeason)
{
    ui->setupUi(this);
    m_show = 0;
    m_season = -1;

    ui->title->clear();
    QFont font = ui->title->font();
    font.setPointSize(font.pointSize()+4);
    ui->title->setFont(font);

    font = ui->labelPoster->font();
    #ifdef Q_OS_WIN32
        font.setPointSize(font.pointSize()-1);
    #else
        font.setPointSize(font.pointSize()-2);
    #endif

    font.setBold(true);
    ui->labelFanart->setFont(font);
    ui->labelBanner->setFont(font);
    ui->labelPoster->setFont(font);
    ui->labelThumb->setFont(font);

    ui->poster->setDefaultPixmap(QPixmap(":/img/film_reel.png"));
    ui->backdrop->setDefaultPixmap(QPixmap(":/img/pictures_alt.png").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->banner->setDefaultPixmap(QPixmap(":/img/pictures_alt.png").scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->thumb->setDefaultPixmap(QPixmap(":/img/pictures_alt.png").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    m_downloadManager = new DownloadManager(this);

    m_loadingMovie = new QMovie(":/img/spinner.gif");
    m_loadingMovie->start();

    m_savingWidget = new QLabel(this);
    m_savingWidget->setMovie(m_loadingMovie);
    m_savingWidget->hide();

    onSetEnabled(false);
    onClear();

    QPainter p;
    QPixmap revert(":/img/arrow_circle_left.png");
    p.begin(&revert);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(revert.rect(), QColor(0, 0, 0, 200));
    p.end();
    ui->buttonRevert->setIcon(QIcon(revert));
    ui->buttonRevert->setVisible(false);

    ui->poster->setImageType(ImageType::TvShowSeasonPoster);
    ui->backdrop->setImageType(ImageType::TvShowSeasonBackdrop);
    ui->banner->setImageType(ImageType::TvShowSeasonBanner);
    ui->thumb->setImageType(ImageType::TvShowSeasonThumb);
    foreach (ClosableImage *image, ui->groupBox_3->findChildren<ClosableImage*>()) {
        connect(image, SIGNAL(clicked()), this, SLOT(onChooseImage()));
        connect(image, SIGNAL(sigClose()), this, SLOT(onDeleteImage()));
    }

    connect(ui->buttonRevert, SIGNAL(clicked()), this, SLOT(onRevertChanges()));
    connect(m_downloadManager, SIGNAL(downloadFinished(DownloadManagerElement)), this, SLOT(onDownloadFinished(DownloadManagerElement)));
}

TvShowWidgetSeason::~TvShowWidgetSeason()
{
    delete ui;
}

void TvShowWidgetSeason::resizeEvent(QResizeEvent *event)
{
    m_savingWidget->move(size().width()/2-m_savingWidget->width(), height()/2-m_savingWidget->height());
    QWidget::resizeEvent(event);
}

void TvShowWidgetSeason::setSeason(TvShow *show, int season)
{
    onClear();

    m_show = show;
    m_season = season;

    emit sigSetActionSearchEnabled(false, WidgetTvShows);
    ui->title->setText(QString(show->name()) + " - " + tr("Season %1").arg(season));

    updateImages(QList<int>() << ImageType::TvShowSeasonPoster << ImageType::TvShowSeasonBackdrop << ImageType::TvShowSeasonBanner << ImageType::TvShowSeasonThumb);

    onSetEnabled(!show->downloadsInProgress());
    emit sigSetActionSaveEnabled(!show->downloadsInProgress(), WidgetTvShows);
}

void TvShowWidgetSeason::updateImages(QList<int> images)
{
    foreach (const int &imageType, images) {
        ClosableImage *image = 0;

        foreach (ClosableImage *cImage, ui->groupBox_3->findChildren<ClosableImage*>()) {
            if (cImage->imageType() == imageType)
                image = cImage;
        }

        if (!image)
            continue;

        if (!m_show->seasonImage(m_season, imageType).isNull())
            image->setImage(m_show->seasonImage(m_season, imageType));
        else if (!Manager::instance()->mediaCenterInterfaceTvShow()->imageFileName(m_show, imageType, m_season).isEmpty() &&
                (!m_show->imagesToRemove().contains(imageType) || !m_show->imagesToRemove().value(imageType).contains(m_season)))
            image->setImage(Manager::instance()->mediaCenterInterfaceTvShow()->imageFileName(m_show, imageType, m_season));
    }
}

void TvShowWidgetSeason::onClear()
{
    ui->title->clear();
    ui->poster->clear();
    ui->backdrop->clear();
    ui->banner->clear();
    ui->thumb->clear();
    ui->buttonRevert->setVisible(false);
}

void TvShowWidgetSeason::onSaveInformation()
{
    if (!m_show)
        return;
    onSetEnabled(false);
    m_savingWidget->show();
    m_show->saveData(Manager::instance()->mediaCenterInterfaceTvShow());
    m_savingWidget->hide();
    onSetEnabled(true);
    MessageBox::instance()->showMessage(tr("<b>\"%1\"</b> Saved").arg(m_show->name()));
}

void TvShowWidgetSeason::onSetEnabled(bool enabled)
{
    ui->groupBox_3->setEnabled(enabled);
}

void TvShowWidgetSeason::onRevertChanges()
{
    // @todo: implement
}

void TvShowWidgetSeason::onDownloadFinished(DownloadManagerElement elem)
{
    foreach (ClosableImage *image, ui->groupBox_3->findChildren<ClosableImage*>()) {
        if (image->imageType() == elem.imageType) {
            if (elem.imageType == ImageType::TvShowSeasonBackdrop)
                Helper::resizeBackdrop(elem.data);
            if (m_show == elem.show)
                image->setImage(elem.data);
            ImageCache::instance()->invalidateImages(Manager::instance()->mediaCenterInterface()->imageFileName(elem.show, elem.imageType, elem.season));
            elem.show->setSeasonImage(elem.season, elem.imageType, elem.data);
            break;
        }
    }

    if (m_downloadManager->downloadsLeftForShow(m_show) == 0)
        emit sigSetActionSaveEnabled(true, WidgetTvShows);
}

void TvShowWidgetSeason::onChooseImage()
{
    if (m_show == 0)
        return;

    ClosableImage *image = static_cast<ClosableImage*>(QObject::sender());
    if (!image)
        return;

    ImageDialog::instance()->setImageType(image->imageType());
    ImageDialog::instance()->clear();
    ImageDialog::instance()->setTvShow(m_show);
    ImageDialog::instance()->setSeason(m_season);
    if (image->imageType() == ImageType::TvShowSeasonPoster)
        ImageDialog::instance()->setDownloads(m_show->seasonPosters(m_season));
    else if (image->imageType() == ImageType::TvShowSeasonBackdrop)
        ImageDialog::instance()->setDownloads(m_show->seasonBackdrops(m_season));
    else if (image->imageType() == ImageType::TvShowSeasonBanner)
        ImageDialog::instance()->setDownloads(m_show->seasonBanners(m_season));
    else
        ImageDialog::instance()->setDownloads(QList<Poster>());
    ImageDialog::instance()->exec(image->imageType());

    if (ImageDialog::instance()->result() == QDialog::Accepted) {
        emit sigSetActionSaveEnabled(false, WidgetTvShows);
        DownloadManagerElement d;
        d.imageType = image->imageType();
        d.url = ImageDialog::instance()->imageUrl();
        d.season = m_season;
        d.show = m_show;
        m_downloadManager->addDownload(d);
        image->setLoading(true);
    }
}

void TvShowWidgetSeason::onDeleteImage()
{
    if (m_show == 0)
        return;

    ClosableImage *image = static_cast<ClosableImage*>(QObject::sender());
    if (!image)
        return;

    m_show->removeImage(image->imageType(), m_season);
    updateImages(QList<int>() << image->imageType());
}
