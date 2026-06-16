#ifndef AUDIO_H
#define AUDIO_H

// Semantic audio events — the rest of the game asks for these, not for files,
// so the underlying library stays hidden behind this interface.
typedef enum {
	SND_JUMP,    // boing
	SND_SLIDE,   // yoink
	SND_CRASH    // hitting the wall
} SoundId;

// Starts the audio engine and preloads the looping run sound. Safe to call once.
void audioInit(void);

// Stops the engine and frees resources.
void audioShutdown(void);

// Plays a one-shot sound effect (mixed, can overlap).
void audioPlay(SoundId id);

// Turns the looping running sound on/off (no-op if already in that state).
void audioRunning(int on);

#endif
