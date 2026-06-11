#pragma once

#include <QMainWindow>
#include <QSet>

#include "DownloadsModel.h"
#include "YtDlpClient.h"

class QStackedWidget;
class QToolButton;
class QButtonGroup;

class NewDownloadPage;
class QueuePage;
class SettingsPage;
class AboutPage;

class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    DownloadsModel* m_model = nullptr;
    YtDlpClient* m_yt = nullptr;

    QStackedWidget* m_stack = nullptr;
    QButtonGroup* m_navGroup = nullptr;

    NewDownloadPage* m_newPage = nullptr;
    QueuePage* m_queuePage = nullptr;
    SettingsPage* m_settingsPage = nullptr;
    AboutPage* m_aboutPage = nullptr;

    QString m_defaultDir;
    QString m_outputTemplate;
    QString m_ytDlpPath;
    QSet<QString> m_cancelRequested;
    QSet<QString> m_pauseRequested;

    void buildUi();
    void loadSettings();
    void saveSettings();
    void applySettingsToPages();

    void wireSignals();
    void startOrResume(const QString& id);
    void pause(const QString& id);
    void cancel(const QString& id);
    void remove(const QString& id);
    void refreshSelectedSummary();
};
