#pragma once
#include "Trail.h"
#include <array>
#include <unordered_map>
#include <string>
#include <vector>
#include <random>
using Grid4x4 = std::array<std::array<int, 4>, 4>;
using Rotations = std::vector<Grid4x4>;
struct TetriminoShape {
    Rotations rotations;
    glm::vec4 color;
};
struct Tetrimino {
    TetriminoShape shape;
    glm::vec2 coords;
    uint32_t rotation;
    std::string key;
};
class TetriBoard {
public:
    TetriBoard();
    std::unordered_map<std::string, TetriminoShape> m_Shapes;
    uint32_t m_Board[24][10] = {};
    void OnEvent(Trail::Event& e);
    void OnUpdate(Trail::DeltaTime deltatime);
    void placeBlocks();
    void checkBoard();
    void getNextRotation();
    int getTetriminoValue(const std::string& key);
    glm::vec4 getTetriminoColor(const int key);
    std::string getNextTetriminoKey();
    bool checkCollision(int xDirection, int yDirection);
    bool checkLegalRotation(int rotation);
    glm::vec2 gridToWorld(int gridX, int gridY) const;
    Tetrimino m_CurrentTetrimino;
private:
    void createTetrimino(std::string& key);
    void renderCurrentTetrimino();
    void renderBoard();
    int m_BlockSize = 42.0f;
    int m_StartY = -410.0f;
    int m_NormalYDescendingSpeed = 100.0f;
    int m_BottomY = m_StartY - (m_BlockSize / 2);
    float m_RotationCooldown = 0.0f;
    int m_Score = 0;
    Tetrimino m_TetriminoBuffer[4];
    std::string m_CurrentKey;
    float m_LockDelay = 0.2f;       // Half a second delay (adjust as you want)
    float m_LockTimer = 0.0f;       // Time spent touching the ground
    bool  m_IsGrounded = false;     // Whether piece is touching ground
    bool m_WillCollide = false;
    float m_GravityTimer = 0.0f;   // accumulates delta time
    float m_GravityInterval = 0.5f;   // seconds between automatic drops (tweak this)
    float m_SoftDropInterval = 0.1f;  // optional: faster interval while holding DOWN
    bool  m_IsSoftDropping = false; // whether player holds soft drop
    bool m_GameOver = false;
    std::mt19937 m_Rng;
    std::shared_ptr<Trail::Texture> m_BackgroundTexture; 
};