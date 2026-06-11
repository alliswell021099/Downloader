#pragma once

#include <QAbstractButton>

class TitleButton : public QAbstractButton
{
    Q_OBJECT
public:
    enum Glyph {
        Minimize,
        Maximize,
        Restore,
        Close,
    };

    explicit TitleButton(Glyph g, QWidget* parent = nullptr);

    void setGlyph(Glyph g);
    Glyph glyph() const { return m_glyph; }

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* e) override;

private:
    Glyph m_glyph;
};
