#include "gamesound.h"
#include <QDebug>
#include <QTimer>

GameSound::GameSound(QObject *parent) : QObject(parent)
{    
    qRegisterMetaType<GameSound::SoundType>("GameSound::SoundType");
    soundSourceHash[SoundType::KillSound] = QUrl::fromLocalFile("./music/KillAudio.wav");
    soundSourceHash[SoundType::TankMoveSound] = QUrl::fromLocalFile("./music/TankMove.wav");
    soundSourceHash[SoundType::ArmorFireSound] = QUrl::fromLocalFile("./music/ArmorFireDistant.wav");
    soundSourceHash[SoundType::MachineGunFireSound] = QUrl::fromLocalFile("./music/MachineGunFire.wav");
    soundSourceHash[SoundType::SelectWeaponSound] = QUrl::fromLocalFile("./music/SelectWeapon.wav");
    soundSourceHash[SoundType::TankExplosion] = QUrl::fromLocalFile("./music/TankExplosion.wav");
}

void GameSound::play(GameSound::SoundType type, int loopCount, bool isInstant)
{
    if(isInstant)
    {
        QSoundEffect* effect = new QSoundEffect(this);
        connect(effect, &QSoundEffect::playingChanged, [effect]()
        {
            if(!effect->isPlaying())
            {
                effect->deleteLater();
                qDebug() << "sound effect deleted";
            }
        });

        effect->setLoopCount(loopCount);
        effect->setSource(soundSourceHash[type]);

        if(type == TankExplosion)
            effect->setVolume(0.5f);

        effect->play();
    }
    else
    {   
        if(!soundEffectHash.contains(type))
        {
            soundEffectHash[type] = new QSoundEffect(this);
            soundEffectHash[type]->setSource(soundSourceHash[type]);
        }

        auto effect = soundEffectHash[type];
        if(effect->isPlaying() && type == MachineGunFireSound)
            return;

        effect->setLoopCount(loopCount);
        effect->play();
    }
}

void GameSound::stop(GameSound::SoundType type)
{
    if(!soundEffectHash.contains(type))
        return;

    if(type == MachineGunFireSound)
    {
        soundEffectHash[type]->setLoopCount(1);
    }
    else
        soundEffectHash[type]->stop();
}
