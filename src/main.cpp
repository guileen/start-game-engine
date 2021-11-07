#include <engine/platform/entry_point.h>
#include <engine/platform/game.h>

class MyGame: public Game {
public:
    MyGame() : Game() {
        std::cout << "MyGame inited" << std::endl;
    }
protected:
    void Update(float deltaTime) override {
        // std::cout << "MyGame updated" << std::endl;
    }
};

Game* CreateGame() {
    return new MyGame();
}