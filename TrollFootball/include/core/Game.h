#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <ui/MainMenu.h>
#include <ui/PauseMenu.h>
#include <ui/GameOver.h>
#include <ui/WinScreen.h>
#include <ui/Timer.h>
#include "entities/Player.h"
#include "entities/Ball.h"
#include "entities/Ground.h"
#include "entities/Goal.h"
#include "entities/Wall.h"
#include "entities/AsteroidManager.h"
#include <optional>
#include "ai/BotController.h"
#include "ui/CharacterSelectMenu.h"
#include "ui/SettingsMenu.h"
#include "ui/RulesScreen.h"
#include <unordered_map>

enum class GameState
{
    MainMenu,
	CharacterSelect,
    SettingsMenu,
    RulesScreen,
    Playing,
    GameOver,
    WinScreen,
    PauseMenu
};

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float deltaTime);
    void render();

    void resetAfterGoal();

    void startNewMatch();

    // MOI: goi khi het gio, kiem tra va chuyen sang WinScreen
    void checkMatchEnd();

    sf::RenderWindow window;
    GameState m_currentState{ GameState::MainMenu };

    bool m_isVsBot{ false };
    bool m_pendingVsBot{ false };

    int m_selectingPlayer{ 1 };        //1 = đang chọn cho Player1, 2 = Player2
    CharacterOption m_p1Option;        //lưu tạm lựa chọn của Player1

    std::unique_ptr<MainMenu> mainMenu;
    std::unique_ptr<CharacterSelectMenu> characterSelectMenu;
    std::unique_ptr<SettingsMenu> settingsMenu;
    std::unique_ptr<RulesScreen> rulesScreen;
    // MOI: 3 man hinh UI con lai
    std::unique_ptr<PauseMenu> pauseMenu;
    std::unique_ptr<GameOver> gameOverScreen;
    std::unique_ptr<WinScreen> winScreen;

    // MOI: dong ho dem nguoc, dung de xac dinh luc nao tran dau ket thuc
    std::unique_ptr<Timer> timer;

    std::unique_ptr<AsteroidManager> asteroidManager;

    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    std::unique_ptr<BotController> botController;
    std::unique_ptr<Ball> ball;
    std::unique_ptr<Ground> ground;
    std::unique_ptr<Goal> leftGoal;
    std::unique_ptr<Goal> rightGoal;
    std::unique_ptr<Wall> leftWall;
    std::unique_ptr<Wall> rightWall;
    std::unique_ptr<Wall> topWall;

	std::unordered_map<GameState, std::vector<IScreen*>> m_screens;

    // =========================
    // SCORE
    // =========================

    int leftScore{ 0 };
    int rightScore{ 0 };

    std::optional<sf::Font> font;

    std::optional<sf::Text> leftScoreText;
    std::optional<sf::Text> rightScoreText;

    // MOI: anh nen luc dang thi dau (ve phia sau san/cau thu/bong)
    // std::optional vi sf::Sprite khong co constructor mac dinh (SFML 3), phai co Texture truoc
    sf::Texture m_pitchTexture;
    std::optional<sf::Sprite> m_pitchSprite;

    // GOAL celebration effect
    bool isGoalCelebrating{ false };
    float goalTimer{ 0.f };
    const float GOAL_DURATION{ 1.0f };
    std::optional<sf::Sprite> goalSprite;
};