#include "MainWindow.h"

#include <QAbstractButton>
#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QStatusBar>
#include <QStackedWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "DarkNotifyDialog.h"
#include "TitleBar.h"
#include "pages/AboutPage.h"
#include "pages/NewDownloadPage.h"
#include "pages/QueuePage.h"
#include "pages/SettingsPage.h"

#include <QDebug>

static QString prettifyTitleFromUrl(const QString& url)
{
    QString t = url;
    if (t.size() > 60) t = t.left(57) + "...";
    return t;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, false);

    m_model = new DownloadsModel(this);
    m_yt = new YtDlpClient(this);

    loadSettings();
    buildUi();
    wireSignals();
    applySettingsToPages();

}

void MainWindow::buildUi()
{
    setWindowTitle("Downloader");

    auto* central = new QWidget();
    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto* titleBar = new TitleBar(this);
    titleBar->setTitle("");
    mainLayout->addWidget(titleBar);

    auto* root = new QHBoxLayout();
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto* nav = new QWidget();
    nav->setFixedWidth(220);
    nav->setStyleSheet("background: #0F0F12; border-right: 1px solid #2A2A33;");
    auto* navLay = new QVBoxLayout(nav);
    navLay->setContentsMargins(12, 12, 12, 12);
    navLay->setSpacing(8);

    auto* appTitle = new QLabel("Downloader");
    appTitle->setStyleSheet("font-size: 16px; font-weight: 800; color: #EDEDF6;");
    navLay->addWidget(appTitle);

    // auto* sub = new QLabel("yt-dlp UI");
    // sub->setStyleSheet("color: #A8A8B5;");
    // navLay->addWidget(sub);
    // navLay->addSpacing(8);

    m_navGroup = new QButtonGroup(this);
    m_navGroup->setExclusive(true);

    auto mkNav = [&](const QString& text) {
        auto* b = new QToolButton();
        b->setText(text);
        b->setCheckable(true);
        b->setToolButtonStyle(Qt::ToolButtonTextOnly);
        b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        b->setMinimumHeight(36);
        b->setStyleSheet("QToolButton { text-align: left; padding-left: 12px; }");
        m_navGroup->addButton(b);
        navLay->addWidget(b);
        return b;
    };

    auto* navNew = mkNav("New download");
    auto* navQueue = mkNav("Queue");
    auto* navSettings = mkNav("Settings");
    auto* navAbout = mkNav("About");

    navLay->addStretch(1);

    auto* footer = new QLabel("Local UI template");
    footer->setStyleSheet("color: #6E6E7A;");
    navLay->addWidget(footer);

    m_stack = new QStackedWidget();
    m_newPage = new NewDownloadPage();
    m_queuePage = new QueuePage();
    m_settingsPage = new SettingsPage();
    m_aboutPage = new AboutPage();

    m_queuePage->setModel(m_model);

    m_stack->addWidget(m_newPage);
    m_stack->addWidget(m_queuePage);
    m_stack->addWidget(m_settingsPage);
    m_stack->addWidget(m_aboutPage);

    root->addWidget(nav);
    root->addWidget(m_stack, 1);
    mainLayout->addLayout(root, 1);

    setCentralWidget(central);

    navNew->setChecked(true);
    m_stack->setCurrentWidget(m_newPage);

    connect(m_navGroup, &QButtonGroup::buttonClicked, this, [=](QAbstractButton* b) {
        if (b == navNew) m_stack->setCurrentWidget(m_newPage);
        else if (b == navQueue) m_stack->setCurrentWidget(m_queuePage);
        else if (b == navSettings) m_stack->setCurrentWidget(m_settingsPage);
        else if (b == navAbout) m_stack->setCurrentWidget(m_aboutPage);
    });

    statusBar()->showMessage("Ready");
}

