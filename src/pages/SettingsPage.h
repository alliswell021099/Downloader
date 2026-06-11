#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;

class SettingsPage final : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget* parent = nullptr);

    void setValues(const QString& ytDlpPath, const QString& defaultDir, const QString& outputTemplate);

signals:
    void settingsChanged(const QString& ytDlpPath, const QString& defaultDir, const QString& outputTemplate);

private:
    QLineEdit* m_exe = nullptr;
    QLineEdit* m_dir = nullptr;
    QLineEdit* m_tpl = nullptr;
    QPushButton* m_save = nullptr;

    void browseExe();
    void browseDir();
};
