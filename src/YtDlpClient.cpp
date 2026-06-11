#include "YtDlpClient.h"

#include <cmath>
#include <QDateTime>
#include <QRegularExpression>
#include <QStandardPaths>

YtDlpClient::YtDlpClient(QObject* parent)
    : QObject(parent)
{
}

void YtDlpClient::setExecutablePath(QString path)
{
    if (path.trimmed().isEmpty()) return;
    m_exe = std::move(path);
}

QString YtDlpClient::executablePath() const
{
    return m_exe;
}

bool YtDlpClient::isRunning(const QString& id) const
{
    auto* p = m_procById.value(id, nullptr);
    return p && p->state() != QProcess::NotRunning;
}

QStringList YtDlpClient::buildArgs(const DownloadItem& item, bool isResume) const
{
    QString outDir = item.outputDir;
    if (outDir.trimmed().isEmpty()) {
        outDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    }

    QString tpl = item.outputTemplate.trimmed();
    if (tpl.isEmpty()) {
        tpl = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".%(ext)s";
    }

    QString outPath = outDir;
    if (!outPath.endsWith('/') && !outPath.endsWith('\\')) outPath += "\\";
    outPath += tpl;

    QStringList args;
    args << "--newline";
    args << "--no-color";
    args << "--progress";
    args << "--output" << outPath;

    if (!item.formatId.trimmed().isEmpty()) {
        args << "-f" << item.formatId.trimmed();
    }

    if (isResume) args << "--continue";

    args << item.url;
    return args;
}

void YtDlpClient::attachProcess(const QString& id, QProcess* p)
{
    connect(p, &QProcess::readyReadStandardOutput, this, [this, id, p]() {
        while (p->canReadLine()) {
            const QString line = QString::fromUtf8(p->readLine()).trimmed();
            if (!line.isEmpty()) {
                emit downloadLog(id, line);
                handleOutputLine(id, line);
            }
        }
    });

    connect(p, &QProcess::readyReadStandardError, this, [this, id, p]() {
        const QString chunk = QString::fromUtf8(p->readAllStandardError());
        m_stderrBuf[id].append(chunk);
        for (const auto& line : chunk.split('\n')) {
            const QString t = line.trimmed();
            if (t.isEmpty()) continue;
            emit downloadLog(id, t);
        }
    });

    connect(p, &QProcess::errorOccurred, this, [this, id](QProcess::ProcessError) {
        emit downloadFinished(id, false, "Failed to start yt-dlp process. Check the executable path in Settings.");
    });

    connect(p, &QProcess::finished, this, [this, id, p]() {
        const int exitCode = p->exitCode();
        const QProcess::ExitStatus status = p->exitStatus();
        const bool ok = (status == QProcess::NormalExit && exitCode == 0);
        QString err;
        if (!ok) {
            const QString stderr_str = m_stderrBuf.value(id).trimmed();
            err = QString("yt-dlp exited with code %1").arg(exitCode);
            if (!stderr_str.isEmpty()) {
                const QString lastLine = stderr_str.split('\n').last().trimmed();
                if (!lastLine.isEmpty()) err += "\n" + lastLine;
            }
        }
        m_stderrBuf.remove(id);
        emit downloadFinished(id, ok, err);
        m_procById.remove(id);
        p->deleteLater();
    });
}

void YtDlpClient::handleOutputLine(const QString& id, const QString& line)
{
    static const QRegularExpression re(R"(\[download\]\s+([0-9]+(?:\.[0-9]+)?)%\s+.*?\s+at\s+([^\s]+)\s+ETA\s+([0-9:]+))");
    auto m = re.match(line);
    if (m.hasMatch()) {
        const int percent = qBound(0, (int)std::lround(m.captured(1).toDouble()), 100);
        emit downloadProgress(id, percent, m.captured(2), m.captured(3), "Downloading");
    }
}

void YtDlpClient::startDownload(const DownloadItem& item)
{
    if (item.id.trimmed().isEmpty()) return;
    if (isRunning(item.id)) return;

    m_stderrBuf.remove(item.id);

    auto* p = new QProcess(this);
    p->setProgram(m_exe);
    QStringList args = buildArgs(item, false);
    qDebug() << "Resuming with args:" << args;
    p->setArguments(buildArgs(item, false));
    p->setProcessChannelMode(QProcess::SeparateChannels);
    if (!item.outputDir.trimmed().isEmpty()) {
        p->setWorkingDirectory(item.outputDir);
    }

    m_procById.insert(item.id, p);
    attachProcess(item.id, p);

    emit downloadStarted(item.id);
    p->start();
}

void YtDlpClient::cancel(const QString& id)
{
    auto* p = m_procById.value(id, nullptr);
    if (!p) return;
    p->kill();
}

void YtDlpClient::pause(const QString& id)
{
    auto* p = m_procById.value(id, nullptr);
    if (!p) return;
    p->terminate();
}

void YtDlpClient::resume(const DownloadItem& item)
{
    if (item.id.trimmed().isEmpty()) return;
    if (isRunning(item.id)) return;

    m_stderrBuf.remove(item.id);

    auto* p = new QProcess(this);
    p->setProgram(m_exe);
    QStringList args = buildArgs(item, true);
    qDebug() << "Resuming with args:" << args;
    p->setArguments(args);
    p->setProcessChannelMode(QProcess::SeparateChannels);
    if (!item.outputDir.trimmed().isEmpty()) {
        p->setWorkingDirectory(item.outputDir);
    }

    m_procById.insert(item.id, p);
    attachProcess(item.id, p);

    emit downloadStarted(item.id);
    p->start();
}