void MainWindow::wireSignals()
{
    connect(m_newPage, &NewDownloadPage::requestAddDownload, this, [this](DownloadItem item, bool startNow) {
        if (item.title.isEmpty()) item.title = prettifyTitleFromUrl(item.url);
        m_model->addItem(item);
        statusBar()->showMessage("Added to queue", 1500);
        if (startNow) startOrResume(item.id);
        m_stack->setCurrentWidget(m_queuePage);
    });

    connect(m_queuePage, &QueuePage::requestStart, this, &MainWindow::startOrResume);
    connect(m_queuePage, &QueuePage::requestPause, this, &MainWindow::pause);
    connect(m_queuePage, &QueuePage::requestCancel, this, &MainWindow::cancel);
    connect(m_queuePage, &QueuePage::requestRemove, this, &MainWindow::remove);
    connect(m_queuePage, &QueuePage::selectionChanged, this, &MainWindow::refreshSelectedSummary);

    connect(m_settingsPage, &SettingsPage::settingsChanged, this, [this](const QString& exe, const QString& dir, const QString& tpl) {
        m_ytDlpPath = exe;
        m_defaultDir = dir;
        m_outputTemplate = tpl;
        saveSettings();
        applySettingsToPages();
        statusBar()->showMessage("Settings saved", 1500);
    });

    connect(m_yt, &YtDlpClient::downloadStarted, this, [this](const QString& id) {
        m_model->updateItem(id, [](DownloadItem& it) {
            it.status = DownloadStatus::Downloading;
            it.statusText = "Downloading";
            if (it.progressPercent == 0) it.progressPercent = 1;
        });
        refreshSelectedSummary();
    });

    connect(m_yt, &YtDlpClient::downloadProgress, this, [this](const QString& id, int percent, const QString& speed, const QString& eta, const QString& statusText) {
        m_model->updateItem(id, [&](DownloadItem& it) {
            it.status = DownloadStatus::Downloading;
            it.progressPercent = percent;
            it.speedText = speed;
            it.etaText = eta;
            it.statusText = statusText;
        });
        refreshSelectedSummary();
    });

    connect(m_yt, &YtDlpClient::downloadFinished, this, [this](const QString& id, bool ok, const QString& errorText) {
        const bool wasCancel = m_cancelRequested.contains(id);
        const bool wasPause = m_pauseRequested.contains(id);
        m_cancelRequested.remove(id);
        m_pauseRequested.remove(id);

        m_model->updateItem(id, [&](DownloadItem& it) {
            if (wasCancel) {
                it.status = DownloadStatus::Cancelled;
                it.statusText = "Cancelled";
                return;
            }
            if (wasPause) {
                it.status = DownloadStatus::Paused;
                it.statusText = "Paused";
                return;
            }
            if (ok) {
                it.status = DownloadStatus::Completed;
                it.statusText = "Completed";
                it.progressPercent = 100;
                it.speedText.clear();
                it.etaText.clear();
            } else {
                it.status = DownloadStatus::Failed;
                it.statusText = "Failed";
                it.lastError = errorText;
            }
        });

        refreshSelectedSummary();
        statusBar()->showMessage(ok ? "Download finished" : "Download finished with error", 3000);

        if (!ok && !wasCancel && !wasPause && !errorText.isEmpty()) {
            DarkNotifyDialog::showError(this, "Download Failed", errorText);
        }
    });
}

void MainWindow::startOrResume(const QString& id)
{
    const int row = m_model->indexOfId(id);
    auto* it = m_model->itemAt(row);
    if (!it) return;

    DownloadItem snapshot = *it;
    if (snapshot.outputDir.trimmed().isEmpty()) snapshot.outputDir = m_defaultDir;
    if (snapshot.outputTemplate.trimmed().isEmpty()) snapshot.outputTemplate = m_outputTemplate;

    if (!m_ytDlpPath.trimmed().isEmpty()) m_yt->setExecutablePath(m_ytDlpPath);

    m_model->updateItem(id, [](DownloadItem& it2) {
        it2.status = DownloadStatus::Downloading;
        it2.statusText = "Starting…";
    });

    if (it->status == DownloadStatus::Paused) {
        m_yt->resume(snapshot);
    } else {
        m_yt->startDownload(snapshot);
    }
}

void MainWindow::pause(const QString& id)
{
    if (!m_model->itemAt(m_model->indexOfId(id))) return;
    m_pauseRequested.insert(id);
    m_model->updateItem(id, [](DownloadItem& it) {
        it.statusText = "Pausing…";
    });
    m_yt->pause(id);
}

void MainWindow::cancel(const QString& id)
{
    if (!m_model->itemAt(m_model->indexOfId(id))) return;
    m_cancelRequested.insert(id);
    m_model->updateItem(id, [](DownloadItem& it) {
        it.statusText = "Cancelling…";
    });
    m_yt->cancel(id);
}

void MainWindow::remove(const QString& id)
{
    if (m_yt->isRunning(id)) {
        DarkNotifyDialog::showInfo(this, "Remove", "Please cancel the running download before removing it.");
        return;
    }
    m_model->removeById(id);
    refreshSelectedSummary();
}

void MainWindow::refreshSelectedSummary()
{
    const QString id = m_queuePage->selectedId();
    if (id.isEmpty()) {
        m_queuePage->setSelectedSummary(0, "-", "-", "-");
        return;
    }
    const int row = m_model->indexOfId(id);
    const auto* it = m_model->itemAt(row);
    if (!it) {
        m_queuePage->setSelectedSummary(0, "-", "-", "-");
        return;
    }
    m_queuePage->setSelectedSummary(it->progressPercent, it->statusText, it->speedText, it->etaText);
}

void MainWindow::loadSettings()
{
    QSettings s;
    m_ytDlpPath = s.value("ytDlp/path", "").toString();
    m_defaultDir = s.value("output/defaultDir", "").toString();
    m_outputTemplate = s.value("output/template", "%(title).200B [%(id)s].%(ext)s").toString();
}

void MainWindow::saveSettings()
{
    QSettings s;
    s.setValue("ytDlp/path", m_ytDlpPath);
    s.setValue("output/defaultDir", m_defaultDir);
    s.setValue("output/template", m_outputTemplate);
}

void MainWindow::applySettingsToPages()
{
    m_settingsPage->setValues(m_ytDlpPath, m_defaultDir, m_outputTemplate);
    m_newPage->setDefaults(m_defaultDir, m_outputTemplate);
    if (!m_ytDlpPath.trimmed().isEmpty()) m_yt->setExecutablePath(m_ytDlpPath);
}
