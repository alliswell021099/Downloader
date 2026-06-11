#include "NavButton.h"

#include <QEnterEvent>
#include <QPainter>

namespace {
constexpr int kMinHeight = 36;
constexpr int kRadius = 5;
constexpr int kIndicatorWidth = 3;
} // namespace

NavButton::NavButton(const QString& text, QWidget* parent)
    : QAbstractButton(parent), m_textAlign(Qt::AlignCenter)
{
    setText(text);
    setCheckable(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(kMinHeight);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
}

QSize NavButton::sizeHint() const
{
    return QSize(150, kMinHeight);
}

QSize NavButton::minimumSizeHint() const
{
    return QSize(100, kMinHeight);
}

void NavButton::setTextAlignment(Qt::Alignment align)
{
    if (m_textAlign == align) return;
    m_textAlign = align;
    update();
}

void NavButton::setNormalTextColor(const QColor& color)
{
    if (m_colors.normalText == color) return;
    m_colors.normalText = color;
    update();
}

void NavButton::setHoverTextColor(const QColor& color)
{
    if (m_colors.hoverText == color) return;
    m_colors.hoverText = color;
    update();
}

void NavButton::setCheckedTextColor(const QColor& color)
{
    if (m_colors.checkedText == color) return;
    m_colors.checkedText = color;
    update();
}

void NavButton::setCheckedBgColor(const QColor& color)
{
    if (m_colors.checkedBg == color) return;
    m_colors.checkedBg = color;
    update();
}

void NavButton::setHoverBgColor(const QColor& color)
{
    if (m_colors.hoverBg == color) return;
    m_colors.hoverBg = color;
    update();
}

void NavButton::setIndicatorColor(const QColor& color)
{
    if (m_colors.indicator == color) return;
    m_colors.indicator = color;
    update();
}

void NavButton::paintEvent(QPaintEvent* /*e*/)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const bool hover = underMouse();
    const bool checked = isChecked();
    const bool down = isDown();

    QColor bg;
    QColor indicator;
    QColor textColor;

    if (checked) {
        bg = m_colors.checkedBg;
        indicator = m_colors.indicator;
        textColor = m_colors.checkedText;
    } else if (down) {
        bg = QColor(27, 27, 36);
        indicator = m_colors.indicator;
        indicator.setAlpha(150);
        textColor = m_colors.hoverText;
    } else if (hover) {
        bg = m_colors.hoverBg;
        indicator = m_colors.indicator;
        indicator.setAlpha(100);
        textColor = m_colors.hoverText;
    } else {
        bg = Qt::transparent;
        indicator = Qt::transparent;
        textColor = m_colors.normalText;
    }

    QRect r = rect();
    
    p.setBrush(bg);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(r, kRadius, kRadius);

    if (checked) {
        QRect indicatorRect(0, r.top() + 8, kIndicatorWidth, r.height() - 16);
        p.setBrush(indicator);
        p.drawRoundedRect(indicatorRect, 2, 2);
    }

    p.setPen(textColor);
    p.setFont(font());
    p.drawText(rect(), m_textAlign, text());
}

void NavButton::enterEvent(QEnterEvent* e)
{
    QAbstractButton::enterEvent(e);
    update();
}

void NavButton::leaveEvent(QEvent* e)
{
    QAbstractButton::leaveEvent(e);
    update();
}
