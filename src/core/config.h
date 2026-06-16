#ifndef CONFIG_H
#define CONFIG_H

// ----- Lanes (X axis) -----
#define NUM_LANES 3
#define LANE_WIDTH 2.0f
// converts a lane index (0=left, 1=middle, 2=right) into the X of its center
#define laneToX(lane) (((lane) - 1) * LANE_WIDTH)

// ----- Player / jump / crouch -----
#define EYE_HEIGHT 1.6f             // camera height while standing
#define EYE_HEIGHT_DUCK 0.8f        // camera height while crouching
#define GRAVITY 0.018f
#define JUMP_SPEED 0.32f
#define JUMP_CLEARANCE 1.1f         // min player height to clear a low obstacle
#define LANE_LERP 0.25f             // lateral move smoothing
#define CROUCH_DOWN_RATE 0.45f      // how fast the slide goes down
#define CROUCH_UP_RATE 0.20f        // how fast it stands back up
#define LOOK_LEGS_RATE 0.25f        // smoothing of the "look at legs" camera dip

// ----- Corridor speed (obstacles move toward the camera) -----
#define INITIAL_SPEED 0.18f
#define MAX_SPEED 0.55f
#define SPEED_GAIN_PER_POINT 0.01f  // how much speed rises per point

// ----- Duck (slide) -----
#define MS_PER_FRAME 16.0f          // game timer interval
#define MAX_DUCK_TIME 500.0f        // max time ducking before standing up (ms)

// ----- Game over -----
#define GAME_OVER_DELAY 500.0f     // delay (ms) between the crash and the game over screen

// ----- World -----
#define SPAWN_DIST -70.0f           // Z where obstacles are born (far away)
#define Z_REMOVE 4.0f               // Z where they are removed (behind the camera)
#define Z_PLAYER 2.5f               // Z of the player (collides when the block reaches the face)
#define COLLISION_WINDOW 0.01f      // half-width in Z of the player's collision band (swept)
#define MAX_OBSTACLES 32

// ----- Camera / sprites tuning -----
#define CAM_DIP_SLIDE 0.40f         // camera looks down during the slide
#define CAM_DIP_JUMP 0.9f           // camera looks down (at the legs) during the jump
#define LEGS_JUMP_DROP 0.22f        // how far the jump legs sprite drops (fraction of screen height)
#define FONT_REF_HEIGHT 756.0f      // reference window height for font scaling

// ----- Action popup (comic-style sprite on jump/slide) -----
#define POPUP_MS 450.0f             // how long the popup stays on screen (ms)
#define POPUP_FRAC 0.20f            // popup size as a fraction of the smaller screen dimension

#endif
