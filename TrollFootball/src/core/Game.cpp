#include <iostream>
#include <stdexcept>
#include "core/Game.h"
#include "core/GameConfig.h"
#include "physics/Collision.h"
#include <cmath>
#include <audio/AudioManager.h>
#include "core/AssetManager.h"


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
    // LOAD TEXTURES
    // =========================
    AssetManager::get().loadTexture(
        "player1",
        "assets/textures/players/player1.png");

    AssetManager::get().loadTexture(
        "player2",
        "assets/textures/players/player2.png");

    AssetManager::get().loadTexture(
        "ball",
        "assets/textures/balls/ball.png");

    AssetManager::get().loadTexture(
        "stadium",
        "assets/textures/stadium/background.png"
    );
    auto& tex = AssetManager::get().getTexture("stadium");

    std::cout << "Background: "
        << tex.getSize().x
        << " x "
        << tex.getSize().y
        << std::endl;


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
    leftScoreText.emplace(*font);
    rightScoreText.emplace(*font);

    leftScoreText->setCharacterSize(30);
    rightScoreText->setCharacterSize(30);

    leftScoreText->setFillColor(sf::Color::White);
    rightScoreText->setFillColor(sf::Color::White);

    leftScoreText->setString("0");
    rightScoreText->setString("0");

    leftScoreText->setPosition({ 675.f, 127.f });
    rightScoreText->setPosition({ 785.f, 127.f });

    // =========================
    // MAIN MENU (da co tu truoc)
    // MOI: truyen them duong dan anh nen cho MainMenu - sua ten file neu ban dat ten khac
    // =========================
    mainMenu = std::make_unique<MainMenu>(
        *font,
        window.getSize(),
        "assets/textures/ui/menu.jpg");

    // =========================
    // MOI: 3 MAN HINH UI CON LAI + TIMER
    // Tat ca deu can (const sf::Font&, sf::Vector2u), tao SAU khi font da load xong
    // =========================
    pauseMenu = std::make_unique<PauseMenu>(*font, window.getSize());
    gameOverScreen = std::make_unique<GameOver>(*font, window.getSize());
    winScreen = std::make_unique<WinScreen>(*font, window.getSize());
    timer = std::make_unique<Timer>(*font, Config::MATCH_SECONDS);


    // =========================
    // PLAYER 1
    // =========================

    ControlScheme p1Controls{
        sf::Keyboard::Key::A,
        sf::Keyboard::Key::D,
        sf::Keyboard::Key::W,
        sf::Keyboard::Key::J,
        sf::Keyboard::Key::C
    };

    // =========================
    // PLAYER 2
    // =========================

    ControlScheme p2Controls{
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::Numpad0,
        sf::Keyboard::Key::Numpad1
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

        //EVENT STATE
        switch (m_currentState)
        {
        case GameState::MainMenu:
        {
            MainMenuAction action = mainMenu->handleEvent(*event, window);

            //MAIN MENU ACTION
            if (action == MainMenuAction::PlayVsPlayer)
            {
                m_isVsBot = false;

                player2->setAIControlled(false);

                botController.reset();

                startNewMatch();
                m_currentState = GameState::Playing;
            }
            else if (action == MainMenuAction::PlayVsBot)
            {
                m_isVsBot = true;

                player2->setAIControlled(true);

                botController = std::make_unique<BotController>(
                    *player2,
                    *ball
                );

                startNewMatch();

                m_currentState = GameState::Playing;

				

            }
            else if (action == MainMenuAction::Exit)
            {
                window.close();
            }
            break;
        }

		// PAUSE MENU
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

        // WINSCREEN
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

    if (m_isVsBot)
    {
        botController->update(deltaTime);
    }

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
        rightScore++;

        rightScoreText->setString(std::to_string(rightScore));

        AudioManager::getInstance().playSound("goal");

        resetAfterGoal();
        return;
    }

    if (rightGoal->contains(*ball))
    {
        leftScore++;
        
        leftScoreText->setString(std::to_string(leftScore));

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

    // MOI: xac dinh ten nguoi thang cu the dua tren ti so 2 ben
    // (Player 1 dieu khien phia trai/leftScore, Player 2 dieu khien phia phai/rightScore,
    // khop voi ControlScheme p1Controls/p2Controls va vi tri reset({150.f, ...}) o tren)
    std::string winnerLabel;
    if (leftScore > rightScore)
        winnerLabel = "PLAYER 1 WIN!";
    else if (rightScore > leftScore)
        winnerLabel = "PLAYER 2 WIN!";
    else
        winnerLabel = "DRAW!";

    winScreen->setWinnerLabel(winnerLabel);
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
        // MOI: ve anh nen san TRUOC TIEN, moi thu con lai (san, khung thanh, cau thu, bong)
        // se duoc ve DE LEN TREN anh nen nay
        sf::Sprite stadium(
            AssetManager::get().getTexture("stadium")
        );

        stadium.setPosition({ 0.f, 0.f });
        stadium.setScale({ 1.f, 1.f });

        window.draw(stadium);

        ground->render(window);

        leftGoal->render(window);
        rightGoal->render(window);

        ball->render(window);

        player1->render(window);
        player2->render(window);

        window.draw(*leftScoreText);
        window.draw(*rightScoreText);

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

    leftScoreText->setString("0");
    rightScoreText->setString("0");

    timer->reset(); // MOI: reset dong ho ve MATCH_SECONDS moi khi bat dau tran

    resetAfterGoal();

    AudioManager::getInstance().stopMusic();
    AudioManager::getInstance().playMusic("match");
}