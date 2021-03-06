#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T11:13:07
#
#-------------------------------------------------

QT       += core gui network script xml sql phonon
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): include(qtmacextras/src/qtmacextras.pri)

LIBS += -lmediainfo -lzen -lz -lquazip

unix:LIBS += -lcurl
macx:LIBS += -framework Foundation

DEFINES += UNICODE

TARGET = MediaElch
TEMPLATE = app

target.path = /usr/bin
INSTALLS += target

dotDesktop.path = /usr/share/applications
dotDesktop.files = desktop/MediaElch.desktop
INSTALLS += dotDesktop

icon.path = /usr/share/pixmaps
icon.files = desktop/MediaElch.png
INSTALLS += icon

ICON = MediaElch.icns
RC_FILE = MediaElch.rc
QMAKE_INFO_PLIST = MediaElch.plist

QMAKE_CFLAGS += -gdwarf-2
QMAKE_CXXFLAGS += -gdwarf-2

SOURCES += main.cpp\
        main/MainWindow.cpp \
    movies/Movie.cpp \
    data/MovieFileSearcher.cpp \
    mediaCenterPlugins/XbmcXml.cpp \
    scrapers/TMDb.cpp \
    globals/Manager.cpp \
    movies/MovieSearch.cpp \
    smallWidgets/MyLineEdit.cpp \
    movies/MovieWidget.cpp \
    smallWidgets/MyLabel.cpp \
    globals/DownloadManager.cpp \
    movies/FilesWidget.cpp \
    data/MovieModel.cpp \
    data/MovieProxyModel.cpp \
    main/AboutDialog.cpp \
    scrapers/VideoBuster.cpp \
    scrapers/OFDb.cpp \
    scrapers/Cinefacts.cpp \
    smallWidgets/FilterWidget.cpp \
    smallWidgets/MyTableWidget.cpp \
    main/MessageBox.cpp \
    main/Message.cpp \
    data/TvShow.cpp \
    data/TvShowFileSearcher.cpp \
    data/TvShowModel.cpp \
    tvShows/TvShowFilesWidget.cpp \
    data/TvShowProxyModel.cpp \
    data/TvShowModelItem.cpp \
    data/TvShowEpisode.cpp \
    data/TvShowDelegate.cpp \
    tvShows/TvShowWidget.cpp \
    tvShows/TvShowWidgetEpisode.cpp \
    tvShows/TvShowWidgetTvShow.cpp \
    scrapers/TheTvDb.cpp \
    tvShows/TvShowSearch.cpp \
    globals/DownloadManagerElement.cpp \
    smallWidgets/ImageLabel.cpp \
    globals/ImagePreviewDialog.cpp \
    sets/SetsWidget.cpp \
    sets/MovieListDialog.cpp \
    globals/Helper.cpp \
    smallWidgets/MyTreeView.cpp \
    globals/ImageDialog.cpp \
    settings/DataFile.cpp \
    data/ConcertFileSearcher.cpp \
    data/ConcertModel.cpp \
    data/ConcertProxyModel.cpp \
    data/Concert.cpp \
    concerts/ConcertWidget.cpp \
    concerts/ConcertSearch.cpp \
    concerts/ConcertFilesWidget.cpp \
    scrapers/TMDbConcerts.cpp \
    settings/Settings.cpp \
    movies/GenreWidget.cpp \
    movies/CertificationWidget.cpp \
    support/SupportDialog.cpp \
    main/FileScannerDialog.cpp \
    globals/Filter.cpp \
    data/MovieFilesOrganizer.cpp \
    globals/NameFormatter.cpp \
    imageProviders/FanartTv.cpp \
    imageProviders/FanartTvMusicArtists.cpp \
    imageProviders/TMDbImages.cpp \
    imageProviders/TheTvDbImages.cpp \
    globals/ComboDelegate.cpp \
    data/StreamDetails.cpp \
    smallWidgets/MediaFlags.cpp \
    settings/DataFileListWidget.cpp \
    data/Database.cpp \
    smallWidgets/LoadingStreamDetails.cpp \
    trailerProviders/MovieMaze.cpp \
    globals/TrailerDialog.cpp \
    smallWidgets/SlidingStackedWidget.cpp \
    scrapers/IMDB.cpp \
    xbmc/XbmcSync.cpp \
    smallWidgets/MyCheckBox.cpp \
    movies/MovieController.cpp \
    movies/MovieMultiScrapeDialog.cpp \
    smallWidgets/Badge.cpp \
    trailerProviders/HdTrailers.cpp \
    smallWidgets/TagCloud.cpp \
    smallWidgets/MyWidget.cpp \
    data/Storage.cpp \
    tvShows/TvShowWidgetSeason.cpp \
    smallWidgets/ImageGallery.cpp \
    smallWidgets/ClosableImage.cpp \
    renamer/Renamer.cpp \
    smallWidgets/MySpinBox.cpp \
    settings/AdvancedSettings.cpp \
    smallWidgets/AlphabeticalList.cpp \
    smallWidgets/MyTableView.cpp \
    data/ImageCache.cpp \
    scrapers/TvTunes.cpp \
    tvShows/TvTunesDialog.cpp \
    settings/SettingsWindow.cpp \
    export/ExportTemplateLoader.cpp \
    export/ExportTemplate.cpp \
    settings/ExportTemplateWidget.cpp \
    smallWidgets/StyledPushButton.cpp \
    export/ExportDialog.cpp \
    smallWidgets/MessageLabel.cpp \
    smallWidgets/SearchOverlay.cpp \
    tvShows/ItemWidgetShow.cpp \
    scrapers/CustomMovieScraper.cpp \
    scrapers/MediaPassion.cpp \
    imageProviders/MediaPassionImages.cpp

