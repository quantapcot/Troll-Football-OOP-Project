#include <iostream>
#include <stdexcept>
#include "core/Game.h"
#include "core/GameConfig.h"
#include "physics/Collision.h"
#include <cmath>
#include <audio/AudioManager.h>

// MOI: so giay 1 tran dau - doi so nay theo y ban, hoac chuyen vao GameConfig.h neu muon
static constexpr float MATCH_SECONDS = 90.f;

Game::Game()
    : window(
        sf::VideoMode(
            {
                static_cast<unsigned>(Config::WINDOW_WIDTH),
                static_cast<unsigned>(Config::WINDOW_HEIGHT)
            }),
        "TrollFootball")
{
    window.setFramerateLimit(60);

    // =========================
    // LOAD FONT
    // =========================
    font.emplace();
    if (!font->openFromFile("assets/fonts/minecraft.ttf"))
    {
        throw std::runtime_error("Khong the load font: assets/fonts/minecraft.ttf");
    }

    // =========================
    // SCORE TEXT
    // =========================
    scoreText.emplace(*font);
    scoreText->setCharacterSize(28);
    scoreText->setFillColor(sf::Color::White);
    scoreText->setPosition({ 20.f, 20.f });

    // =========================
    // MAIN MENU (da co tu truoc)
    // =========================
    mainMenu = std::make_unique<MainMenu>(*font, window.getSize());

    // =========================
    // MOI: 3 MAN HINH UI CON LAI + TIMER
    // Tat ca deu can (const sf::Font&, sf::Vector2u), tao SAU khi font da load xong
    // =========================
    pauseMenu = std::make_unique<PauseMenu>(*font, window.getSize());
    gameOverScreen = std::make_unique<GameOver>(*font, window.getSize());
    winScreen = std::make_unique<WinScreen>(*font, window.getSize());
    timer = std::make_unique<Timer>(*font, MATCH_SECONDS);

    AudioManager::getInstance().loadAll();

    // =========================
    // PLAYER 1
    // =========================

    ControlScheme p1Controls{
        sf::Keyboard::Key::A,
        sf::Keyboard::Key::D,
        sf::Keyboard::Key::W,
        sf::Keyboard::Key::J
    };

    // =========================
    // PLAYER 2
    // =========================

    ControlScheme p2Controls{
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::Numpad0
    };

    player1 = std::make_unique<Player>(p1Controls, sf::Color::Red);
    player2 = std::make_unique<Player>(p2Controls, sf::Color::Blue);

    player1->reset({
        150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    player2->reset({
        Config::WINDOW_WIDTH - 150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    ball = std::make_unique<Ball>();
    ground = std::make_unique<Ground>();

    leftGoal = std::make_unique<Goal>(0.f);

    rightGoal = std::make_unique<Goal>(
        Config::WINDOW_WIDTH - Config::GOAL_WIDTH);

    AudioManager::getInstance().playMusic("menu");
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        // MOI: bam ESC trong luc dang thi dau => mo PauseMenu
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::Escape
                && m_currentState == GameState::Playing)
            {
                m_currentState = GameState::PauseMenu;
                timer->pause();
            }
        }

        // SUA: mo rong switch, moi state co UI rieng xu ly event cua no
        switch (m_currentState)
        {
        case GameState::MainMenu:
        {
            MainMenuAction action = mainMenu->handleEvent(*event, window);

            if (action == MainMenuAction::Play)
            {
                startNewMatch();
                m_currentState = GameState::Playing;
            }
            else if (action == MainMenuAction::Exit)
            {
                window.close();
            }
            break;
        }

        // MOI: xu ly PauseMenu
        case GameState::PauseMenu:
        {
            PauseMenuAction action = pauseMenu->handleEvent(*event, window);

            if (action == PauseMenuAction::Resume)
            {
                timer->resume();
                m_currentState = GameState::Playing;
            }
            else if (action == PauseMenuAction::Restart)
            {
                startNewMatch();
                m_currentState = GameState::Playing;
            }
            else if (action == PauseMenuAction::MainMenu)
            {
                AudioManager::getInstance().stopMusic();
                AudioManager::getInstance().playMusic("menu");
                m_currentState = GameState::MainMenu;
            }
            break;
        }

        // MOI: xu ly WinScreen
        case GameState::WinScreen:
        {
            WinScreenAction action = winScreen->handleEvent(*event, window);

            if (action == WinScreenAction::Restart)
            {
                startNewMatch();
                m_currentState = GameState::Playing;
            }
            else if (action == WinScreenAction::MainMenu)
            {
                AudioManager::getInstance().stopMusic();
                AudioManager::getInstance().playMusic("menu");
                m_currentState = GameState::MainMenu;
            }
            break;
        }

        // MOI: xu ly GameOver (hien khong co gi tu dong kich hoat man hinh nay,
        // nhung wire san logic bam nut de dung khi ban them dieu kien trigger sau nay)
        case GameState::GameOver:
        {
            GameOverAction action = gameOverScreen->handleEvent(*event, window);

            if (action == GameOverAction::Restart)
            {
                startNewMatch();
                m_currentState = GameState::Playing;
            }
            else if (action == GameOverAction::MainMenu)
            {
                AudioManager::getInstance().stopMusic();
                AudioManager::getInstance().playMusic("menu");
                m_currentState = GameState::MainMenu;
            }
            break;
        }

        default:
            break; // Playing: ESC da xu ly rieng o tren, khong can gi them
        }
    }
}

void Game::update(float deltaTime)
{
    if (m_currentState != GameState::Playing)
        return;

    timer->update(deltaTime); // MOI: dem nguoc dong ho

    player1->update(deltaTime);
    player2->update(deltaTime);

    ball->update(deltaTime);
    ground->update(deltaTime);

    leftGoal->update(deltaTime);
    rightGoal->update(deltaTime);

    // =========================
    // COLLISION PLAYER 1
    // =========================

    Collision::handlePlayerBall(*player1, *ball);
    Collision::handleKick(*player1, *ball);

    // =========================
    // COLLISION PLAYER 2
    // =========================

    Collision::handlePlayerBall(*player2, *ball);
    Collision::handleKick(*player2, *ball);

    // =========================
    // COLLISION PLAYER 1 VS PLAYER 2
    // =========================

    Collision::handlePlayerPlayer(*player1, *player2);

    // =========================
    // GOAL
    // =========================

    if (leftGoal->contains(*ball))
    {
        std::cout << "Right Player Scores!\n";

        rightScore++;
        scoreText->setString(
            std::to_string(leftScore) + "  -  " + std::to_string(rightScore));

        AudioManager::getInstance().playSound("goal");

        resetAfterGoal();
        return;
    }

    if (rightGoal->contains(*ball))
    {
        std::cout << "Left Player Scores!\n";

        leftScore++;
        scoreText->setString(
            std::to_string(leftScore) + "  -  " + std::to_string(rightScore));

        AudioManager::getInstance().playSound("goal");

        resetAfterGoal();
        return;
    }

    // MOI: kiem tra het gio sau moi frame
    checkMatchEnd();
}

void Game::checkMatchEnd()
{
    if (!timer->isTimeUp())
        return; // Chua het gio thi khong lam gi ca

    AudioManager::getInstance().stopMusic();

    winScreen->setFinalScore(leftScore, rightScore);
    AudioManager::getInstance().playMusic("victory");
    m_currentState = GameState::WinScreen;
}

void Game::render()
{
    window.clear(sf::Color(30, 120, 30));

    // SUA: gom dieu kien - ve san/cau thu/bong khi KHONG o MainMenu
    // (Playing, PauseMenu, GameOver, WinScreen deu can san lam nen phia sau lop UI phu)
    if (m_currentState != GameState::MainMenu)
    {
        ground->render(window);

        leftGoal->render(window);
        rightGoal->render(window);

        ball->render(window);

        player1->render(window);
        player2->render(window);

        window.draw(*scoreText);
        timer->draw(window); // MOI: ve dong ho dem nguoc
    }

    // SUA: them cac case moi de ve lop UI phu tuong ung, DE SAU CUNG de nam tren cung
    switch (m_currentState)
    {
    case GameState::MainMenu:
        mainMenu->draw(window);
        break;
    case GameState::PauseMenu:
        pauseMenu->draw(window);
        break;
    case GameState::GameOver:
        gameOverScreen->draw(window);
        break;
    case GameState::WinScreen:
        winScreen->draw(window);
        break;
    default:
        break; // Playing: khong co lop phu nao them
    }

    window.display();
}

void Game::resetAfterGoal()
{
    player1->reset({
        150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    player2->reset({
        Config::WINDOW_WIDTH - 150.f,
        Config::GROUND_Y - Config::PLAYER_HALF_HEIGHT
        });

    ball->reset({
        Config::WINDOW_WIDTH / 2.f,
        200.f
        });
}

void Game::startNewMatch()
{
    leftScore = 0;
    rightScore = 0;
    scoreText->setString("0  -  0");

    timer->reset(); // MOI: reset dong ho ve MATCH_SECONDS moi khi bat dau tran

    resetAfterGoal();

    AudioManager::getInstance().stopMusic();
    AudioManager::getInstance().playMusic("match");
}