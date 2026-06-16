#include "audio.h"

#include <stdio.h>

#include "miniaudio.h"   // declarations only; the implementation lives in vendor/

static ma_engine g_engine;
static ma_sound  g_running;
static int g_engineOk = 0;
static int g_runningLoaded = 0;
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

	printf("[audio] engine started\n");
}

void audioShutdown(void) {
	if (g_runningLoaded) {
		ma_sound_uninit(&g_running);
		g_runningLoaded = 0;
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
	const char *path =
		id == SND_JUMP  ? "assets/boing.mp3" :
		id == SND_SLIDE ? "assets/yoink.mp3" :
		                  "assets/hitting-wall.mp3";
	// fire-and-forget: miniaudio decodes and mixes it, allowing overlap
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
