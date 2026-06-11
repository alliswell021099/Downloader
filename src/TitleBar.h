#pragma once

#include <QPoint>
#include <QWidget>

class QLabel;
class QMouseEvent;
class TitleButton;

class TitleBar final : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget* parent = nullptr);

    void setTitle(const QString& title);

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private:
    QLabel* m_title = nullptr;
    TitleButton* m_minBtn = nullptr;
    TitleButton* m_maxBtn = nullptr;
    TitleButton* m_closeBtn = nullptr;
    QPoint m_dragStart;
    bool m_dragging = false;
};
