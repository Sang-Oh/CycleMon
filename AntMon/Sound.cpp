#include "stdafx.h"
#include "Sound.h"

#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
CSound::CSound(void)
{
	System = NULL;
	cadenceSound = NULL;
	m_sndHalf = NULL;
	m_sndInterval_start = NULL;
	m_snd5start = NULL;
	m_snd1mstop = NULL;
	m_snd5stop = NULL;
	m_sndRest = NULL;
	m_sndCooldown_start = NULL;
	m_sndCooldown_stop = NULL;

	channel = NULL;
	LOADSoundAll();//sound loading 클래스 생성시 한번만 호출

	FMOD_RESULT  result;
	result = System->playSound(cadenceSound, NULL, FALSE, &channel);
	channel->setVolume(1.f);
	ERRCHECK(result);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CSound::~CSound(void)
{
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------//사운드를 로드시켜 줌. 메쉬 모델을 로드시켜주는 것과 같은 맥락.
void CSound::LOADSoundAll()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&System);
	ERRCHECK(result);
	result = System->init(100, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
	result = System->createSound("sound\\tick.wav", FMOD_DEFAULT, 0, &cadenceSound);
	ERRCHECK(result);
	result = System->createSound("sound\\warmup_start.wma", FMOD_DEFAULT, 0, &m_sndWarmup_start);
	ERRCHECK(result);
	result = System->createSound("sound\\half.wma", FMOD_DEFAULT, 0, &m_sndHalf);
	ERRCHECK(result);
	result = System->createSound("sound\\interval_start.wma", FMOD_DEFAULT, 0, &m_sndInterval_start);
	ERRCHECK(result);
	result = System->createSound("sound\\5start.wma", FMOD_DEFAULT, 0, &m_snd5start);
	ERRCHECK(result);
	result = System->createSound("sound\\1mstop.wma", FMOD_DEFAULT, 0, &m_snd1mstop);
	ERRCHECK(result);
	result = System->createSound("sound\\5stop.wma", FMOD_DEFAULT, 0, &m_snd5stop);
	ERRCHECK(result);
	result = System->createSound("sound\\1mstop.wma", FMOD_DEFAULT, 0, &m_snd1mstop);
	ERRCHECK(result);
	result = System->createSound("sound\\rest.wma", FMOD_DEFAULT, 0, &m_sndRest);
	ERRCHECK(result);
	result = System->createSound("sound\\cooldown_start.wma", FMOD_DEFAULT, 0, &m_sndCooldown_start);
	ERRCHECK(result);
	result = System->createSound("sound\\cooldown_stop.wma", FMOD_DEFAULT, 0, &m_sndCooldown_stop);
	ERRCHECK(result);
}
void CSound::ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		char str[256];
		sprintf(str, "FMOD error! ( %d ) %s\n", result, FMOD_ErrorString(result));
		MessageBox(NULL, str, "TEST", MB_OK);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//함수 인자로 문자열을 입력하면 상황에 따른 사운드를 재생.
void CSound::PLAYsound(char* szSound)
{
	if (szSound == NULL || strlen(szSound)==0) return;

	FMOD_RESULT result;
	result = System->update();
	ERRCHECK(result);

	if (strcmp("rpm", szSound)==0)
	{
		result = System->playSound(cadenceSound, NULL, FALSE, &channel);
		channel->setVolume(0.8f);
		ERRCHECK(result);
	}
	else if (strcmp("1mstop", szSound) == 0)
	{
		result = System->playSound(m_snd1mstop, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	} else if (strcmp("5start", szSound) == 0)
	{
		result = System->playSound(m_snd5start, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	}
	else if (strcmp("5stop", szSound) == 0)
	{
		result = System->playSound(m_snd5stop, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	}
	else if (strcmp("cooldown_start", szSound) == 0)
	{
		result = System->playSound(m_sndCooldown_start, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	} else if (strcmp("cooldown_stop", szSound) == 0)
	{
		result = System->playSound(m_sndCooldown_stop, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	}
	else if (strcmp("half", szSound) == 0)
	{
		result = System->playSound(m_sndHalf, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	}
	else if (strcmp("interval_start", szSound) == 0)
	{
		result = System->playSound(m_sndInterval_start, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	}
	else if (strcmp("rest", szSound) == 0)
	{
		result = System->playSound(m_sndRest, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	}
	else if (strcmp("warmup_start", szSound) == 0)
	{
		result = System->playSound(m_sndWarmup_start, NULL, FALSE, &channel);
		channel->setVolume(1.f);
		ERRCHECK(result);
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CSound::DeleteSound()
{
	if (cadenceSound) { SAFE_DELETE(cadenceSound); }
}