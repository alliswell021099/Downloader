#include "NewDownloadPage.h"

#include <QUuid>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

NewDownloadPage::NewDownloadPage(QWidget* parent)
    : QWidget(parent)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 18);
    root->setSpacing(12);

    auto* title = new QLabel("Download");
    title->setStyleSheet("font-size: 20px; font-weight: 700;");
    root->addWidget(title);

    auto* box = new QGroupBox("Source");
    auto* form = new QFormLayout(box);
    form->setLabelAlignment(Qt::AlignRight);
    form->setFormAlignment(Qt::AlignTop);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(10);

    m_url = new QLineEdit();
    m_url->setPlaceholderText("Paste video URL…");
    form->addRow("URL", m_url);

    m_format = new QComboBox();
    m_format->addItem("Auto（不指定格式，推荐 B 站）", "");
    m_format->addItem("Best", "best");
    m_format->addItem("Best video+audio (需 ffmpeg)", "bestvideo+bestaudio/best");
    m_format->addItem("Best video only", "bestvideo/best");
    m_format->addItem("Best audio only", "bestaudio/best");
    m_format->addItem("1080p 及以下", "bv*[height<=1080]+ba/bv*+ba/best");
    m_format->addItem("720p 及以下", "bv*[height<=720]+ba/bv*+ba/best");
    form->addRow("Format", m_format);

    root->addWidget(box);

    auto* outBox = new QGroupBox("Output");
    auto* outForm = new QFormLayout(outBox);
    outForm->setLabelAlignment(Qt::AlignRight);
    outForm->setHorizontalSpacing(12);
    outForm->setVerticalSpacing(10);

    auto* outDirRow = new QWidget();
    auto* outDirLay = new QHBoxLayout(outDirRow);
    outDirLay->setContentsMargins(0, 0, 0, 0);
    outDirLay->setSpacing(8);

    m_outDir = new QLineEdit();
    m_outDir->setPlaceholderText("Choose output folder…");
    auto* browse = new QPushButton("Browse…");
    connect(browse, &QPushButton::clicked, this, &NewDownloadPage::browseDir);
    outDirLay->addWidget(m_outDir, 1);
    outDirLay->addWidget(browse);

    outForm->addRow("Folder", outDirRow);

    m_outTpl = new QLineEdit();
    m_outTpl->setPlaceholderText("%(title).200B [%(id)s].%(ext)s");
    outForm->addRow("Template", m_outTpl);

    auto* hint = new QLabel("Tip: template uses yt-dlp fields like %(title)s, %(id)s, %(ext)s.");
    hint->setStyleSheet("color: #A8A8B5;");
    outForm->addRow("", hint);

    root->addWidget(outBox);
    root->addStretch(1);

    auto* actions = new QHBoxLayout();
    actions->addStretch(1);
    m_add = new QPushButton("Add to queue");
    m_addStart = new QPushButton("Add & start");
    m_addStart->setProperty("primary", true);
    actions->addWidget(m_add);
    actions->addWidget(m_addStart);
    root->addLayout(actions);

    connect(m_add, &QPushButton::clicked, this, [this]() { emitAdd(false); });
    connect(m_addStart, &QPushButton::clicked, this, [this]() { emitAdd(true); });

    const QString dl = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (!dl.trimmed().isEmpty()) m_outDir->setText(dl);
    m_outTpl->setText("%(title).200B [%(id)s].%(ext)s");
}

void NewDownloadPage::setDefaults(const QString& defaultDir, const QString& outputTemplate)
{
    if (!defaultDir.trimmed().isEmpty()) m_outDir->setText(defaultDir);
    if (!outputTemplate.trimmed().isEmpty()) m_outTpl->setText(outputTemplate);
}

void NewDownloadPage::browseDir()
{
    const QString start = m_outDir->text().trimmed().isEmpty()
        ? QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
        : m_outDir->text().trimmed();

    const QString dir = QFileDialog::getExistingDirectory(this, "Select output folder", start);
    if (!dir.isEmpty()) m_outDir->setText(dir);
}

void NewDownloadPage::emitAdd(bool startNow)
{
    const QString url = m_url->text().trimmed();
    if (url.isEmpty()) {
        m_url->setFocus();
        return;
    }

    DownloadItem item;
    item.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    item.url = url;
    item.formatId = m_format->currentData().toString();
    item.outputDir = m_outDir->text().trimmed();
    item.outputTemplate = m_outTpl->text().trimmed();
    item.status = DownloadStatus::Queued;
    item.statusText = "Queued";

    emit requestAddDownload(std::move(item), startNow);
}
