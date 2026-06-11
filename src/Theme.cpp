#include "Theme.h"

#include <QPalette>
#include <QStyleFactory>

void Theme::applyDark(QApplication& app)
{
    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette p;
    p.setColor(QPalette::Window, QColor(18, 18, 20));
    p.setColor(QPalette::WindowText, QColor(235, 235, 235));
    p.setColor(QPalette::Base, QColor(26, 26, 30));
    p.setColor(QPalette::AlternateBase, QColor(32, 32, 38));
    p.setColor(QPalette::ToolTipBase, QColor(26, 26, 30));
    p.setColor(QPalette::ToolTipText, QColor(235, 235, 235));
    p.setColor(QPalette::Text, QColor(235, 235, 235));
    p.setColor(QPalette::Button, QColor(28, 28, 34));
    p.setColor(QPalette::ButtonText, QColor(235, 235, 235));
    p.setColor(QPalette::BrightText, QColor(255, 70, 70));
    p.setColor(QPalette::Link, QColor(95, 180, 255));
    p.setColor(QPalette::Highlight, QColor(70, 130, 255));
    p.setColor(QPalette::HighlightedText, QColor(10, 10, 12));
    p.setColor(QPalette::Disabled, QPalette::Text, QColor(140, 140, 150));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(140, 140, 150));
    app.setPalette(p);

    app.setStyleSheet(QStringLiteral(R"CSS(
      QMainWindow { background: #121214; }
      QWidget { font-size: 13px; }
      QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit, QPlainTextEdit {
        background: #1A1A1E;
        border: 1px solid #2A2A33;
        border-radius: 8px;
        padding: 8px 10px;
        selection-background-color: #4682FF;
      }
      QComboBox::drop-down { border: none; width: 24px; }
      QComboBox QAbstractItemView {
        background: #1A1A1E;
        border: 1px solid #2A2A33;
        selection-background-color: #2E5BBD;
      }
      QPushButton {
        background: #242430;
        border: 1px solid #303040;
        border-radius: 10px;
        padding: 8px 12px;
      }
      QPushButton:hover { background: #2B2B3A; }
      QPushButton:pressed { background: #20202A; }
      QPushButton:disabled { color: #8C8C96; background: #1B1B22; border-color: #272732; }
      QPushButton[primary="true"] {
        background: #2E5BFF;
        border-color: #2E5BFF;
        color: #0B0B10;
        font-weight: 600;
      }
      QPushButton[primary="true"]:hover { background: #3C69FF; border-color: #3C69FF; }
      QToolButton {
        background: transparent;
        border: 1px solid transparent;
        border-radius: 10px;
        padding: 8px 12px;
        color: #CFCFDE;
        transition: all 150ms ease;
      }
      QToolButton:hover { 
        background: #1B1B24; 
        border-color: #2A2A33;
        border-radius: 10px;
      }
      QToolButton:checked { 
        background: #202034; 
        border-color: #2E5BFF;
        border-radius: 10px;
      }
      QToolButton:pressed { 
        background: #1A1A28;
        border-radius: 10px;
      }
      QGroupBox {
        border: 1px solid #2A2A33;
        border-radius: 12px;
        margin-top: 12px;
      }
      QGroupBox::title {
        subcontrol-origin: margin;
        left: 12px;
        padding: 0 6px;
      }
      QHeaderView::section {
        background: #151518;
        color: #CFCFDE;
        border: none;
        border-bottom: 1px solid #2A2A33;
        padding: 8px 10px;
      }
      QTableView {
        background: #121214;
        border: 1px solid #2A2A33;
        border-radius: 12px;
        gridline-color: #2A2A33;
        selection-background-color: #203060;
        selection-color: #FFFFFF;
      }
      QTableView::item { padding: 6px 8px; }
      QProgressBar {
        border: 1px solid #2A2A33;
        border-radius: 8px;
        background: #1A1A1E;
        text-align: center;
      }
      QProgressBar::chunk {
        border-radius: 7px;
        background: #2E5BFF;
      }
      QScrollBar:vertical {
        background: transparent;
        width: 12px;
        margin: 4px 2px 4px 2px;
      }
      QScrollBar::handle:vertical {
        background: #2A2A33;
        border-radius: 6px;
        min-height: 24px;
      }
      QScrollBar::handle:vertical:hover { background: #3A3A48; }
      QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }
      QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }
    )CSS"));
}
