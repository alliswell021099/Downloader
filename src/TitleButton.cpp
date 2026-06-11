#include "TitleButton.h"

#include <QPaintEvent>
#include <QPainter>

namespace {
constexpr int kBtnWidth = 46;
constexpr int kBtnHeight = 36;
constexpr int kIconSide = 12;
constexpr qreal kStroke = 1.0;
} // namespace

TitleButton::TitleButton(Glyph g, QWidget* parent)
    : QAbstractButton(parent), m_glyph(g)
{
    setFixedSize(kBtnWidth, kBtnHeight);
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_Hover, true);
}

void TitleButton::setGlyph(Glyph g)
{
    if (m_glyph == g) return;
    m_glyph = g;
    update();
}

QSize TitleButton::sizeHint() const
{
    return QSize(kBtnWidth, kBtnHeight);
}

QSize TitleButton::minimumSizeHint() const
{
    return QSize(kBtnWidth, kBtnHeight);
}

void TitleButton::paintEvent(QPaintEvent* /*e*/)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const bool hover = underMouse();
    const bool down = isDown();

    // Hover / press background
    QColor bg;
    if (m_glyph == Close) {
        if (down) bg = QColor(180, 30, 20);
        else if (hover) bg = QColor(196, 43, 28);
    } else {
        if (down) bg = QColor(20, 20, 28);
        else if (hover) bg = QColor(27, 27, 36);
    }
    if (bg.isValid()) p.fillRect(rect(), bg);

    // Icon color
    QColor iconColor;
    if (m_glyph == Close) {
        iconColor = hover ? QColor(255, 255, 255) : QColor(168, 168, 181);
    } else {
        iconColor = hover ? QColor(235, 235, 235) : QColor(168, 168, 181);
    }

    p.setPen(QPen(iconColor, kStroke, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.setBrush(Qt::NoBrush);

    const qreal cx = width() / 2.0;
    const qreal cy = height() / 2.0;
    const qreal x = cx - kIconSide / 2.0;
    const qreal y = cy - kIconSide / 2.0;
    const qreal w = kIconSide;
    const qreal h = kIconSide;

    switch (m_glyph) {
    case Minimize: {
        // Single horizontal line at vertical center
        p.drawLine(QPointF(x + 1, cy), QPointF(x + w - 1, cy));
        break;
    }
    case Maximize: {
        // Square outline
        p.drawRect(QRectF(x, y, w - 1, h - 1));
        break;
    }
    case Restore: {
        // Front square (bottom-left): full outline
        const qreal fx = x;
        const qreal fy = y + h * 4.0 / 12.0;
        const qreal fs = w * 7.0 / 12.0;
        p.drawRect(QRectF(fx, fy, fs, fs));
        // Back square (top-right): only top + right edges visible
        const qreal bx = x + w * 5.0 / 12.0;
        const qreal by = y;
        const qreal bs = w * 7.0 / 12.0;
        p.drawLine(QPointF(bx, by), QPointF(bx + bs, by));               // top edge
        p.drawLine(QPointF(bx + bs, by), QPointF(bx + bs, by + bs));     // right edge
        break;
    }
    case Close: {
        // X
        p.drawLine(QPointF(x + 1, y + 1), QPointF(x + w - 1, y + h - 1));
        p.drawLine(QPointF(x + w - 1, y + 1), QPointF(x + 1, y + h - 1));
        break;
    }
    }
}
