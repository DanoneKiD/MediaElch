#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include <QAction>
#include <QDebug>
#include "data/MovieFilesOrganizer.h"
#include "export/ExportTemplateLoader.h"
#include "globals/Manager.h"
#include "main/MessageBox.h"
#include "settings/DataFile.h"
#include "settings/ExportTemplateWidget.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
#ifdef Q_OS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
#endif

#ifdef Q_OS_MAC
    QFont smallFont = ui->labelGlobal->font();
    smallFont.setPointSize(smallFont.pointSize()-1);
    ui->labelGlobal->setFont(smallFont);
    ui->label_44->setFont(smallFont);
    ui->label_45->setFont(smallFont);
    ui->label_46->setFont(smallFont);
    ui->label_47->setFont(smallFont);
    ui->label_48->setFont(smallFont);
    ui->label_49->setFont(smallFont);
    ui->label_7->setFont(smallFont);
    ui->label_18->setFont(smallFont);
#endif

    ui->customScraperTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->customScraperTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->customScraperTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    ui->actionGlobal->setIcon(ui->actionGlobal->property("iconActive").value<QIcon>());
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget->setAnimation(QEasingCurve::Linear);
    ui->stackedWidget->setSpeed(200);

    m_settings = Settings::instance(this);

    ui->xbmcPort->setValidator(new QIntValidator(0, 99999, ui->xbmcPort));
    ui->dirs->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->dirs->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    ui->exportTemplates->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    int scraperCounter = 0;
    foreach (ScraperInterface *scraper, Manager::instance()->scrapers()) {
        if (scraper->hasSettings()) {
            if (scraperCounter++ > 0) {
                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                ui->verticalLayoutScrapers->addWidget(line);
            }
            ui->verticalLayoutScrapers->addWidget(new QLabel(scraper->name()));
            ui->verticalLayoutScrapers->addWidget(scraper->settingsWidget());
        }
    }
    foreach (TvScraperInterface *scraper, Manager::instance()->tvScrapers()) {
        if (scraper->hasSettings()) {
            if (scraperCounter++ > 0) {
                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                ui->verticalLayoutScrapers->addWidget(line);
            }
            ui->verticalLayoutScrapers->addWidget(new QLabel(scraper->name()));
            ui->verticalLayoutScrapers->addWidget(scraper->settingsWidget());
        }
    }
    foreach (ConcertScraperInterface *scraper, Manager::instance()->concertScrapers()) {
        if (scraper->hasSettings()) {
            if (scraperCounter++ > 0) {
                QFrame *line = new QFrame();
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                ui->verticalLayoutScrapers->addWidget(line);
            }
            ui->verticalLayoutScrapers->addWidget(new QLabel(scraper->name()));
            ui->verticalLayoutScrapers->addWidget(scraper->settingsWidget());
        }
    }

    ui->comboMovieSetArtwork->setItemData(0, MovieSetArtworkSingleSetFolder);
    ui->comboMovieSetArtwork->setItemData(1, MovieSetArtworkSingleArtworkFolder);

    connect(ui->buttonAddDir, SIGNAL(clicked()), this, SLOT(chooseDirToAdd()));
    connect(ui->buttonRemoveDir, SIGNAL(clicked()), this, SLOT(removeDir()));
    connect(ui->buttonMovieFilesToDirs, SIGNAL(clicked()), this, SLOT(organize()));
    connect(ui->dirs, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(dirListRowChanged(int)));
    connect(ui->comboMovieSetArtwork, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboMovieSetArtworkChanged()));
    connect(ui->btnMovieSetArtworkDir, SIGNAL(clicked()), this, SLOT(onChooseMovieSetArtworkDir()));
    connect(ui->chkUseProxy, SIGNAL(clicked()), this, SLOT(onUseProxy()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(ExportTemplateLoader::instance(this), SIGNAL(sigTemplatesLoaded(QList<ExportTemplate*>)), this, SLOT(onTemplatesLoaded(QList<ExportTemplate*>)));
    connect(ExportTemplateLoader::instance(this), SIGNAL(sigTemplateInstalled(ExportTemplate*,bool)), this, SLOT(onTemplateInstalled(ExportTemplate*,bool)));
    connect(ExportTemplateLoader::instance(this), SIGNAL(sigTemplateUninstalled(ExportTemplate*,bool)), this, SLOT(onTemplateUninstalled(ExportTemplate*,bool)));

    ui->movieNfo->setProperty("dataFileType", DataFileType::MovieNfo);
    ui->moviePoster->setProperty("dataFileType", DataFileType::MoviePoster);
    ui->movieBackdrop->setProperty("dataFileType", DataFileType::MovieBackdrop);
    ui->movieCdArt->setProperty("dataFileType", DataFileType::MovieCdArt);
    ui->movieClearArt->setProperty("dataFileType", DataFileType::MovieClearArt);
    ui->movieLogo->setProperty("dataFileType", DataFileType::MovieLogo);
    ui->movieBanner->setProperty("dataFileType", DataFileType::MovieBanner);
    ui->movieThumb->setProperty("dataFileType", DataFileType::MovieThumb);
    ui->movieSetPosterFileName->setProperty("dataFileType", DataFileType::MovieSetPoster);
    ui->movieSetFanartFileName->setProperty("dataFileType", DataFileType::MovieSetBackdrop);
    ui->showBackdrop->setProperty("dataFileType", DataFileType::TvShowBackdrop);
    ui->showBanner->setProperty("dataFileType", DataFileType::TvShowBanner);
    ui->showCharacterArt->setProperty("dataFileType", DataFileType::TvShowCharacterArt);
    ui->showClearArt->setProperty("dataFileType", DataFileType::TvShowClearArt);
    ui->showEpisodeNfo->setProperty("dataFileType", DataFileType::TvShowEpisodeNfo);
    ui->showEpisodeThumbnail->setProperty("dataFileType", DataFileType::TvShowEpisodeThumb);
    ui->showLogo->setProperty("dataFileType", DataFileType::TvShowLogo);
    ui->showThumb->setProperty("dataFileType", DataFileType::TvShowThumb);
    ui->showNfo->setProperty("dataFileType", DataFileType::TvShowNfo);
    ui->showPoster->setProperty("dataFileType", DataFileType::TvShowPoster);
    ui->showSeasonBackdrop->setProperty("dataFileType", DataFileType::TvShowSeasonBackdrop);
    ui->showSeasonBanner->setProperty("dataFileType", DataFileType::TvShowSeasonBanner);
    ui->showSeasonPoster->setProperty("dataFileType", DataFileType::TvShowSeasonPoster);
    ui->showSeasonThumb->setProperty("dataFileType", DataFileType::TvShowSeasonThumb);
    ui->concertNfo->setProperty("dataFileType", DataFileType::ConcertNfo);
    ui->concertPoster->setProperty("dataFileType", DataFileType::ConcertPoster);
    ui->concertBackdrop->setProperty("dataFileType", DataFileType::ConcertBackdrop);
    ui->concertLogo->setProperty("dataFileType", DataFileType::ConcertLogo);
    ui->concertClearArt->setProperty("dataFileType", DataFileType::ConcertClearArt);
    ui->concertDiscArt->setProperty("dataFileType", DataFileType::ConcertCdArt);

#ifdef Q_OS_MAC
    ui->btnCancel->setVisible(false);
    ui->btnSave->setVisible(false);
    ui->horizontalSpacerButtons->setGeometry(QRect(0, 0, 1, 1));
#endif

    loadSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::show()
{
    ui->themesErrorMessage->setText("");
    loadRemoteTemplates();
    loadSettings();
    if (Settings::instance()->settingsWindowSize().isValid() && !Settings::instance()->settingsWindowPosition().isNull()) {
        move(Settings::instance()->settingsWindowPosition());
        resize(Settings::instance()->settingsWindowSize());
    }
    QMainWindow::show();
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
#ifdef Q_OS_MAC
    saveSettings();
    emit sigSaved();
#endif

    Settings::instance()->setSettingsWindowSize(size());
    Settings::instance()->setSettingsWindowPosition(pos());
}

void SettingsWindow::onSave()
{
    saveSettings();
    close();
    emit sigSaved();
}

void SettingsWindow::onCancel()
{
    m_settings->loadSettings();
    close();
}

void SettingsWindow::onAction()
{
    QAction *triggeredAction = static_cast<QAction*>(sender());
    foreach (QAction *action, ui->toolBar->actions())
        action->setIcon(action->property("iconNormal").value<QIcon>());
    triggeredAction->setIcon(triggeredAction->property("iconActive").value<QIcon>());
    ui->stackedWidget->slideInIdx(triggeredAction->property("page").toInt());
}

void SettingsWindow::loadSettings()
{
    m_settings->loadSettings();

    // Stream Details
    ui->chkAutoLoadStreamDetails->setChecked(m_settings->autoLoadStreamDetails());

    // Proxy
    ui->chkUseProxy->setChecked(m_settings->useProxy());
    ui->proxyType->setCurrentIndex(m_settings->proxyType());
    ui->proxyHost->setText(m_settings->proxyHost());
    ui->proxyPort->setValue(m_settings->proxyPort());
    ui->proxyUsername->setText(m_settings->proxyUsername());
    ui->proxyPassword->setText(m_settings->proxyPassword());
    onUseProxy();

    ui->usePlotForOutline->setChecked(m_settings->usePlotForOutline());
    ui->chkDownloadActorImages->setChecked(m_settings->downloadActorImages());
    ui->chkIgnoreArticlesWhenSorting->setChecked(m_settings->ignoreArticlesWhenSorting());

    // Directories
    ui->dirs->setRowCount(0);
    ui->dirs->clearContents();
    QList<SettingsDir> movieDirectories = m_settings->movieDirectories();
    for (int i=0, n=movieDirectories.count() ; i<n ; ++i)
        addDir(movieDirectories.at(i).path, movieDirectories.at(i).separateFolders, movieDirectories.at(i).autoReload, DirTypeMovies);
    QList<SettingsDir> tvShowDirectories = m_settings->tvShowDirectories();
    for (int i=0, n=tvShowDirectories.count() ; i<n ; ++i)
        addDir(tvShowDirectories.at(i).path, tvShowDirectories.at(i).separateFolders, tvShowDirectories.at(i).autoReload, DirTypeTvShows);
    QList<SettingsDir> concertDirectories = m_settings->concertDirectories();
    for (int i=0, n=concertDirectories.count() ; i<n ; ++i)
        addDir(concertDirectories.at(i).path, concertDirectories.at(i).separateFolders, concertDirectories.at(i).autoReload, DirTypeConcerts);

    dirListRowChanged(ui->dirs->currentRow());

    // Exclude words
    ui->excludeWordsText->setPlainText(m_settings->excludeWords());

    ui->useYoutubePluginUrls->setChecked(m_settings->useYoutubePluginUrls());

    // XBMC
    ui->xbmcHost->setText(m_settings->xbmcHost());
    if (m_settings->xbmcPort() != 0)
        ui->xbmcPort->setText(QString::number(m_settings->xbmcPort()));
    else
        ui->xbmcPort->clear();

    foreach (QLineEdit *lineEdit, findChildren<QLineEdit*>()) {
        if (lineEdit->property("dataFileType").isNull())
            continue;
        int dataFileType = lineEdit->property("dataFileType").toInt();
        QList<DataFile> dataFiles = m_settings->dataFiles(dataFileType);
        QStringList filenames;
        foreach (DataFile dataFile, dataFiles)
            filenames << dataFile.fileName();
        lineEdit->setText(filenames.join(","));
    }

    // Movie set artwork
    for (int i=0, n=ui->comboMovieSetArtwork->count() ; i<n ; ++i) {
        if (ui->comboMovieSetArtwork->itemData(i).toInt() == m_settings->movieSetArtworkType()) {
            ui->comboMovieSetArtwork->setCurrentIndex(i);
            break;
        }
    }
    ui->movieSetArtworkDir->setText(m_settings->movieSetArtworkDirectory());
    onComboMovieSetArtworkChanged();


    QList<int> infos = QList<int>() << MovieScraperInfos::Title << MovieScraperInfos::Set << MovieScraperInfos::Tagline
                                    << MovieScraperInfos::Rating << MovieScraperInfos::Released << MovieScraperInfos::Runtime
                                    << MovieScraperInfos::Director << MovieScraperInfos::Writer << MovieScraperInfos::Certification
                                    << MovieScraperInfos::Trailer << MovieScraperInfos::Overview << MovieScraperInfos::Poster
                                    << MovieScraperInfos::Backdrop << MovieScraperInfos::Actors << MovieScraperInfos::Genres
                                    << MovieScraperInfos::Studios << MovieScraperInfos::Countries << MovieScraperInfos::Logo
                                    << MovieScraperInfos::ClearArt << MovieScraperInfos::CdArt << MovieScraperInfos::Banner
                                    << MovieScraperInfos::Thumb;

    ui->customScraperTable->clearContents();
    ui->customScraperTable->setRowCount(0);

    foreach (const int &info, infos) {
        int row = ui->customScraperTable->rowCount();
        ui->customScraperTable->insertRow(row);
        ui->customScraperTable->setItem(row, 0, new QTableWidgetItem(titleForMovieScraperInfo(info)));
        ui->customScraperTable->setCellWidget(row, 1, comboForMovieScraperInfo(info));
    }
}

void SettingsWindow::saveSettings()
{
    QList<DataFile> dataFiles;
    foreach (QLineEdit *lineEdit, findChildren<QLineEdit*>()) {
        if (lineEdit->property("dataFileType").isNull())
            continue;
        int pos = 0;
        int dataFileType = lineEdit->property("dataFileType").toInt();
        QStringList filenames = lineEdit->text().split(",", QString::SkipEmptyParts);
        foreach (const QString &filename, filenames) {
            DataFile df(dataFileType, filename.trimmed(), pos++);
            dataFiles << df;
        }
    }
    m_settings->setDataFiles(dataFiles);

    m_settings->setUseYoutubePluginUrls(ui->useYoutubePluginUrls->isChecked());
    m_settings->setAutoLoadStreamDetails(ui->chkAutoLoadStreamDetails->isChecked());
    m_settings->setDownloadActorImages(ui->chkDownloadActorImages->isChecked());
    m_settings->setIgnoreArticlesWhenSorting(ui->chkIgnoreArticlesWhenSorting->isChecked());

    m_settings->setXbmcHost(ui->xbmcHost->text());
    m_settings->setXbmcPort(ui->xbmcPort->text().toInt());

    // Proxy
    m_settings->setUseProxy(ui->chkUseProxy->isChecked());
    m_settings->setProxyType(ui->proxyType->currentIndex());
    m_settings->setProxyHost(ui->proxyHost->text());
    m_settings->setProxyPort(ui->proxyPort->value());
    m_settings->setProxyUsername(ui->proxyUsername->text());
    m_settings->setProxyPassword(ui->proxyPassword->text());

    m_settings->setUsePlotForOutline(ui->usePlotForOutline->isChecked());

    // Save Directories
    QList<SettingsDir> movieDirectories;
    QList<SettingsDir> tvShowDirectories;
    QList<SettingsDir> concertDirectories;
    for (int row=0, n=ui->dirs->rowCount() ; row<n ; ++row) {
        SettingsDir dir;
        dir.path = ui->dirs->item(row, 1)->text();
        dir.separateFolders = ui->dirs->item(row, 2)->checkState() == Qt::Checked;
        dir.autoReload = ui->dirs->item(row, 3)->checkState() == Qt::Checked;
        if (static_cast<QComboBox*>(ui->dirs->cellWidget(row, 0))->currentIndex() == 0)
            movieDirectories.append(dir);
        else if (static_cast<QComboBox*>(ui->dirs->cellWidget(row, 0))->currentIndex() == 1)
            tvShowDirectories.append(dir);
        else if (static_cast<QComboBox*>(ui->dirs->cellWidget(row, 0))->currentIndex() == 2)
            concertDirectories.append(dir);
    }
    m_settings->setMovieDirectories(movieDirectories);
    m_settings->setTvShowDirectories(tvShowDirectories);
    m_settings->setConcertDirectories(concertDirectories);

    // exclude words
    m_settings->setExcludeWords(ui->excludeWordsText->toPlainText());

    // Movie set artwork
    m_settings->setMovieSetArtworkType(static_cast<MovieSetArtworkType>(ui->comboMovieSetArtwork->itemData(ui->comboMovieSetArtwork->currentIndex()).toInt()));
    m_settings->setMovieSetArtworkDirectory(ui->movieSetArtworkDir->text());

    // Custom movie scraper
    QMap<int, QString> customMovieScraper;
    for (int row=0, n=ui->customScraperTable->rowCount() ; row<n ; ++row) {
        QComboBox *box = static_cast<QComboBox*>(ui->customScraperTable->cellWidget(row, 1));
        int info = box->itemData(0, Qt::UserRole+1).toInt();
        QString scraper = box->itemData(box->currentIndex()).toString();
        customMovieScraper.insert(info, scraper);
    }
    m_settings->setCustomMovieScraper(customMovieScraper);

    m_settings->saveSettings();

    Manager::instance()->movieFileSearcher()->setMovieDirectories(m_settings->movieDirectories());
    Manager::instance()->tvShowFileSearcher()->setMovieDirectories(m_settings->tvShowDirectories());
    Manager::instance()->concertFileSearcher()->setConcertDirectories(m_settings->concertDirectories());
    MessageBox::instance()->showMessage(tr("Settings saved"));
}

void SettingsWindow::addDir(QString dir, bool separateFolders, bool autoReload, SettingsDirType dirType)
{
    dir = QDir::toNativeSeparators(dir);
    if (!dir.isEmpty()) {
        bool exists = false;
        for (int i=0, n=ui->dirs->rowCount() ; i<n ; ++i) {
            if (ui->dirs->item(i, 1)->text() == dir)
                exists = true;
        }

        if (!exists) {
            int row = ui->dirs->rowCount();
            ui->dirs->insertRow(row);
            QTableWidgetItem *item = new QTableWidgetItem(dir);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
            item->setToolTip(dir);
            QTableWidgetItem *itemCheck = new QTableWidgetItem();
            if (separateFolders)
                itemCheck->setCheckState(Qt::Checked);
            else
                itemCheck->setCheckState(Qt::Unchecked);

            QTableWidgetItem *itemCheckReload = new QTableWidgetItem();
            if (autoReload)
                itemCheckReload->setCheckState(Qt::Checked);
            else
                itemCheckReload->setCheckState(Qt::Unchecked);

            QComboBox *box = new QComboBox();
            box->addItems(QStringList() << tr("Movies") << tr("TV Shows") << tr("Concerts"));
            if (dirType == DirTypeMovies)
                box->setCurrentIndex(0);
            else if (dirType == DirTypeTvShows)
                box->setCurrentIndex(1);
            else if (dirType == DirTypeConcerts)
                box->setCurrentIndex(2);

            ui->dirs->setCellWidget(row, 0, box);
            ui->dirs->setItem(row, 1, item);
            ui->dirs->setItem(row, 2, itemCheck);
            ui->dirs->setItem(row, 3, itemCheckReload);
        }
    }
}

void SettingsWindow::removeDir()
{
    int row = ui->dirs->currentRow();
    if (row < 0)
        return;
    ui->dirs->removeRow(row);
}

void SettingsWindow::organize()
{
    MovieFilesOrganizer* organizer = new MovieFilesOrganizer(this);

    int row = ui->dirs->currentRow();
    if (static_cast<QComboBox*>(ui->dirs->cellWidget(row, 0))->currentIndex() != 0
            || ui->dirs->item(row, 2)->checkState() == Qt::Checked) {
        organizer->canceled(tr("Organizing movies does only work on " \
                                      "movies, not already sorted to " \
                                      "separate folders."));
        return;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("Are you sure?"));
    msgBox.setInformativeText(tr("This operation sorts all movies in this directory to separate " \
                                 "sub-directories based on the file name. Click \"Ok\", if thats, what you want to do. "));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setButtonText(1, tr("Ok"));
    msgBox.setButtonText(2, tr("Cancel"));
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
      case QMessageBox::Ok:
        organizer->moveToDirs(ui->dirs->item(ui->dirs->currentRow(), 1)->text());
        ui->dirs->item(ui->dirs->currentRow(), 2)->setCheckState(Qt::Checked);
        break;
      case QMessageBox::Cancel:
        break;
      default:
        break;
    }
}

void SettingsWindow::dirListRowChanged(int currentRow)
{
    if (currentRow < 0 || currentRow >= ui->dirs->rowCount()) {
        ui->buttonRemoveDir->setDisabled(true);
        ui->buttonMovieFilesToDirs->setDisabled(true);
    } else {
        ui->buttonRemoveDir->setDisabled(false);
        if (ui->dirs->cellWidget(currentRow, 0) != 0
                && static_cast<QComboBox*>(ui->dirs->cellWidget(currentRow, 0))->currentIndex() == 0
                && ui->dirs->item(currentRow, 2)->checkState() == Qt::Unchecked) {
            ui->buttonMovieFilesToDirs->setDisabled(false);
        } else {
            ui->buttonMovieFilesToDirs->setDisabled(true);
        }
    }
}

void SettingsWindow::onUseProxy()
{
    bool enabled = ui->chkUseProxy->isChecked();
    ui->proxyType->setEnabled(enabled);
    ui->proxyHost->setEnabled(enabled);
    ui->proxyPort->setEnabled(enabled);
    ui->proxyUsername->setEnabled(enabled);
    ui->proxyPassword->setEnabled(enabled);
}

void SettingsWindow::chooseDirToAdd()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a directory containing your movies, TV show or concerts"), QDir::homePath());
    if (!dir.isEmpty())
        addDir(dir);
}

