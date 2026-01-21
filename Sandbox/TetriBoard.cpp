#include "TetriBoard.h"

TetriBoard::TetriBoard()
{
    m_BackgroundTexture.reset(new Trail::Texture("Assets/Textures/tetris.png"));
    std::random_device rd;
    m_Rng = std::mt19937(rd());
    m_Shapes = {
        // I piece
        {"I", {
            {
                Grid4x4{{ {1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} }},
                Grid4x4{{ {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }}
            },
            {0.0f, 1.0f, 1.0f, 1.0f} // Cyan
        }},
        // O piece
        {"O", {
            {
                Grid4x4{{ {0,1,1,0}, {0,1,1,0},{0,0,0,0}, {0,0,0,0} }}
            },
            {1.0f, 1.0f, 0.0f, 1.0f} // Yellow
        }},
        // T piece
        {"T", {
            {   
                Grid4x4{{ {1,1,1,0}, {0,1,0,0}, {0,0,0,0}, {0,0,0,0} }},
                Grid4x4{{ {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}  }},
                Grid4x4{{ {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }},
                Grid4x4{{ {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }}
            },
            {0.5f, 0.0f, 0.5f, 1.0f} // Purple
        }},
        {"J", {
        {
            Grid4x4{{ {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }},
            Grid4x4{{ {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} }},
            Grid4x4{{ {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} }},
            Grid4x4{{ {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }},
            
            
        },
        {0.0f, 0.0f, 1.0f, 1.0f} // Blue
    }},
        // L piece
        {"L", {
            {
                Grid4x4{{ {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }},
                Grid4x4{{ {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }},
                Grid4x4{{ {1,1,1,0}, {1,0,0,0}, {0,0,0,0}, {0,0,0,0} }},
                Grid4x4{{ {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} }}
            },
            {1.0f, 0.5f, 0.0f, 1.0f} // Orange
        }},
        
        // S piece
        {"S", {
            {
                Grid4x4{{ {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} }},
                Grid4x4{{ {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }}
            },
            {0.0f, 1.0f, 0.0f, 1.0f} // Green
        }},
        // Z piece
        {"Z", {
            {
                Grid4x4{{ {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }},
                Grid4x4{{ {0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0} }}
            },
            {1.0f, 0.0f, 0.0f, 1.0f} // Red
        }}
    };
    const std::string keys[7] = { "I", "O", "T", "J", "L", "S", "Z" };
    std::string newKey = keys[rand() % 7];
    m_TetriminoBuffer[0].shape = m_Shapes[newKey];
    m_TetriminoBuffer[0].coords = { 14,22 };
    m_TetriminoBuffer[0].rotation = 0;
    m_TetriminoBuffer[0].key = newKey;

    newKey = keys[rand() % 7];
    m_TetriminoBuffer[1].shape = m_Shapes[newKey];
    m_TetriminoBuffer[1].coords = { 14,18 };
    m_TetriminoBuffer[1].rotation = 0;
    m_TetriminoBuffer[1].key = newKey;

    newKey = keys[rand() % 7];
    m_TetriminoBuffer[2].shape = m_Shapes[newKey];
    m_TetriminoBuffer[2].coords = { 14,14 };
    m_TetriminoBuffer[2].rotation = 0;
    m_TetriminoBuffer[2].key = newKey;

    newKey = keys[rand() % 7];
    m_TetriminoBuffer[3].shape = m_Shapes[newKey];
    m_TetriminoBuffer[3].coords = { 14,10 };
    m_TetriminoBuffer[3].rotation = 0;
    m_TetriminoBuffer[3].key = newKey;
    createTetrimino(getNextTetriminoKey());
}
glm::vec2 TetriBoard::gridToWorld(int gridX, int gridY) const {
    return {
        (m_BlockSize * -5) + gridX * m_BlockSize,
        m_StartY + gridY * m_BlockSize
    };
}

bool TetriBoard::checkCollision(int xDirection, int yDirection) {//to be used when moving
    Grid4x4 currentRotation = m_CurrentTetrimino.shape.rotations[m_CurrentTetrimino.rotation];

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (currentRotation[row][col] == 0) continue;

            int boardX = m_CurrentTetrimino.coords.x + col + xDirection;
            int boardY = m_CurrentTetrimino.coords.y + row + yDirection;

            // check bounds
            if (boardX < 0 || boardX >= 10 || boardY < 0 || boardY >= 24)
                return true;

            // check existing blocks
            if (m_Board[boardY][boardX] != 0)
                return true;
        }
    }
    return false;
}
int TetriBoard::getTetriminoValue(const std::string& key) {
    if (key == "I")      return 1;
    else if (key == "O") return 2;
    else if (key == "T") return 3;
    else if (key == "J") return 4;
    else if (key == "L") return 5;
    else if (key == "S") return 6;
    else if (key == "Z") return 7;
    return -1; //error
}
glm::vec4 TetriBoard::getTetriminoColor(const int keyValue) {
    if      (keyValue == 1) return { 0.0f, 1.0f, 1.0f, 1.0f };
    else if (keyValue == 2) return { 1.0f, 1.0f, 0.0f, 1.0f };
    else if (keyValue == 3) return { 0.5f, 0.0f, 0.5f, 1.0f };
    else if (keyValue == 4) return { 0.0f, 0.0f, 1.0f, 1.0f };
    else if (keyValue == 5) return { 1.0f, 0.5f, 0.0f, 1.0f };
    else if (keyValue == 6) return { 0.0f, 1.0f, 0.0f, 1.0f };
    else if (keyValue == 7) return { 1.0f, 0.0f, 0.0f, 1.0f };
    return { 1.0f, 1.0f, 1.0f, 1.0f };//error
}
void TetriBoard::placeBlocks() {
    Grid4x4 currentRotation = m_CurrentTetrimino.shape.rotations[m_CurrentTetrimino.rotation];
    const std::string keys[7] = { "I", "O", "T", "J", "L", "S", "Z" };
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (currentRotation[row][col] == 0) continue;

            int boardX = m_CurrentTetrimino.coords.x + col;
            int boardY = m_CurrentTetrimino.coords.y + row;
            
            m_Board[boardY][boardX] = getTetriminoValue(m_CurrentKey); //TODO: make color change depending on current tetrimino type
        }
    }

    checkBoard();
    createTetrimino(getNextTetriminoKey());
}

std::string TetriBoard::getNextTetriminoKey() {
    m_CurrentKey = m_TetriminoBuffer[0].key;
    for (int i = 0; i < 3; i++) {
        m_TetriminoBuffer[i].shape = m_TetriminoBuffer[i + 1].shape;
        m_TetriminoBuffer[i].key = m_TetriminoBuffer[i + 1].key;
    }
       
    const std::string keys[7] = { "I", "O", "T", "J", "L", "S", "Z" };
    std::uniform_int_distribution<int> dist(0, 6);
    std::string newKey = keys[dist(m_Rng)];
    m_TetriminoBuffer[3].shape = m_Shapes[newKey];
    m_TetriminoBuffer[3].key = newKey;
    return m_CurrentKey;
}

void TetriBoard::checkBoard() {
    for (int row = 0; row < 21; ++row) {
        int counter = 0;
        for (int col = 0; col < 10; ++col) {
            //if current row is full, take every row until the top and drop them one row
            if (m_Board[row][col] != 0)
                ++counter;
        }
        if (counter == 10) {
            m_Score += 10;
            for (int replaceRow = row; replaceRow < 20; ++replaceRow) {
                for (int replaceCol = 0; replaceCol < 10; ++replaceCol) {
                    m_Board[replaceRow][replaceCol] = m_Board[replaceRow + 1][replaceCol];
                }
            }
            for (int lastCol = 0; lastCol < 10; ++lastCol) {
                m_Board[20][lastCol] = 0;
            }
            --row;
        }
    }
}

void TetriBoard::createTetrimino(std::string& key) {
    m_CurrentTetrimino = {
        m_Shapes[key],
        {4,20},
        0,
        key
    };
    m_GravityTimer = 0.0f;
    bool collisionUnder = checkCollision(0, -1); // collision one row below
    if (m_CurrentTetrimino.coords.y >= 20 && collisionUnder) {
        TRL_CORE_WARN("Game Over triggered on spawn.  collisionUnder={0}", collisionUnder);
        m_GameOver = true;
    }
    else {
        m_GameOver = false; // not strictly necessary but explicit
    }
}


bool TetriBoard::checkLegalRotation(int rotation) {//to be used when rotating
    int rotationIndex = rotation % m_CurrentTetrimino.shape.rotations.size();
    const Grid4x4 checkedRotation = m_CurrentTetrimino.shape.rotations[rotationIndex];
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (checkedRotation[row][col] == 0) continue;

            int boardX = m_CurrentTetrimino.coords.x + col;
            int boardY = m_CurrentTetrimino.coords.y + row;

            // check bounds
            if (boardX < 0 || boardX >= 10 || boardY < 0 || boardY >= 24)
                return true;

            // check existing blocks
            if (m_Board[boardY][boardX] != 0)
                return true;
        }
    }
    return false;
}

void TetriBoard::getNextRotation() {
    int limitRotation = m_CurrentTetrimino.shape.rotations.size();
    int futureRotation;
    if (m_CurrentTetrimino.rotation+1 == limitRotation) {
        futureRotation = 0;
    }
    else {
        futureRotation = m_CurrentTetrimino.rotation+1;
    }
    if (!TetriBoard::checkLegalRotation(futureRotation))
        m_CurrentTetrimino.rotation = futureRotation;
    else
        TRL_CORE_INFO("Rotation not legal!");//play sound
}

void TetriBoard::renderCurrentTetrimino() {
    Grid4x4 currentRotation = m_CurrentTetrimino.shape.rotations[m_CurrentTetrimino.rotation];
    for (uint16_t row = 0; row < 4; ++row) {
        for (uint16_t col = 0; col < 4; ++col) {
            if (currentRotation[row][col] == 1) {
                glm::vec2 pos = gridToWorld(
                    m_CurrentTetrimino.coords.x + col,
                    m_CurrentTetrimino.coords.y + row
                );
                Trail::Renderer::DrawQuad(
                    { pos.x, pos.y, 0.0f},
                    { m_BlockSize, m_BlockSize },
                    m_CurrentTetrimino.shape.color
                );
            }
            for (const auto& tetrimino : m_TetriminoBuffer) {
                if (tetrimino.shape.rotations[tetrimino.rotation][row][col] == 1) {
                    glm::vec2 pos = {
                        (32 * -5)-20 + ((tetrimino.coords.x + col) * 32),
                        -500 + ((tetrimino.coords.y + row) * 32)
                    };
                    Trail::Renderer::DrawQuad(
                        { pos.x, pos.y, 0.0f },
                        { 32, 32 },
                        tetrimino.shape.color
                    );
                }
            }
        }
    }
}
void TetriBoard::renderBoard() { //TODO:: modify so it's just a single texture, no need to traverse every frame
    
    Trail::Renderer::RenderText(
        { -720.0f, -300.0f },
        std::to_string(m_Score),
        8,
        { 1.0f,1.0f,1.0f,1.0f }
    );
    for (int row = 0; row < 20; ++row) {
        for (int col = 0; col < 10; ++col) {
            glm::vec4 color = ((row + col) % 2 == 0)
                ? glm::vec4(0.6f, 0.6f, 0.6f, 0.2f)  
                : glm::vec4(0.4f, 0.4f, 0.4f, 0.2f); 

            if (m_Board[row][col] != 0) {
                color = getTetriminoColor(m_Board[row][col]);
            }
            if(row >=20) color = ((row + col) % 2 == 0)
                ? glm::vec4(0.2f, 0.0f, 0.2f, 1.0f)
                : glm::vec4(0.1f, 0.0f, 0.1f, 1.0f);

            Trail::Renderer::DrawQuad(
                { (m_BlockSize * -5) + (col * m_BlockSize),m_StartY+ (row * m_BlockSize), 0.0f },
                { m_BlockSize, m_BlockSize },
                color
            );
        }
    } 
    
}

void TetriBoard::OnUpdate(Trail::DeltaTime deltatime) {

    m_RotationCooldown -= deltatime;
    m_GravityTimer += deltatime;

    if (Trail::Input::isKeyPressed(TRL_KEY_LEFT) && !checkCollision(-1, 0) && m_RotationCooldown <= 0.0f) {
        m_CurrentTetrimino.coords.x--;
        if (checkCollision(-1, 0)) {
            TRL_CORE_INFO("collision detected on the left!");
            if (checkCollision(0, -1)) {
                placeBlocks();
                m_IsGrounded = false;
                m_WillCollide = false;
            }
        }
        m_RotationCooldown = 0.2f;
        m_LockTimer = 0.0f; // reset lock timer on move
    }

    if (Trail::Input::isKeyPressed(TRL_KEY_RIGHT) && !checkCollision(1, 0) && m_RotationCooldown <= 0.0f) {
        m_CurrentTetrimino.coords.x++;
        if (checkCollision(1, 0)) {
            TRL_CORE_INFO("collision detected on the right!");
            if (checkCollision(0, -1)) {
                placeBlocks();
                m_IsGrounded = false;
                m_WillCollide = false;
            }
        }
        m_RotationCooldown = 0.2f;
        m_LockTimer = 0.0f;
    }

    if (Trail::Input::isKeyPressed(TRL_KEY_R) && m_RotationCooldown <= 0.0f) {
        getNextRotation();
        m_RotationCooldown = 0.2f;
        m_LockTimer = 0.0f; 
    }
       //// Optional: make DOWN hold change the gravity interval (soft drop behavior)
    if (Trail::Input::isKeyPressed(TRL_KEY_DOWN)) m_IsSoftDropping = true;
    else m_IsSoftDropping = false;

    // choose interval (periodic only)
    float currentInterval = m_IsSoftDropping ? m_SoftDropInterval : m_GravityInterval;

    if (m_GravityTimer >= currentInterval) {
        // subtract instead of zero to keep stable timing if dt > interval
        m_GravityTimer -= currentInterval;

        if (!checkCollision(0, -1)) {
            m_CurrentTetrimino.coords.y--;
            // Since we moved down, reset grounded/lock state
            m_WillCollide = false;
            m_IsGrounded = false;
            m_LockTimer = 0.0f;
        }
        else {
            m_WillCollide = true;
        }
    }

    if (m_WillCollide) {
        if (!m_IsGrounded) {
            m_IsGrounded = true;
            m_LockTimer = 0.0f;
        }
        else {
            m_LockTimer += deltatime;
            if (m_LockTimer >= m_LockDelay) {
                placeBlocks();
                m_IsGrounded = false;
                m_WillCollide = false;
            }
        }
    }
    else {
        m_IsGrounded = false;
    }

    if (m_GameOver) {
        renderBoard();
        // render a "GAME OVER" text in the middle of the screen
        Trail::Renderer::RenderText(
            { 450.0f, 0.0f },
            "GAME OVER",
            61,
            { 1.0f, 0.0f, 0.0f, 1.0f }
        );
        return;
    }
    renderCurrentTetrimino();
    renderBoard();
}
void TetriBoard::OnEvent(Trail::Event& e) {
    Trail::EventDispatcher dispatcher(e);
    //dispatcher.Dispatch<Trail::MouseScrolledEvent>([this](Trail::MouseScrolledEvent& event)-> bool {return m_CameraController.Zoom(-0.01f); });
}
//TODO:: collisions DONE 
//TODO:: limitation du tableau DONE 
//TODO:: access aux rotations par le joueur DONE (//TODO: give acess to forward and backward rotation) 
//TODO:: access à la position + déplacements DONE (//TODO: give time before actually placing the block) 
//TODO:: élimination des lignes DONE (//TODO:: keep score) 
//TODO:: score DONE 
//TODO:: menu 
//TODO:: musique + sons 
//TODO:: ghost block and placing 
//TODO:: timer DONE 
//TODO:: make random buffer for the pieces DONE
//TODO:: put repetition limit to 2 in the buffer 
//TODO:: fix bug where pieces get out of bounds when rotating DONE
//TODO:: give leeway when placing
//TODO:: create a button for auto placing
//TODO:: implement ghost blocks
//TODO:: use a different layer for the menu
//TODO:: test the "event handling" feature
//TODO:: topo des fonctionnalités à faire figurer dans la documentation + exemple simple de documentation
//TODO:: (texture, son, input, event, text, layer)