/*
 *  AudioFilePlayers.cpp
 *  Aserve
 *
 *  Created by tjmitche on 06/08/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "AudioFilePlayerManager.h"

AudioFilePlayerManager::AudioFilePlayerManager()
{
    for (int i = 0; i < NumPlayers; i++) 
    {
        players.add(new AudioFilePlayer());
        mixerSource.addInputSource(players[i],false);
        players[i]->addListener(this);
    }
}

AudioFilePlayerManager::~AudioFilePlayerManager()
{
    for (int i = 0; i < NumPlayers; i++) 
    {
        players[i]->removeListener(this);
    }
    //mixerSource.releaseResources(); this should happed in the release resources callback?
    mixerSource.removeAllInputs();
}

void AudioFilePlayerManager::addListener(Listener *listenerToAdd)
{
    if (listenerToAdd != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.add(listenerToAdd);
    }
}

void AudioFilePlayerManager::removeListener(Listener *listenerToRemove)
{
    if (listenerToRemove != 0) 
    {
        //ScopedLock sl(listenerLock);
        listeners.remove(listenerToRemove);
    }
}


void AudioFilePlayerManager::loadAudioFile (const int playerIndex, const String& path)
{ 
    players[playerIndex]->loadAudioFile(path);
}

void AudioFilePlayerManager::play(const int playerIndex, const float gain)
{
    players[playerIndex]->play(gain);
}

void AudioFilePlayerManager::stop(const int playerIndex)
{
    players[playerIndex]->stop();
}

void AudioFilePlayerManager::togglePlayState(const int playerIndex)
{
    players[playerIndex]->togglePlayState();
}

void AudioFilePlayerManager::stopAll()
{
   for (int i = 0; i < NumPlayers; ++i) 
   {
       stop(i);
       loadAudioFile (i, String::empty);
   }
}


//AudioSourceCallbacks
void AudioFilePlayerManager::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioFilePlayerManager::releaseResources()
{
    mixerSource.releaseResources();
}

void AudioFilePlayerManager::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

//Changelistener Callback
void AudioFilePlayerManager::audioFileNameChanged(const AudioFilePlayer *player, const String &path)
{
    for (int i = 0; i < NumPlayers; ++i) 
    {
        if (players[i] == player) 
        {
            listeners.call(&Listener::audioFileNameChanged, i, path);
        }
    }
}

void AudioFilePlayerManager::audioFilePlayStateChanged(const AudioFilePlayer *player, const AudioFilePlayer::PlayState state)
{
    for (int i = 0; i < NumPlayers; ++i) 
    {
        if (players[i] == player) 
        {
            listeners.call(&Listener::audioFilePlayStateChanged, i, static_cast<PlayState> (state));
        }
    }
}