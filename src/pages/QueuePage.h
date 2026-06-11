#pragma once

#include <QWidget>

class QTableView;
class QPushButton;
class QProgressBar;
class QLabel;

class DownloadsModel;

class QueuePage final : public QWidget
{
    Q_OBJECT
public:
    explicit QueuePage(QWidget* parent = nullptr);

    void setModel(DownloadsModel* model);
    QString selectedId() const;

signals:
    void selectionChanged();
    void requestStart(const QString& id);
    void requestPause(const QString& id);
    void requestCancel(const QString& id);
    void requestRemove(const QString& id);

public slots:
    void setSelectedSummary(int percent, const QString& status, const QString& speed, const QString& eta);

private:
    DownloadsModel* m_model = nullptr;
    QTableView* m_table = nullptr;
    QProgressBar* m_progress = nullptr;
    QLabel* m_status = nullptr;
    QLabel* m_speed = nullptr;
    QLabel* m_eta = nullptr;
    QPushButton* m_start = nullptr;
    QPushButton* m_pause = nullptr;
    QPushButton* m_cancel = nullptr;
    QPushButton* m_remove = nullptr;

    void wireButtons();
};
