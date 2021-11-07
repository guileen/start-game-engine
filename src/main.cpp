#include <engine/platform/entry_point.h>
#include <engine/platform/game.h>

class MyGame: public Game {
public:
    MyGame() : Game() {
        std::cout << "MyGame inited" << std::endl;
        MapInputToAction(InputKey::KEY_A, "strafe", -1.0f);
        MapInputToAction(InputKey::KEY_D, "strafe", 1.0f);
        MapInputToAction(InputKey::KEY_W, "move", 1.0f);
        MapInputToAction(InputKey::MOUSE_LEFT, "fire", 1.0f);

        RegisterAction("strafe", [](InputSource source, int sourceIndex, float value) {
            std::cout << "strafe: " << value << std::endl;
            return true;
        });
        RegisterAction("move", [](InputSource source, int sourceIndex, float value) {
            std::cout << "move: " << value << std::endl;
            return true;
        });
        RegisterAction("fire", [](InputSource source, int sourceIndex, float value) {
            std::cout << "fire: " << value << std::endl;
            return true;
        });
    }
protected:
    void Update(float deltaTime) override {
        // std::cout << "MyGame updated" << std::endl;
    }

    inline void MapInputToAction(InputKey key, std::string action, float scale) {
        ServiceLocator::GetInputManager()->MapInputToAction(key, InputAction {
            .actionName = action,
            .scale = scale
        });
    }
    
    inline void RegisterAction(std::string actionName, InputManager::ActionCallbackFunc callback) {
        ServiceLocator::GetInputManager()->RegisterActionCallback(actionName, InputManager::ActionCallback {
            .Ref = "MyGame",
            .Func = callback
        });
    }
};

Game* CreateGame() {
    return new MyGame();
}