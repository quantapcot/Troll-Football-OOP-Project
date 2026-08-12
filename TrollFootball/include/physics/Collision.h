#pragma once

class Player;
class Ball;
class Wall;
struct Asteroid;


class Collision
{
public:

    static void handlePlayerBall(Player& player, Ball& ball);

    // Xử lý chung bóng với cả 2 player để tránh lỗi tunneling/kẹt bóng
    static void handlePlayersBall(Player& p1, Player& p2, Ball& ball);

    static void handleKick(Player& player, Ball& ball);

    static void handlePlayerPlayer(Player& p1, Player& p2);

    static void handleBallWall(Ball& ball, const Wall& wall);

    static void handlePlayerAsteroid(Player& player, Asteroid& asteroid);
};