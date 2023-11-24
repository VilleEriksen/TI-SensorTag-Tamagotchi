#include "Board.h"

struct song {
    uint16_t *song;
    uint16_t length;
    uint32_t speed;
};

// Happy theme
static uint16_t HAPPY_THEME_NOTES[] = {
    523, 0,    784,  659, 392, 0,    784,  659, 523,  0,    784,  659, 392, 0,    784,  659,
    440, 784,  659,  554, 330, 784,  659,  554, 440,  880,  659,  554, 330, 1109, 659,  554,
    587, 0,    1047, 740, 440, 0,    1047, 740, 587,  0,    1047, 740, 440, 0,    1047, 740,
    698, 1245, 1047, 831, 523, 1245, 1047, 813, 698,  1397, 1047, 831, 523, 1245, 1047, 831
};

static struct song HAPPY_THEME = { &HAPPY_THEME_NOTES[0], 64, 125000 };

// Angry theme
static uint16_t ANGRY_THEME_NOTES[] = {
    1047, 0, 1047, 0,   523, 0, 784, 0,    784,  831, 784,  784, 392,  0,    932,  1109,
    1047, 0, 1047, 415, 392, 0, 0,   1245, 1397, 0,   1397, 523, 1109, 1397, 1109, 1109,
    1047, 0, 1047, 0,   523, 0, 784, 0,    784,  831, 784,  784, 392,  0,    932,  1109,
    1047, 0, 1047, 415, 392, 0, 0,   830,  830,  0,   830,  0,   784,  740,  698,  784
};

static struct song ANGRY_THEME = { &ANGRY_THEME_NOTES[0], 64, 175000 };

static uint16_t WARNING_BEEP_NOTES[] = {
    1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109, 1047, 1109,
};

static struct song WARNING_BEEP = { &WARNING_BEEP_NOTES[0], 112, 12500 };

static uint16_t MOVING_UP_STING_NOTES[] = {
    1047, 1397, 1568, 1568, 1568, 1568
};

static struct song MOVING_UP_STING = { &MOVING_UP_STING_NOTES[0], 6, 125000 };

static uint16_t SHAKING_STING_NOTES[] = {
    1047, 1245, 1397, 1245, 1047, 1245, 1480, 1047
};

static struct song SHAKING_STING = { &SHAKING_STING_NOTES[0], 8, 100000 };

static uint16_t PLAYING_STING_NOTES[] = {
    1568, 1568, 1568, 1568, 2093, 1976, 1760, 1568
};

static struct song PLAYING_STING = { &PLAYING_STING_NOTES[0], 8, 125000 };

static uint16_t PIPES_EFFECT_NOTES[] = {
    1047, 1109, 1175
};

static struct song PIPES_EFFECT = { &PIPES_EFFECT_NOTES[0], 3, 75000 };

static uint16_t GAME_LOSE_STRING_NOTES[] = {
    1568, 1397, 1109, 1109, 1109, 1109
};

static struct song GAME_LOSE_STING = { &GAME_LOSE_STRING_NOTES[0], 6, 125000 };

void musixFxn();
void initMusic();
void startMusic(struct song *songVar, bool loopMusicVar);
void stopMusic();
void toggleMusic();
void playHappyTheme();
void playAngryTheme();
void playWaningBeep();
void playMovingUpSting();
void playShakingSting();
void playPlayingString();
void playPipesEffect();
void playGameLoseSting();
