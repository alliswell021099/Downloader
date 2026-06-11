#pragma once

#include <QAbstractTableModel>
#include <QVector>
#include <functional>

#include "DownloadItem.h"

class DownloadsModel final : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column {
        ColTitle = 0,
        ColProgress,
        ColStatus,
        ColSpeed,
        ColEta,
        ColUrl,
        ColCount
    };

    explicit DownloadsModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    const DownloadItem* itemAt(int row) const;
    DownloadItem* itemAt(int row);
    int indexOfId(const QString& id) const;

    void addItem(DownloadItem item);
    void updateItem(const QString& id, const std::function<void(DownloadItem&)>& mutator);
    void removeById(const QString& id);
    void clear();

private:
    QVector<DownloadItem> m_items;
};