void SettingsWindow::onComboMovieSetArtworkChanged()
{
    int value = ui->comboMovieSetArtwork->itemData(ui->comboMovieSetArtwork->currentIndex()).toInt();
    ui->btnMovieSetArtworkDir->setEnabled(value == MovieSetArtworkSingleArtworkFolder);
    ui->movieSetArtworkDir->setEnabled(value == MovieSetArtworkSingleArtworkFolder);

    if (value == MovieSetArtworkSingleArtworkFolder) {
        ui->movieSetPosterFileName->setText("<setName>-folder.jpg");
        ui->movieSetFanartFileName->setText("<setName>-fanart.jpg");
    } else if (value == MovieSetArtworkSingleSetFolder) {
        ui->movieSetPosterFileName->setText("folder.jpg");
        ui->movieSetFanartFileName->setText("fanart.jpg");
    }
}

void SettingsWindow::onChooseMovieSetArtworkDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a directory where your movie set artwork is stored"), QDir::homePath());
    if (!dir.isEmpty())
        ui->movieSetArtworkDir->setText(dir);
}

void SettingsWindow::loadRemoteTemplates()
{
    ExportTemplateLoader::instance()->getRemoteTemplates();
}

void SettingsWindow::onTemplatesLoaded(QList<ExportTemplate*> templates)
{
    ui->exportTemplates->clearContents();
    ui->exportTemplates->setRowCount(0);

    foreach (ExportTemplate *exportTemplate, templates) {
        ExportTemplateWidget *widget = new ExportTemplateWidget(ui->exportTemplates);
        widget->setExportTemplate(exportTemplate);

        int row = ui->exportTemplates->rowCount();
        ui->exportTemplates->insertRow(row);
        ui->exportTemplates->setCellWidget(row, 0, widget);
        widget->adjustSize();
    }
}

