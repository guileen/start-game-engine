#pragma once

#include <string>

class Game {
public:
    Game();
    explicit Game(std::string windowTitle);

    ~Game() = default;

    void Run();

protected:
    // virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    // virtual void Render() = 0;
    // virtual void Shutdown() = 0;

private:
    void initializeServices();
    void shutdownServices();

public:

private:
    std::string m_windowTitle;
    bool m_isRunning = true;
};

extern Game* CreateGame();