#pragma once

namespace Config
{
    // =====================================================
    // WINDOW
    // =====================================================

    constexpr float WINDOW_WIDTH = 1024.f;
    constexpr float WINDOW_HEIGHT = 768.f;

    // =====================================================
    // FIELD
    // =====================================================

    constexpr float GROUND_HEIGHT = 35.f;
    constexpr float GROUND_Y = WINDOW_HEIGHT - GROUND_HEIGHT;

    // =====================================================
    // PLAYER
    // =====================================================

    constexpr float PLAYER_WIDTH = 50.f;
    constexpr float PLAYER_HEIGHT = 70.f;

    constexpr float PLAYER_HALF_WIDTH = PLAYER_WIDTH / 2.f;
    constexpr float PLAYER_HALF_HEIGHT = PLAYER_HEIGHT / 2.f;

    constexpr float PLAYER_SPEED = 200.f;
    constexpr float PLAYER_JUMP_FORCE = -500.f;

    // =====================================================
    // PLAYER COLLISION
    // =====================================================

    constexpr float PLAYER_COLLISION_RADIUS = 35.f;

    // =====================================================
    // PLAYER ATTACK
    // =====================================================

    constexpr float PLAYER_BUMP_FORCE_X = 450.f;
    constexpr float PLAYER_BUMP_FORCE_Y = -250.f;

    constexpr float PLAYER_KICK_FORCE_X = 1100.f;
    constexpr float PLAYER_KICK_FORCE_Y = -550.f;

    // =====================================================
    // BALL
    // =====================================================

    constexpr float BALL_RADIUS = 15.f;

    constexpr float BALL_BOUNCE = 0.65f;

    // =====================================================
    // GOAL
    // =====================================================

    constexpr float GOAL_WIDTH = 40.f;
    constexpr float GOAL_HEIGHT = 150.f;

    // =====================================================
    // PHYSICS
    // =====================================================

    constexpr float GRAVITY = 1200.f;
	// =====================================================
	// TIMER
	// =====================================================
    constexpr float MATCH_SECONDS = 90.f;
}