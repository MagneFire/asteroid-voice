
#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioDeviceInfo;
class QFile;
class AudioIODevice;
QT_END_NAMESPACE

class AudioRecorder : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool recording READ isRecording NOTIFY recordingChanged)
    Q_PROPERTY(double audio READ audio NOTIFY audioChanged)
    Q_PROPERTY(bool audioSamples READ audioSamples NOTIFY audioSamplesChanged)

public:
    AudioRecorder(QObject *parent = 0);

    bool isRecording();
    bool audioSamples();
    double audio();

    Q_INVOKABLE void record(const QString fileName);

signals:
    void recordingChanged();
    void audioSamplesChanged();
    void audioChanged();

public slots:
    Q_INVOKABLE void stop();

private:
    QAudioInput *m_audioInput;
    AudioIODevice *m_device;
};

#endif // AUDIORECORDER_H