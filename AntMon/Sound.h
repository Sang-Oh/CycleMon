#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#pragma  comment( lib, "fmod_vc.lib" )

#define MAX_SOUND_COUNT 10

#define SND_CADENCE 0
#define SND_10_STOP	 1
#define SND_HALF		2
#define SND_5_START		4
#define SND_5_STOP		5
#define SND_REST		6
#define SND_WORKOUT		7
class CSound
{
public:
	CSound();
	~CSound();
	void LOADSoundAll();
	void ERRCHECK(FMOD_RESULT result);
	void PLAYsound(char* szSound);
	void DeleteSound();
public:
	FMOD::System   * System;
private:
	FMOD::Sound   * cadenceSound;

	FMOD::Sound   * m_sndWarmup_start;
	FMOD::Sound   * m_sndHalf;
	FMOD::Sound   * m_sndInterval_start;
	FMOD::Sound   * m_snd5start;
	FMOD::Sound   * m_snd1mstop;
	FMOD::Sound   * m_snd5stop;
	FMOD::Sound   * m_sndRest;
	FMOD::Sound   * m_sndCooldown_start;
	FMOD::Sound   * m_sndCooldown_stop;

	FMOD::Channel  * channel;
};

