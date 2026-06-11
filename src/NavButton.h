#pragma once

#include <QAbstractButton>
#include <QColor>
#include <Qt>

class NavButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit NavButton(const QString& text, QWidget* parent = nullptr);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    Qt::Alignment textAlignment() const { return m_textAlign; }
    void setTextAlignment(Qt::Alignment align);

    QColor normalTextColor() const { return m_colors.normalText; }
    void setNormalTextColor(const QColor& color);

    QColor hoverTextColor() const { return m_colors.hoverText; }
    void setHoverTextColor(const QColor& color);

    QColor checkedTextColor() const { return m_colors.checkedText; }
    void setCheckedTextColor(const QColor& color);

    QColor checkedBgColor() const { return m_colors.checkedBg; }
    void setCheckedBgColor(const QColor& color);

    QColor hoverBgColor() const { return m_colors.hoverBg; }
    void setHoverBgColor(const QColor& color);

    QColor indicatorColor() const { return m_colors.indicator; }
    void setIndicatorColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent* e) override;
    void enterEvent(QEnterEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    Qt::Alignment m_textAlign;

    struct Colors {
        QColor normalText = QColor(168, 168, 181);
        QColor hoverText = QColor(235, 235, 235);
        QColor checkedText = QColor(255, 255, 255);
        QColor hoverBg = QColor(24, 24, 32);
        QColor checkedBg = QColor(46, 91, 255);
        QColor indicator = QColor(46, 91, 255);
    };
    Colors m_colors;
};
