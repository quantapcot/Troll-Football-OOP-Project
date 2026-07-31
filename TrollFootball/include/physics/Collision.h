#pragma once

class Player;
class Ball;

class Collision
{
public:

    static void handlePlayerBall(Player& player, Ball& ball);

    static void handleKick(Player& player, Ball& ball);

    static void handlePlayerPlayer(Player& p1, Player& p2);
};