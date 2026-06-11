#pragma once

#include <QWidget>

#include "../DownloadItem.h"

class QLineEdit;
class QComboBox;
class QPushButton;

class NewDownloadPage final : public QWidget
{
    Q_OBJECT
public:
    explicit NewDownloadPage(QWidget* parent = nullptr);

    void setDefaults(const QString& defaultDir, const QString& outputTemplate);

signals:
    void requestAddDownload(DownloadItem item, bool startNow);

private:
    QLineEdit* m_url = nullptr;
    QComboBox* m_format = nullptr;
    QLineEdit* m_outDir = nullptr;
    QLineEdit* m_outTpl = nullptr;
    QPushButton* m_add = nullptr;
    QPushButton* m_addStart = nullptr;

    void browseDir();
    void emitAdd(bool startNow);
};
