#pragma once

namespace Config
{
    // =====================================================
    // WINDOW
    // =====================================================

    constexpr float WINDOW_WIDTH = 1480.f;
    constexpr float WINDOW_HEIGHT = 744.f;

    // =====================================================
    // FIELD
    // =====================================================

    constexpr float GROUND_HEIGHT = 140.f;
    constexpr float GROUND_Y = WINDOW_HEIGHT - GROUND_HEIGHT;

    // =====================================================
    // PLAYER
    // =====================================================

    constexpr float PLAYER_WIDTH = 50.f;
    constexpr float PLAYER_HEIGHT = 70.f;

    constexpr float PLAYER_HALF_WIDTH = PLAYER_WIDTH / 2.f;
    constexpr float PLAYER_HALF_HEIGHT = PLAYER_HEIGHT / 2.f;

    constexpr float PLAYER_SPEED = 200.f;
    constexpr float PLAYER_JUMP_FORCE = -615.f;
    constexpr float PLAYER_DEFAULT_SCALE = 0.18f;

    // =====================================================
    // PLAYER ACTIONS (KICK & DASH)
    // =====================================================

    constexpr float PLAYER_KICK_DURATION = 0.18f;
    constexpr float PLAYER_KICK_COOLDOWN = 0.20f;

    constexpr float PLAYER_DASH_SPEED = 600.f;
    constexpr float PLAYER_DASH_DURATION = 0.15f;
    constexpr float PLAYER_DASH_COOLDOWN = 0.5f;

    // =====================================================
    // STUN EFFECT & ANIMATION
    // =====================================================

    constexpr int STUN_FRAME_COUNT = 5;
    constexpr float STUN_FRAME_DURATION = 0.08f;
    constexpr float STUN_DEFAULT_DURATION = 0.5f;
    constexpr float STUN_HEAD_OFFSET_Y = -85.f;
    constexpr float STUN_SPRITE_SCALE = 0.5f;

    // =====================================================
    // TRAIL EFFECT
    // =====================================================

    constexpr float TRAIL_SPAWN_INTERVAL = 0.025f;
    constexpr float TRAIL_LIFETIME = 0.15f;

    // =====================================================
    // PLAYER COLLISION & ATTACK
    // =====================================================

    constexpr float PLAYER_COLLISION_RADIUS = 35.f;

    constexpr float PLAYER_BUMP_FORCE_X = 450.f;
    constexpr float PLAYER_BUMP_FORCE_Y = -250.f;

    constexpr float PLAYER_KICK_FORCE_X = 1100.f;
    constexpr float PLAYER_KICK_FORCE_Y = -550.f;

    // BODY HITBOX
    constexpr float WIDTH_BODY_HITBOX = 75.f;
    constexpr float HEIGHT_BODY_HITBOX = 160.f;

    // KICK HITBOX
    constexpr float WIDTH_KICK_HITBOX = 45.f;
    constexpr float HEIGHT_KICK_HITBOX = 40.f;

    // =====================================================
    // BALL
    // =====================================================

    constexpr float BALL_RADIUS = 15.f;
    constexpr float BALL_BOUNCE = 0.65f;
    constexpr float BALL_DEFAULT_SCALE = 0.08f;
    constexpr float BALL_START_Y = 200.f;
    constexpr float BALL_ROTATION_FACTOR = 0.3f;

    constexpr float BALL_AIR_DRAG_X = 0.998f;
    constexpr float BALL_AIR_DRAG_Y = 0.9995f;
    constexpr float BALL_BOUNCE_DAMPING = 0.92f;
    constexpr float BALL_GROUND_FRICTION = 0.985f;
    constexpr float BALL_VERTICAL_STOP_THRESHOLD = 30.f;
    constexpr float BALL_HORIZONTAL_STOP_THRESHOLD = 5.f;

    // =====================================================
    // ASTEROID
    // =====================================================

    constexpr float ASTEROID_SCALE = 4.4f;
    constexpr float ASTEROID_MIN_SPEED = 345.f;
    constexpr float ASTEROID_MAX_SPEED = 520.f;
    constexpr float ASTEROID_MAX_SPAWN_DELAY = 1.4f;
    constexpr int ASTEROID_MIN_COUNT = 5;
    constexpr int ASTEROID_MAX_COUNT = 8;
    constexpr float ASTEROID_STUN_DURATION = 1.0f;

    // =====================================================
    // AI / BOT CONTROLLER TUNING
    // =====================================================

    constexpr float AI_KICK_DISTANCE = 55.f;
    constexpr float AI_STOP_DISTANCE = 10.f;
    constexpr float AI_JUMP_HEIGHT = 70.f;
    constexpr float AI_JUMP_DISTANCE = 90.f;
    constexpr float AI_DEFEND_X_OFFSET = 220.f;
    constexpr float AI_DASH_DISTANCE = 150.f;
    constexpr float AI_PREDICT_TIME = 0.18f;
    constexpr float AI_STUCK_DISTANCE = 45.f;

    // =====================================================
    // GOAL & GAME CELEBRATION
    // =====================================================

    constexpr float GOAL_WIDTH = 40.f;
    constexpr float GOAL_HEIGHT = 190.f;
    constexpr float GOAL_VISUAL_HEIGHT = 260.f;
    constexpr float GOAL_SPRITE_OFFSET_Y = 62.f;

    constexpr float LEFT_GOAL_OFFSET_X = 0.f;
    constexpr float LEFT_GOAL_OFFSET_Y = 53.f;
    constexpr float LEFT_GOAL_ORIGIN_X = 213.f;

    constexpr float RIGHT_GOAL_OFFSET_X = 0.f;
    constexpr float RIGHT_GOAL_OFFSET_Y = 53.f;
    constexpr float RIGHT_GOAL_ORIGIN_X = 925.f;

    constexpr float GOAL_CELEBRATION_DURATION = 1.0f;

    // =====================================================
    // PHYSICS & TIMER
    // =====================================================

    constexpr float GRAVITY = 1200.f;
    constexpr float MATCH_SECONDS = 90.f;
    constexpr float WALL_THICKNESS = 10.f;
}
