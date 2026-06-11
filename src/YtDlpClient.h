#pragma once

#include <QObject>
#include <QHash>
#include <QProcess>
#include <QString>

#include "DownloadItem.h"

class YtDlpClient final : public QObject
{
    Q_OBJECT
public:
    explicit YtDlpClient(QObject* parent = nullptr);

    void setExecutablePath(QString path);
    QString executablePath() const;

    bool isRunning(const QString& id) const;

    void startDownload(const DownloadItem& item);
    void cancel(const QString& id);
    void pause(const QString& id);
    void resume(const DownloadItem& item);

signals:
    void downloadStarted(const QString& id);
    void downloadLog(const QString& id, const QString& line);
    void downloadProgress(const QString& id, int percent, const QString& speed, const QString& eta, const QString& statusText);
    void downloadFinished(const QString& id, bool ok, const QString& errorText);

private:
    QString m_exe = "yt-dlp";
    QHash<QString, QProcess*> m_procById;
    QHash<QString, QString> m_stderrBuf;

    QStringList buildArgs(const DownloadItem& item, bool isResume) const;
    void attachProcess(const QString& id, QProcess* p);
    void handleOutputLine(const QString& id, const QString& line);
};
