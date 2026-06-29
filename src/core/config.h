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
#define SPEED_GAIN_PER_DIST 0.0006f // how much speed rises per unit of distance traveled

// ----- Frame timing (fixed timestep) -----
#define MS_PER_FRAME 16.0f          // fixed simulation step (one gameUpdate)
#define MAX_FRAME_MS 250.0f         // cap on accumulated time per tick (anti spiral-of-death)

// ----- Duck (slide) -----
#define MAX_DUCK_TIME 500.0f        // max time ducking before standing up (ms)

// ----- Scoring -----
#define SCORE_PASS 1                // points for overtaking an obstacle in another lane
#define SCORE_BONUS 5               // points for clearing one in your lane (jumped over / ducked under)
#define BONUS_POPUP_MS 700.0f       // how long the "BONUS +5" text stays on screen (ms)

// ----- Game over -----
#define GAME_OVER_DELAY 500.0f     // delay (ms) between the crash and the game over screen

// ----- Crash "toin" sting (played on top of hitting-wall) -----
#define TOIN_PITCH 1.6f             // playback speed (>1 = faster & shorter)
#define TOIN_SKIP_MS 0.0f           // trim this many ms from the start of the clip

// ----- World -----
#define SPAWN_DIST -70.0f           // Z where obstacles are born (far away)
#define Z_REMOVE 4.0f               // Z where they are removed (behind the camera)
#define Z_PLAYER 2.5f               // Z of the player (collides when the block reaches the face)
#define COLLISION_WINDOW 0.01f      // half-width in Z of the player's collision band (swept)
#define MAX_OBSTACLES 32

// ----- Camera / sprites tuning -----
#define CAM_DIP_SLIDE 0.40f         // camera looks down during the slide
#define CAM_DIP_JUMP 0.9f           // camera looks down (at the legs) during the jump
#define CAM_ROLL_SLIDE 0.14f        // sideways camera roll during the slide (~8°)
#define LANE_ROLL 0.10f             // camera banks into a lane change (rad per world unit of lateral offset)
#define LANE_LOOK 0.09f             // camera looks slightly toward the lane it is moving into
#define LEGS_JUMP_DROP 0.22f        // how far the jump legs sprite drops (fraction of screen height)
#define ARMS_DROP_SLIDE 60.0f       // how far the arms sprite drops during the slide (px)
#define ARMS_DROP_JUMP 60.0f        // how far the arms sprite drops during the jump (px)
#define FONT_REF_HEIGHT 756.0f      // reference window height for font scaling

// ----- Action popup (comic-style sprite on jump/slide) -----
#define POPUP_MS 450.0f             // how long the popup stays on screen (ms)
#define POPUP_FRAC 0.20f            // popup size as a fraction of the smaller screen dimension

#endif
