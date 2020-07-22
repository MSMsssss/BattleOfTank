#ifndef GAMESOUND_H
#define GAMESOUND_H

#include <QObject>
#include <QSoundEffect>
#include <QUrl>
#include <QHash>
#include <QMetaType>

class GameSound : public QObject
{
    Q_OBJECT

public:
    enum SoundType{
        KillSound,
        ArmorFireSound,
        MachineGunFireSound,
        SelectWeaponSound,
        TankMoveSound,
        TankExplosion
    };
    Q_ENUM(SoundType)

public:
    explicit GameSound(QObject *parent = nullptr);

protected:
    QHash<SoundType, QSoundEffect*> soundEffectHash;
    QHash<SoundType, QUrl> soundSourceHash;

signals:

public slots:
    //  播放声音
    void play(GameSound::SoundType type, int count, bool isInstant);

    //  需要循环播放的类型可以停止
    void stop(GameSound::SoundType type);
};

#endif // GAMESOUND_H
