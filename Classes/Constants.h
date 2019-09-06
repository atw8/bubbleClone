#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_


static const int MIN_SAME_COLOR_DESTRUCTION = 3;

static const float EVENT_WOOD1_BREAK_DELAY = 0.2f;
static const float EVENT_WOOD2_BREAK_DELAY = 0.2f;

static const float EVENT_ICE_BREAK_DELAY = 0.1f;


static const float EVENT_FISSION_DELAY = 0.0f;
static const float EVENT_FOG_DELAY = 0.0f;

static const float EVENT_WITCH_CHANGE_COLOR_DELAY = 0.2f;
static const float EVENT_WITCH_DESTROY_DELAY = 0.15f;
static const float EVENT_CHROME_EXPLOSION_DELAY = 0.0f;
static const float EVENT_NORMAL_EXPLOSION_DELAY = 0.1f;
static const float EVENT_FALL_DELAY = 0.05f;// 0.1f;
static const float EVENT_END2_DELAY = 0.5f;

static const float JITTER_PERIOD = 0.5f;
static const int JITTER_NORMAL_EXPLOSION_RADIUS = 5;
static const float JITTER_NORMAL_EXPLOSION_MAGNITUDE = 25.0f;


static const float MOVE_FIELD_VELOCITY_0 = 500.0f;
static const float MOVE_FIELD_VELOCITY_CONSTANT = 300.0f;



static const float SHOOT_BUBBLE_MIN_VELOCITY = 3000.0f;
static const float SHOOT_BUBBLE_MAX_VELOCITY = 4000.0f;

static const float SHOOT_BUBBLE_MAX_RADIUS = 1.0f;
static const float SHOOT_BUBBLE_MIN_RADIUS = 0.6f;

static const float PREDICTION_LINE_SPEED = 100.0f;


static const float PREDICTION_LINE_OFFSET_MULTIPLIER = 2.0f;
static const float PREDICTION_LINE_MIN_BUBBLE_RADIUS_MULTIPLIER = 0.3f;
static const float PREDICTION_LINE_MAX_BUBBLE_RADIUS_MULTIPLIER = 0.5f;

static const float END2_VELOCITY_ANGLE = 5.0f;
static const float END3_VELOCITY_ZERO = 100.0f;




//VIEW_RELATED_CONTANTS
static const float EVENT_NORMAL_EXPLOSION_SCALE_0_DELAY = 0.1f;

static const float GRAPHICS_SCALE_CONSTANT = 33.0f;

#endif //_CONSTANTS_H_