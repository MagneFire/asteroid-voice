
#ifndef AUDIOIODEVICE_H
#define AUDIOIODEVICE_H

#include <QObject>
#include <QIODevice>
#include <QQueue>
#include <QFile>
#include <QAudioFormat>

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioDeviceInfo;
QT_END_NAMESPACE

class AudioIODevice : public QIODevice
{
    Q_OBJECT

public:
    AudioIODevice(QAudioFormat formatAudio, QObject *parent = 0);
    static const int sampleCount = 2000;

    bool audioSamples();
    double audio();

    bool open(QIODevice::OpenMode mode, const QString fileName);
    void close();

signals:
    void audioSamplesChanged();
    void audioChanged();

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;
private:
    void writeHeader();

    double m_buffer;
    QFile m_outputFile;
    QAudioFormat m_formatAudio;

};

#endif // AUDIOIODEVICE_H