macx {
    OBJECTIVE_SOURCES += mac/MacFullscreen.mm
}

HEADERS  += main/MainWindow.h \
    movies/Movie.h \
    globals/Globals.h \
    data/MediaCenterInterface.h \
    data/MovieFileSearcher.h \
    mediaCenterPlugins/XbmcXml.h \
    scrapers/TMDb.h \
    data/ScraperInterface.h \
    globals/Manager.h \
    movies/MovieSearch.h \
    smallWidgets/MyLineEdit.h \
    movies/MovieWidget.h \
    smallWidgets/MyLabel.h \
    globals/DownloadManager.h \
    movies/FilesWidget.h \
    data/MovieModel.h \
    data/MovieProxyModel.h \
    main/AboutDialog.h \
    scrapers/VideoBuster.h \
    scrapers/OFDb.h \
    scrapers/Cinefacts.h \
    smallWidgets/FilterWidget.h \
    smallWidgets/MyTableWidget.h \
    main/MessageBox.h \
    main/Message.h \
    data/TvShow.h \
    data/TvShowFileSearcher.h \
    data/TvShowModel.h \
    tvShows/TvShowFilesWidget.h \
    data/TvShowProxyModel.h \
    data/TvShowModelItem.h \
    data/TvShowEpisode.h \
    data/TvShowDelegate.h \
    tvShows/TvShowWidget.h \
    tvShows/TvShowWidgetEpisode.h \
    tvShows/TvShowWidgetTvShow.h \
    scrapers/TheTvDb.h \
    data/TvScraperInterface.h \
    tvShows/TvShowSearch.h \
    globals/DownloadManagerElement.h \
    smallWidgets/ImageLabel.h \
    globals/ImagePreviewDialog.h \
    sets/SetsWidget.h \
    sets/MovieListDialog.h \
    globals/Helper.h \
    smallWidgets/MyTreeView.h \
    globals/ImageDialog.h \
    settings/DataFile.h \
    data/ConcertFileSearcher.h \
    data/ConcertModel.h \
    data/ConcertProxyModel.h \
    data/Concert.h \
    data/ConcertScraperInterface.h \
    concerts/ConcertWidget.h \
    concerts/ConcertSearch.h \
    concerts/ConcertFilesWidget.h \
    scrapers/TMDbConcerts.h \
    settings/Settings.h \
    movies/GenreWidget.h \
    movies/CertificationWidget.h \
    support/SupportDialog.h \
    main/FileScannerDialog.h \
    globals/Filter.h \
    data/MovieFilesOrganizer.h \
    globals/NameFormatter.h \
    data/ImageProviderInterface.h \
    imageProviders/FanartTv.h \
    imageProviders/FanartTvMusicArtists.h \
    imageProviders/TMDbImages.h \
    imageProviders/TheTvDbImages.h \
    globals/ComboDelegate.h \
    data/StreamDetails.h \
    smallWidgets/MediaFlags.h \
    settings/DataFileListWidget.h \
    data/Database.h \
    smallWidgets/LoadingStreamDetails.h \
    trailerProviders/TrailerProvider.h \
    trailerProviders/MovieMaze.h \
    globals/TrailerDialog.h \
    smallWidgets/SlidingStackedWidget.h \
    scrapers/IMDB.h \
    xbmc/XbmcSync.h \
    smallWidgets/MyCheckBox.h \
    movies/MovieController.h \
    movies/MovieMultiScrapeDialog.h \
    smallWidgets/Badge.h \
    trailerProviders/HdTrailers.h \
    smallWidgets/TagCloud.h \
    smallWidgets/MyWidget.h \
    data/Storage.h \
    tvShows/TvShowWidgetSeason.h \
    smallWidgets/ImageGallery.h \
    smallWidgets/ClosableImage.h \
    renamer/Renamer.h \
    smallWidgets/MySpinBox.h \
    settings/AdvancedSettings.h \
    smallWidgets/AlphabeticalList.h \
    smallWidgets/MyTableView.h \
    data/ImageCache.h \
    scrapers/TvTunes.h \
    tvShows/TvTunesDialog.h \
    globals/LocaleStringCompare.h \
    settings/SettingsWindow.h \
    export/ExportTemplateLoader.h \
    export/ExportTemplate.h \
    settings/ExportTemplateWidget.h \
    smallWidgets/StyledPushButton.h \
    export/ExportDialog.h \
    smallWidgets/MessageLabel.h \
    mac/MacFullscreen.h \
    smallWidgets/SearchOverlay.h \
    tvShows/ItemWidgetShow.h \
    scrapers/CustomMovieScraper.h \
    scrapers/MediaPassion.h \
    imageProviders/MediaPassionImages.h

