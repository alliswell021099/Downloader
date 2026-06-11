#pragma once

#include <QDialog>

class DarkNotifyDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit DarkNotifyDialog(QWidget* parent = nullptr);

    static void showError(QWidget* parent, const QString& title, const QString& message);
    static void showInfo(QWidget* parent, const QString& title, const QString& message);
};
