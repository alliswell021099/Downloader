#include "TitleBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>

#include "TitleButton.h"

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(36);
    setStyleSheet("TitleBar { background: #0F0F12; border-bottom: 1px solid #2A2A33; }");

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 0, 0);
    layout->setSpacing(0);

    m_title = new QLabel("Downloader");
    m_title->setStyleSheet("color: #CFCFDE; font-size: 13px;");
    layout->addWidget(m_title);

    layout->addStretch(1);

    m_minBtn = new TitleButton(TitleButton::Minimize);
    connect(m_minBtn, &TitleButton::clicked, this, [this]() {
        if (QWidget* w = window()) w->showMinimized();
    });
    layout->addWidget(m_minBtn);

    m_maxBtn = new TitleButton(TitleButton::Maximize);
    connect(m_maxBtn, &TitleButton::clicked, this, [this]() {
        QWidget* w = window();
        if (w->isMaximized()) {
            w->showNormal();
            m_maxBtn->setGlyph(TitleButton::Maximize);
        } else {
            w->showMaximized();
            m_maxBtn->setGlyph(TitleButton::Restore);
        }
    });
    layout->addWidget(m_maxBtn);

    m_closeBtn = new TitleButton(TitleButton::Close);
    connect(m_closeBtn, &TitleButton::clicked, this, [this]() {
        if (QWidget* w = window()) w->close();
    });
    layout->addWidget(m_closeBtn);
}

void TitleBar::setTitle(const QString& title)
{
    m_title->setText(title);
}

void TitleBar::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton && !window()->isMaximized()) {
        m_dragStart = e->globalPosition().toPoint() - window()->frameGeometry().topLeft();
        m_dragging = true;
    }
    QWidget::mousePressEvent(e);
}

void TitleBar::mouseMoveEvent(QMouseEvent* e)
{
    if (m_dragging && (e->buttons() & Qt::LeftButton)) {
        window()->move(e->globalPosition().toPoint() - m_dragStart);
    }
    QWidget::mouseMoveEvent(e);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) m_dragging = false;
    QWidget::mouseReleaseEvent(e);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        QWidget* w = window();
        if (w->isMaximized()) {
            w->showNormal();
            m_maxBtn->setGlyph(TitleButton::Maximize);
        } else {
            w->showMaximized();
            m_maxBtn->setGlyph(TitleButton::Restore);
        }
    }
    QWidget::mouseDoubleClickEvent(e);
}
