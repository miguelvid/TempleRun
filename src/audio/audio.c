#include "audio.h"

#include <stdio.h>

#include "miniaudio.h"   // declarations only; the implementation lives in vendor/
#include "config.h"      // TOIN_PITCH / TOIN_SKIP_MS tuning

static ma_engine g_engine;
static ma_sound  g_running;
static ma_sound  g_music;
static ma_sound  g_toin;        // crash sting: managed so we can pitch/seek it
static int g_engineOk = 0;
static int g_runningLoaded = 0;
static int g_musicLoaded = 0;
static int g_toinLoaded = 0;
static int g_runningOn = 0;

void audioInit(void) {
	if (ma_engine_init(NULL, &g_engine) != MA_SUCCESS) {
		printf("[audio] could not start the audio engine (sound disabled)\n");
		return;
	}
	g_engineOk = 1;

	// running sound: decoded fully so it loops seamlessly
	if (ma_sound_init_from_file(&g_engine, "assets/running.mp3",
	                            MA_SOUND_FLAG_DECODE, NULL, NULL, &g_running) == MA_SUCCESS) {
		ma_sound_set_looping(&g_running, MA_TRUE);
		g_runningLoaded = 1;
	} else {
		printf("[audio] could not load assets/running.mp3\n");
	}

	// background music (drums): loops continuously, kept low under the SFX
	if (ma_sound_init_from_file(&g_engine, "assets/drum.mp3",
	                            MA_SOUND_FLAG_DECODE, NULL, NULL, &g_music) == MA_SUCCESS) {
		ma_sound_set_looping(&g_music, MA_TRUE);
		ma_sound_set_volume(&g_music, 0.45f);
		ma_sound_start(&g_music);
		g_musicLoaded = 1;
	} else {
		printf("[audio] could not load assets/drum.mp3\n");
	}

	// crash sting: decoded and kept around so each crash can pitch/seek it
	// (the clip is long-winded; we speed it up and optionally trim the intro)
	if (ma_sound_init_from_file(&g_engine, "assets/toin.mp3",
	                            MA_SOUND_FLAG_DECODE, NULL, NULL, &g_toin) == MA_SUCCESS) {
		ma_sound_set_pitch(&g_toin, TOIN_PITCH);
		g_toinLoaded = 1;
	} else {
		printf("[audio] could not load assets/toin.mp3\n");
	}

	printf("[audio] engine started\n");
}

void audioShutdown(void) {
	if (g_runningLoaded) {
		ma_sound_uninit(&g_running);
		g_runningLoaded = 0;
	}
	if (g_musicLoaded) {
		ma_sound_uninit(&g_music);
		g_musicLoaded = 0;
	}
	if (g_toinLoaded) {
		ma_sound_uninit(&g_toin);
		g_toinLoaded = 0;
	}
	if (g_engineOk) {
		ma_engine_uninit(&g_engine);
		g_engineOk = 0;
	}
}

void audioPlay(SoundId id) {
	if (!g_engineOk) {
		return;
	}
	// fire-and-forget: miniaudio decodes and mixes each one, allowing overlap
	if (id == SND_CRASH) {
		// crash: the wall hit (fire-and-forget) plus the "toin" sting on top.
		// the sting is sped up via pitch and can skip its slow intro via seek.
		ma_engine_play_sound(&g_engine, "assets/hitting-wall.mp3", NULL);
		if (g_toinLoaded) {
			ma_uint32 sampleRate = ma_engine_get_sample_rate(&g_engine);
			ma_uint64 skipFrames = (ma_uint64)((TOIN_SKIP_MS / 1000.0f) * sampleRate);
			ma_sound_stop(&g_toin);                          // retrigger from the top
			ma_sound_seek_to_pcm_frame(&g_toin, skipFrames); // trim the intro
			ma_sound_start(&g_toin);
		}
		return;
	}
	const char *path =
		id == SND_JUMP  ? "assets/boing.mp3" :
		                  "assets/yoink.mp3";   // SND_SLIDE
	ma_engine_play_sound(&g_engine, path, NULL);
}

void audioRunning(int on) {
	if (!g_engineOk || !g_runningLoaded) {
		return;
	}
	if (on && !g_runningOn) {
		ma_sound_start(&g_running);
		g_runningOn = 1;
	} else if (!on && g_runningOn) {
		ma_sound_stop(&g_running);
		g_runningOn = 0;
	}
}
