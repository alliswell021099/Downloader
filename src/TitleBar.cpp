#include "TitleBar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(36);
    setStyleSheet("TitleBar { background: #0F0F12; border-bottom: 1px solid #2A2A33; }");

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 4, 0);
    layout->setSpacing(0);

    m_title = new QLabel("QtVideoDownloader");
    m_title->setStyleSheet("color: #CFCFDE; font-size: 13px;");
    layout->addWidget(m_title);

    layout->addStretch(1);

    auto btnStyle = QStringLiteral(R"(
        QPushButton {
            background: transparent;
            border: none;
            color: #A8A8B5;
            min-width: 46px;
            min-height: 36px;
        }
        QPushButton:hover {
            background: #1B1B24;
            color: #EBEBEB;
        }
        QPushButton#closeBtn:hover {
            background: #C42B1C;
            color: white;
        }
    )");

    m_minBtn = new QPushButton("—");
    m_minBtn->setStyleSheet(btnStyle);
    m_minBtn->setCursor(Qt::PointingHandCursor);
    connect(m_minBtn, &QPushButton::clicked, this, [this]() {
        if (QWidget* w = window()) w->showMinimized();
    });
    layout->addWidget(m_minBtn);

    m_maxBtn = new QPushButton("□");
    m_maxBtn->setStyleSheet(btnStyle);
    m_maxBtn->setCursor(Qt::PointingHandCursor);
    connect(m_maxBtn, &QPushButton::clicked, this, [this]() {
        QWidget* w = window();
        if (w->isMaximized()) {
            w->showNormal();
            m_maxBtn->setText("□");
        } else {
            w->showMaximized();
            m_maxBtn->setText("❐");
        }
    });
    layout->addWidget(m_maxBtn);

    m_closeBtn = new QPushButton("×");
    m_closeBtn->setObjectName("closeBtn");
    m_closeBtn->setStyleSheet(btnStyle);
    m_closeBtn->setCursor(Qt::PointingHandCursor);
    connect(m_closeBtn, &QPushButton::clicked, this, [this]() {
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
            m_maxBtn->setText("□");
        } else {
            w->showMaximized();
            m_maxBtn->setText("❐");
        }
    }
    QWidget::mouseDoubleClickEvent(e);
}