void SettingsWindow::onTemplateInstalled(ExportTemplate *exportTemplate, bool success)
{
    if (success)
        ui->themesErrorMessage->setSuccessMessage(tr("Theme \"%1\" was successfully installed").arg(exportTemplate->name()));
    else
        ui->themesErrorMessage->setErrorMessage(tr("There was an error while processing the theme \"%1\"").arg(exportTemplate->name()));
}

void SettingsWindow::onTemplateUninstalled(ExportTemplate *exportTemplate, bool success)
{
    if (success)
        ui->themesErrorMessage->setSuccessMessage(tr("Theme \"%1\" was successfully uninstalled").arg(exportTemplate->name()));
    else
        ui->themesErrorMessage->setErrorMessage(tr("There was an error while processing the theme \"%1\"").arg(exportTemplate->name()));
}

QComboBox *SettingsWindow::comboForMovieScraperInfo(const int &info)
{
    QString currentScraper = m_settings->customMovieScraper().value(info, "notset");

    QComboBox *box = new QComboBox();
    int index = 0;
    if (info != MovieScraperInfos::Title) {
        box->addItem(tr("Don't use"), "");
        box->setItemData(0, info, Qt::UserRole+1);
        index = 1;
    }
    foreach (ScraperInterface *scraper, Manager::instance()->scrapers()) {
        if (scraper->identifier() == "custom-movie")
            continue;
        if (scraper->scraperNativelySupports().contains(info)) {
            box->addItem(scraper->name(), scraper->identifier());
            box->setItemData(index, info, Qt::UserRole+1);
            if (scraper->identifier() == currentScraper || (currentScraper == "notset" && index == 1))
                box->setCurrentIndex(index);
            index++;
        }
    }

    QList<int> images;
    images << MovieScraperInfos::Backdrop << MovieScraperInfos::Logo << MovieScraperInfos::ClearArt
           << MovieScraperInfos::CdArt << MovieScraperInfos::Banner << MovieScraperInfos::Thumb;
    if (images.contains(info)) {
        foreach (ImageProviderInterface *img, Manager::instance()->imageProviders()) {
            if (img->identifier() == "images.fanarttv") {
                box->addItem(img->name(), img->identifier());
                box->setItemData(index, info, Qt::UserRole+1);
                if (img->identifier() == currentScraper || (currentScraper == "notset" && index == 1))
                    box->setCurrentIndex(index);
                index++;
                break;
            }
        }
    }

    return box;
}

