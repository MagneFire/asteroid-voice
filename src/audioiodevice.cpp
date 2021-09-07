
#include "audioiodevice.h"

#include <QQuickWindow>
#include <QTimer>
#include <QDebug>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioDeviceInfo>

AudioIODevice::AudioIODevice(QAudioFormat formatAudio, QObject *parent) : QIODevice(parent), m_formatAudio(formatAudio)
{
}

void AudioIODevice::writeHeader() {
    // Code curtesy of https://forum.qt.io/topic/6744/solved-how-to-form-wav-header/6
    QDataStream out(&m_outputFile);
    out.setByteOrder(QDataStream::LittleEndian);

    // RIFF chunk
    out.writeRawData("RIFF", 4);
    out << quint32(0); // Placeholder for the RIFF chunk size (filled by close())
    out.writeRawData("WAVE", 4);

    // Format description chunk
    out.writeRawData("fmt ", 4);
    out << quint32(16); // "fmt " chunk size (always 16 for PCM)
    out << quint16(1); // data format (1 => PCM)
    out << quint16(m_formatAudio.channelCount());
    out << quint32(m_formatAudio.sampleRate());
    out << quint32(m_formatAudio.sampleRate() * m_formatAudio.channelCount() * m_formatAudio.sampleSize() / 8 ); // bytes per second
    out << quint16(m_formatAudio.channelCount() * m_formatAudio.sampleSize() / 8); // Block align
    out << quint16(m_formatAudio.sampleSize()); // Significant Bits Per Sample

    // Data chunk
    out.writeRawData("data", 4);
    out << quint32(0); // Placeholder for the data chunk size (filled by close())
}

bool AudioIODevice::open(QIODevice::OpenMode mode, const QString fileName) {
    m_outputFile.setFileName(fileName);
    m_outputFile.open(QIODevice::WriteOnly);
    writeHeader();
    return QIODevice::open(mode);
}

void AudioIODevice::close() {
    // Fill the header size placeholders
    quint32 fileSize = m_outputFile.size();

    QDataStream out(&m_outputFile);
    // Set the same ByteOrder like in writeHeader()
    out.setByteOrder(QDataStream::LittleEndian);
    // RIFF chunk size
    seek(4);
    out << quint32(fileSize - 8);

    // data chunk size
    seek(40);
    out << quint32(fileSize - 44);

    m_outputFile.close();
    QIODevice::close();
}

qint64 AudioIODevice::readData(char *data, qint64 maxSize)
{
    return -1;
}

bool AudioIODevice::audioSamples() {
    return 0;
}

double AudioIODevice::audio() {
    return m_buffer;
}

qint64 AudioIODevice::writeData(const char *data, qint64 maxSize) {
    m_outputFile.write(data, maxSize);
    double val = 0;
    // Find max value used for audio visualization
    for (int i=0; i < maxSize; i++, data ++) {
        double d = qAbs(qreal(uchar(*data) -128) / qreal(128));
        if (d > val) {
            val = d;
        }
    }
    m_buffer = (val);
    emit audioChanged();
    emit audioSamplesChanged();
    return maxSize;
}