FORMS    += main/MainWindow.ui \
    movies/MovieSearch.ui \
    movies/MovieWidget.ui \
    movies/FilesWidget.ui \
    main/AboutDialog.ui \
    smallWidgets/FilterWidget.ui \
    main/MessageBox.ui \
    main/Message.ui \
    tvShows/TvShowFilesWidget.ui \
    tvShows/TvShowWidget.ui \
    tvShows/TvShowWidgetEpisode.ui \
    tvShows/TvShowWidgetTvShow.ui \
    tvShows/TvShowSearch.ui \
    smallWidgets/ImageLabel.ui \
    globals/ImagePreviewDialog.ui \
    sets/SetsWidget.ui \
    sets/MovieListDialog.ui \
    globals/ImageDialog.ui \
    concerts/ConcertWidget.ui \
    concerts/ConcertSearch.ui \
    concerts/ConcertFilesWidget.ui \
    movies/GenreWidget.ui \
    movies/CertificationWidget.ui \
    support/SupportDialog.ui \
    main/FileScannerDialog.ui \
    smallWidgets/MediaFlags.ui \
    smallWidgets/LoadingStreamDetails.ui \
    globals/TrailerDialog.ui \
    xbmc/XbmcSync.ui \
    movies/MovieMultiScrapeDialog.ui \
    smallWidgets/TagCloud.ui \
    tvShows/TvShowWidgetSeason.ui \
    renamer/Renamer.ui \
    tvShows/TvTunesDialog.ui \
    settings/SettingsWindow.ui \
    settings/ExportTemplateWidget.ui \
    export/ExportDialog.ui \
    tvShows/ItemWidgetShow.ui

RESOURCES += \
    MediaElch.qrc

TRANSLATIONS += \
    i18n/MediaElch_en.ts \
    i18n/MediaElch_de.ts \
    i18n/MediaElch_fr.ts \
    i18n/MediaElch_cs_CZ.ts \
    i18n/MediaElch_pt_BR.ts \
    i18n/MediaElch_no.ts \
    i18n/MediaElch_pl.ts \
    i18n/MediaElch_pt_PT.ts \
    i18n/MediaElch_nl_NL.ts


# qjsonrpc
INCLUDEPATH += $$PWD/qjsonrpc/src
INCLUDEPATH += $$PWD/qjsonrpc/src/json
HEADERS += \
    qjsonrpc/src/qjsonrpcservice_p.h \
    qjsonrpc/src/qjsonrpcmessage_p.h \
    qjsonrpc/src/qjsonrpcservice.h \
    qjsonrpc/src/qjsonrpcmessage.h \
    qjsonrpc/src/qjsonrpc_export.h \
    qjsonrpc/src/json/qjson_p.h \
    qjsonrpc/src/json/qjsonwriter_p.h \
    qjsonrpc/src/json/qjsonparser_p.h \
    qjsonrpc/src/json/qjsondocument.h \
    qjsonrpc/src/json/qjsonobject.h \
    qjsonrpc/src/json/qjsonvalue.h \
    qjsonrpc/src/json/qjsonarray.h
SOURCES += \
    qjsonrpc/src/qjsonrpcservice.cpp \
    qjsonrpc/src/qjsonrpcmessage.cpp \
    qjsonrpc/src/json/qjson.cpp \
    qjsonrpc/src/json/qjsondocument.cpp \
    qjsonrpc/src/json/qjsonobject.cpp \
    qjsonrpc/src/json/qjsonarray.cpp \
    qjsonrpc/src/json/qjsonvalue.cpp \
    qjsonrpc/src/json/qjsonwriter.cpp \
    qjsonrpc/src/json/qjsonparser.cpp