QString SettingsWindow::titleForMovieScraperInfo(const int &info)
{
    switch (info) {
    case MovieScraperInfos::Title:
        return tr("Title");
    case MovieScraperInfos::Tagline:
        return tr("Tagline");
    case MovieScraperInfos::Rating:
        return tr("Rating");
    case MovieScraperInfos::Released:
        return tr("Released");
    case MovieScraperInfos::Runtime:
        return tr("Runtime");
    case MovieScraperInfos::Certification:
        return tr("Certification");
    case MovieScraperInfos::Trailer:
        return tr("Trailer");
    case MovieScraperInfos::Overview:
        return tr("Plot");
    case MovieScraperInfos::Poster:
        return tr("Poster");
    case MovieScraperInfos::Backdrop:
        return tr("Fanart");
    case MovieScraperInfos::Actors:
        return tr("Actors");
    case MovieScraperInfos::Genres:
        return tr("Genres");
    case MovieScraperInfos::Studios:
        return tr("Studios");
    case MovieScraperInfos::Countries:
        return tr("Countries");
    case MovieScraperInfos::Writer:
        return tr("Writer");
    case MovieScraperInfos::Director:
        return tr("Director");
    case MovieScraperInfos::Tags:
        return tr("Tags");
    case MovieScraperInfos::Set:
        return tr("Set");
    case MovieScraperInfos::Logo:
        return tr("Logo");
    case MovieScraperInfos::CdArt:
        return tr("Disc Art");
    case MovieScraperInfos::ClearArt:
        return tr("Clear Art");
    case MovieScraperInfos::Banner:
        return tr("Banner");
    case MovieScraperInfos::Thumb:
        return tr("Thumb");
    default:
        return tr("Unsupported");
    }
}
