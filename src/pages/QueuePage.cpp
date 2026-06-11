#include "QueuePage.h"

#include "../DownloadsModel.h"

#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

QueuePage::QueuePage(QWidget* parent)
    : QWidget(parent)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 18);
    root->setSpacing(12);

    auto* title = new QLabel("Queue");
    title->setStyleSheet("font-size: 20px; font-weight: 700;");
    root->addWidget(title);

    auto* actions = new QHBoxLayout();
    actions->setSpacing(8);
    m_start = new QPushButton("Start / Resume");
    m_start->setProperty("primary", true);
    m_pause = new QPushButton("Pause");
    m_cancel = new QPushButton("Cancel");
    m_remove = new QPushButton("Remove");
    actions->addWidget(m_start);
    actions->addWidget(m_pause);
    actions->addWidget(m_cancel);
    actions->addStretch(1);
    actions->addWidget(m_remove);
    root->addLayout(actions);

    m_table = new QTableView();
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(true);
    m_table->setSortingEnabled(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    root->addWidget(m_table, 1);

    auto* summary = new QGroupBox("Selected");
    auto* sumLay = new QHBoxLayout(summary);
    sumLay->setContentsMargins(12, 12, 12, 12);
    sumLay->setSpacing(12);

    m_progress = new QProgressBar();
    m_progress->setRange(0, 100);
    m_progress->setValue(0);
    m_progress->setTextVisible(true);
    m_progress->setMinimumHeight(24);

    auto* right = new QVBoxLayout();
    m_status = new QLabel("Status: -");
    m_speed = new QLabel("Speed: -");
    m_eta = new QLabel("ETA: -");
    m_status->setStyleSheet("color: #CFCFDE;");
    m_speed->setStyleSheet("color: #A8A8B5;");
    m_eta->setStyleSheet("color: #A8A8B5;");
    right->addWidget(m_status);
    right->addWidget(m_speed);
    right->addWidget(m_eta);
    right->addStretch(1);

    sumLay->addWidget(m_progress, 2);
    sumLay->addLayout(right, 1);
    root->addWidget(summary);

    wireButtons();
}

void QueuePage::setModel(DownloadsModel* model)
{
    m_model = model;
    m_table->setModel(model);
    m_table->setColumnWidth(DownloadsModel::ColTitle, 280);
    m_table->setColumnWidth(DownloadsModel::ColProgress, 90);
    m_table->setColumnWidth(DownloadsModel::ColStatus, 140);
    m_table->setColumnWidth(DownloadsModel::ColSpeed, 120);
    m_table->setColumnWidth(DownloadsModel::ColEta, 90);
    if (model) {
        connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &QueuePage::selectionChanged);
    }
}

void QueuePage::wireButtons()
{
    connect(m_start, &QPushButton::clicked, this, [this]() {
        const auto id = selectedId();
        if (!id.isEmpty()) emit requestStart(id);
    });
    connect(m_pause, &QPushButton::clicked, this, [this]() {
        const auto id = selectedId();
        if (!id.isEmpty()) emit requestPause(id);
    });
    connect(m_cancel, &QPushButton::clicked, this, [this]() {
        const auto id = selectedId();
        if (!id.isEmpty()) emit requestCancel(id);
    });
    connect(m_remove, &QPushButton::clicked, this, [this]() {
        const auto id = selectedId();
        if (!id.isEmpty()) emit requestRemove(id);
    });
}


QString QueuePage::selectedId() const
{
    if (!m_model) return {};
    const auto idx = m_table->currentIndex();
    if (!idx.isValid()) return {};
    const auto* it = m_model->itemAt(idx.row());
    return it ? it->id : QString();
}

void QueuePage::setSelectedSummary(int percent, const QString& status, const QString& speed, const QString& eta)
{
    m_progress->setValue(percent);
    m_status->setText(QString("Status: %1").arg(status));
    m_speed->setText(QString("Speed: %1").arg(speed.isEmpty() ? "-" : speed));
    m_eta->setText(QString("ETA: %1").arg(eta.isEmpty() ? "-" : eta));
}
