#include "AboutPage.h"

#include <QLabel>
#include <QVBoxLayout>

AboutPage::AboutPage(QWidget* parent)
    : QWidget(parent)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 18);
    root->setSpacing(12);

    auto* title = new QLabel("About");
    title->setStyleSheet("font-size: 20px; font-weight: 700;");
    root->addWidget(title);

    auto* body = new QLabel(
        "QtVideoDownloader UI demo\n\n"
        "This project provides a clean Qt6 Widgets interface for managing video downloads via yt-dlp.\n"
        "It includes a queue view, basic controls, and settings for yt-dlp path and output defaults.\n\n"
        "Note: Downloading from websites may be restricted by their Terms of Service and local laws."
    );
    body->setWordWrap(true);
    body->setStyleSheet("color: #CFCFDE; line-height: 1.3;");
    root->addWidget(body);
    root->addStretch(1);
}
