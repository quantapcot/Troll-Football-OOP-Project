#include "core/Game.h"
#include "audio/AudioManager.h"

int main() {
    AudioManager::getInstance().loadAll();
    Game game;
    game.run();
    return 0;
}