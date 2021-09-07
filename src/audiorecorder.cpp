#include "audiorecorder.h"
#include "audioiodevice.h"

#include <QQuickWindow>
#include <QTimer>
#include <QDebug>
#include <QDir>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioDeviceInfo>

AudioRecorder::AudioRecorder(QObject *parent) : QObject(parent) {
    const QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultInputDevice();
    if (deviceInfo.isNull()) {
        qDebug() << "There is no audio input device available.";
    }

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(8);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);

    if (!deviceInfo.isFormatSupported(formatAudio)) {
        qWarning()<<"default format not supported try to use nearest";
        formatAudio = deviceInfo.nearestFormat(formatAudio);
    }
    m_audioInput = new QAudioInput(deviceInfo, formatAudio, this);

    m_device = new AudioIODevice(formatAudio, this);
    connect(m_device, &AudioIODevice::audioChanged, this, &AudioRecorder::audioChanged);
    connect(m_device, &AudioIODevice::audioSamplesChanged, this, &AudioRecorder::audioSamplesChanged);
}

bool AudioRecorder::isRecording() {
    qDebug() << m_audioInput->state() ;
    return m_audioInput->state() == QAudio::ActiveState
            ||  m_audioInput->state() == QAudio::IdleState;
}

bool AudioRecorder::audioSamples() {
    return m_device->audioSamples();
}

double AudioRecorder::audio() {
    return m_device->audio();
}

void AudioRecorder::record(const QString fileName) {
    QDir().mkpath(QDir::homePath() + "/recordings/");
    m_device->open(QIODevice::WriteOnly, QDir::homePath() + "/recordings/" + fileName);
    m_audioInput->start(m_device);
    emit recordingChanged();
}

void AudioRecorder::stop() {
    m_audioInput->stop();
    m_device->close();
    emit recordingChanged();
}
