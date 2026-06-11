#include "DownloadItem.h"

QString toDisplayString(DownloadStatus s)
{
    switch (s) {
    case DownloadStatus::Queued: return "Queued";
    case DownloadStatus::FetchingInfo: return "Fetching info";
    case DownloadStatus::Downloading: return "Downloading";
    case DownloadStatus::Paused: return "Paused";
    case DownloadStatus::Completed: return "Completed";
    case DownloadStatus::Failed: return "Failed";
    case DownloadStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}
