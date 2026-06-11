#pragma once

#include <QString>
#include <QDateTime>

enum class DownloadStatus {
    Queued,
    FetchingInfo,
    Downloading,
    Paused,
    Completed,
    Failed,
    Cancelled
};

struct DownloadItem {
    QString id;
    QString url;
    QString title;
    QString formatId;
    QString ext;
    QString outputDir;
    QString outputTemplate;

    DownloadStatus status = DownloadStatus::Queued;
    int progressPercent = 0;
    QString speedText;
    QString etaText;
    QString statusText;
    QString lastError;

    QDateTime createdAt = QDateTime::currentDateTime();
};

QString toDisplayString(DownloadStatus s);
