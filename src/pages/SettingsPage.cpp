#include "SettingsPage.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

SettingsPage::SettingsPage(QWidget* parent)
    : QWidget(parent)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 18);
    root->setSpacing(12);

    auto* title = new QLabel("Settings");
    title->setStyleSheet("font-size: 20px; font-weight: 700;");
    root->addWidget(title);

    auto* box = new QGroupBox("yt-dlp");
    auto* form = new QFormLayout(box);
    form->setLabelAlignment(Qt::AlignRight);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);

    auto* exeRow = new QWidget();
    auto* exeLay = new QHBoxLayout(exeRow);
    exeLay->setContentsMargins(0, 0, 0, 0);
    exeLay->setSpacing(8);
    m_exe = new QLineEdit();
    m_exe->setPlaceholderText("yt-dlp (in PATH) or full path to yt-dlp.exe");
    auto* exeBrowse = new QPushButton("Browse…");
    connect(exeBrowse, &QPushButton::clicked, this, &SettingsPage::browseExe);
    exeLay->addWidget(m_exe, 1);
    exeLay->addWidget(exeBrowse);
    form->addRow("Executable", exeRow);

    root->addWidget(box);

    auto* out = new QGroupBox("Defaults");
    auto* outForm = new QFormLayout(out);
    outForm->setLabelAlignment(Qt::AlignRight);
    outForm->setHorizontalSpacing(12);
    outForm->setVerticalSpacing(10);

    auto* dirRow = new QWidget();
    auto* dirLay = new QHBoxLayout(dirRow);
    dirLay->setContentsMargins(0, 0, 0, 0);
    dirLay->setSpacing(8);
    m_dir = new QLineEdit();
    m_dir->setPlaceholderText("Default output folder");
    auto* dirBrowse = new QPushButton("Browse…");
    connect(dirBrowse, &QPushButton::clicked, this, &SettingsPage::browseDir);
    dirLay->addWidget(m_dir, 1);
    dirLay->addWidget(dirBrowse);
    outForm->addRow("Folder", dirRow);

    m_tpl = new QLineEdit();
    m_tpl->setPlaceholderText("%(title).200B [%(id)s].%(ext)s");
    outForm->addRow("Template", m_tpl);

    root->addWidget(out);
    root->addStretch(1);

    auto* bottom = new QHBoxLayout();
    bottom->addStretch(1);
    m_save = new QPushButton("Save");
    m_save->setProperty("primary", true);
    bottom->addWidget(m_save);
    root->addLayout(bottom);

    connect(m_save, &QPushButton::clicked, this, [this]() {
        emit settingsChanged(m_exe->text().trimmed(), m_dir->text().trimmed(), m_tpl->text().trimmed());
    });

    const QString dl = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (!dl.trimmed().isEmpty()) m_dir->setText(dl);
    m_tpl->setText("%(title).200B [%(id)s].%(ext)s");
}

void SettingsPage::setValues(const QString& ytDlpPath, const QString& defaultDir, const QString& outputTemplate)
{
    if (!ytDlpPath.trimmed().isEmpty()) m_exe->setText(ytDlpPath);
    if (!defaultDir.trimmed().isEmpty()) m_dir->setText(defaultDir);
    if (!outputTemplate.trimmed().isEmpty()) m_tpl->setText(outputTemplate);
}

void SettingsPage::browseExe()
{
    const QString exe = QFileDialog::getOpenFileName(this, "Select yt-dlp executable", QString(), "Executable (*.exe);;All files (*.*)");
    if (!exe.isEmpty()) m_exe->setText(exe);
}

void SettingsPage::browseDir()
{
    const QString start = m_dir->text().trimmed().isEmpty()
        ? QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
        : m_dir->text().trimmed();
    const QString dir = QFileDialog::getExistingDirectory(this, "Select default output folder", start);
    if (!dir.isEmpty()) m_dir->setText(dir);
}
