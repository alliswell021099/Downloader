#include "DarkNotifyDialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

DarkNotifyDialog::DarkNotifyDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Notification"));
    setMinimumWidth(420);
    setStyleSheet(
        "DarkNotifyDialog { background: #121214; border: 1px solid #2A2A33; border-radius: 12px; }"
        "QLabel { color: #EBEBEB; font-size: 13px; }"
        "QPushButton { background: #242430; border: 1px solid #303040; border-radius: 8px; padding: 8px 16px; color: #EBEBEB; }"
        "QPushButton:hover { background: #2B2B3A; }"
        "QPushButton:pressed { background: #20202A; }"
    );
}

void DarkNotifyDialog::showError(QWidget* parent, const QString& title, const QString& message)
{
    auto* dlg = new DarkNotifyDialog(parent);
    dlg->setWindowTitle(title);

    auto* layout = new QVBoxLayout(dlg);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    auto* iconLabel = new QLabel("⚠");
    iconLabel->setStyleSheet("color: #FF8A65; font-size: 28px;");
    iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(iconLabel);

    auto* msgLabel = new QLabel(message);
    msgLabel->setWordWrap(true);
    msgLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgLabel->setStyleSheet("color: #CFCFDE; padding: 0 8px;");
    layout->addWidget(msgLabel);

    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    btnBox->setCenterButtons(true);
    connect(btnBox, &QDialogButtonBox::accepted, dlg, &QDialog::accept);
    layout->addWidget(btnBox);

    dlg->exec();
    dlg->deleteLater();
}

void DarkNotifyDialog::showInfo(QWidget* parent, const QString& title, const QString& message)
{
    auto* dlg = new DarkNotifyDialog(parent);
    dlg->setWindowTitle(title);

    auto* layout = new QVBoxLayout(dlg);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    auto* msgLabel = new QLabel(message);
    msgLabel->setWordWrap(true);
    msgLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    msgLabel->setStyleSheet("color: #CFCFDE; padding: 0 8px;");
    layout->addWidget(msgLabel);

    auto* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    btnBox->setCenterButtons(true);
    connect(btnBox, &QDialogButtonBox::accepted, dlg, &QDialog::accept);
    layout->addWidget(btnBox);

    dlg->exec();
    dlg->deleteLater();
}
