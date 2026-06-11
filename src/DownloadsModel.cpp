#include "DownloadsModel.h"

#include <QBrush>

DownloadsModel::DownloadsModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int DownloadsModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return m_items.size();
}

int DownloadsModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) return 0;
    return ColCount;
}

QVariant DownloadsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};
    const auto& it = m_items[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case ColTitle: return it.title.isEmpty() ? "(untitled)" : it.title;
        case ColProgress: return QString("%1%").arg(it.progressPercent);
        case ColStatus: return it.statusText.isEmpty() ? toDisplayString(it.status) : it.statusText;
        case ColSpeed: return it.speedText;
        case ColEta: return it.etaText;
        case ColUrl: return it.url;
        default: return {};
        }
    }

    if (role == Qt::TextAlignmentRole) {
        if (index.column() == ColProgress) return Qt::AlignCenter;
        if (index.column() == ColStatus) return Qt::AlignCenter;
        if (index.column() == ColEta) return Qt::AlignCenter;
        return {};
    }

    if (role == Qt::ForegroundRole) {
        if (it.status == DownloadStatus::Failed) return QBrush(QColor(255, 110, 110));
        if (it.status == DownloadStatus::Completed) return QBrush(QColor(140, 220, 160));
        if (it.status == DownloadStatus::Paused) return QBrush(QColor(220, 200, 120));
        return {};
    }

    return {};
}

QVariant DownloadsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
    case ColTitle: return "Title";
    case ColProgress: return "Progress";
    case ColStatus: return "Status";
    case ColSpeed: return "Speed";
    case ColEta: return "ETA";
    case ColUrl: return "URL";
    default: return {};
    }
}

Qt::ItemFlags DownloadsModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

const DownloadItem* DownloadsModel::itemAt(int row) const
{
    if (row < 0 || row >= m_items.size()) return nullptr;
    return &m_items[row];
}

DownloadItem* DownloadsModel::itemAt(int row)
{
    if (row < 0 || row >= m_items.size()) return nullptr;
    return &m_items[row];
}

int DownloadsModel::indexOfId(const QString& id) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].id == id) return i;
    }
    return -1;
}

void DownloadsModel::addItem(DownloadItem item)
{
    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back(std::move(item));
    endInsertRows();
}

void DownloadsModel::updateItem(const QString& id, const std::function<void(DownloadItem&)>& mutator)
{
    const int row = indexOfId(id);
    if (row < 0) return;
    mutator(m_items[row]);
    emit dataChanged(index(row, 0), index(row, ColCount - 1));
}

void DownloadsModel::removeById(const QString& id)
{
    const int row = indexOfId(id);
    if (row < 0) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

void DownloadsModel::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}
