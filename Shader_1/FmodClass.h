#pragma once


#include <fmod.hpp>
#include <fmod_errors.h>

#define SPECTRUM_SIZE 128 //Rec = 128 or 512
//#define 

//FMOD ERROR CHECKING
void ERRORCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! : " << FMOD_ErrorString(result) << std::endl;
	}
}

//FMOD
FMOD::System* m_fmodSystem;
FMOD::Sound* m_music[3];
FMOD::Channel* m_musicChannel;
float m_Spectrum_Left[SPECTRUM_SIZE];
float m_Spectrum_Right[SPECTRUM_SIZE];

static bool pause = true;

//FMOD
void initFMOD()
{
	FMOD_RESULT		results;
	unsigned int	version;

	results = FMOD::System_Create(&m_fmodSystem);
	ERRORCHECK(results);

	results = m_fmodSystem->getVersion(&version);
	ERRORCHECK(results);

	if (version < FMOD_VERSION)
	{
		std::cout << "FMOD Error! You are using old version of FMOD" << std::endl;
	}

	//Initialize
	results = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
	ERRORCHECK(results);

	//Load
	results = m_fmodSystem->createStream("../Media/After Dark - Aimer.mp3", FMOD_SOFTWARE, 0, &m_music[0]);
	ERRORCHECK(results);

	results = m_fmodSystem->createStream("../Media/From The Earth - Abridged Version.mp3", FMOD_SOFTWARE, 0, &m_music[1]);
	ERRORCHECK(results);

	results = m_fmodSystem->createStream("../Media/Zaregoto OST - old fashioned fairy tale.mp3", FMOD_SOFTWARE, 0, &m_music[2]);
	ERRORCHECK(results);

	//Play
	results = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music[0], false, &m_musicChannel);
	ERRORCHECK(results);

	results = m_music[0]->setMode(FMOD_LOOP_NORMAL);
	ERRORCHECK(results);

	results = m_music[0]->setLoopCount(-1);
	ERRORCHECK(results);
}

void UpdateFMOD(GLFWwindow* window)
{
	m_fmodSystem->update();

	//Get Spectrum for Left and Right Stereo Channels
	m_musicChannel->getSpectrum(m_Spectrum_Left, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
	m_musicChannel->getSpectrum(m_Spectrum_Right, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);
}

void PlayOrPause(int action)
{
	if (action == GLFW_PRESS)
	{
		pause = !pause;
		m_musicChannel->setPaused(pause);
	}
}

void ChangeMusic(int action, int musicID)
{
	FMOD_RESULT results;

	if (action == GLFW_PRESS)// && pressed == false)
	{
		m_musicChannel->stop();

		results = m_fmodSystem->playSound(FMOD_CHANNELINDEX::FMOD_CHANNEL_FREE, m_music[musicID], true, &m_musicChannel);
		ERRORCHECK(results);

		results = m_music[musicID]->setMode(FMOD_LOOP_NORMAL);
		ERRORCHECK(results);

		results = m_music[musicID]->setLoopCount(-1);
		ERRORCHECK(results);

		m_musicChannel->setPaused(false);
	}
}
