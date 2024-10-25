#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp> 
#include <iostream>
#include <vector>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

float windowWidth = sf::VideoMode::getDesktopMode().width;
float windowHeight = sf::VideoMode::getDesktopMode().height;


float paddleWidth = windowWidth / 7.5;
float paddleHeight = windowHeight / 25;
constexpr float paddleSpeed = 1000.0f;
float ballRadius = windowWidth / (85.71);
constexpr float ballSpeed = 500.0f;
constexpr int brickRows = 5;
constexpr int brickCols = 9;
float brickWidth = windowWidth / 10;
float brickHeight = windowHeight / 25;
float brickSpacing = windowWidth / 110;

struct Paddle {
    RectangleShape shape;

    Paddle(float x, float y, bool level) {
        shape.setPosition(x, y - 20);
        
        if (level == 0)
        {
            shape.setFillColor(Color::Green);
            shape.setSize({ paddleWidth, paddleHeight });
        }
        else
        {
            shape.setFillColor(Color::Blue);
            shape.setSize({ paddleWidth - 80, paddleHeight });
        }
        shape.setOrigin(paddleWidth / 2, paddleHeight / 2);
    }

    void update(Time dt, bool level) 
    {
        if (Keyboard::isKeyPressed(Keyboard::Left) && left() > 0)
        {
            if(level == 0)  
                shape.move(-paddleSpeed * dt.asSeconds(), 0);
            else
                shape.move(-(paddleSpeed + 200) * dt.asSeconds(), 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && right() < windowWidth)
        {
            if (level == 0)
                shape.move(paddleSpeed * dt.asSeconds(), 0);
            else
                shape.move((paddleSpeed + 200) * dt.asSeconds(), 0);
        }
    }

    float x() 
    { 
        return shape.getPosition().x; 
    }
    float y() 
    { 
        return shape.getPosition().y; 
    }
    float left() 
    { 
        return x() - paddleWidth / 2; 
    }
    float right() 
    { 
        return x() + paddleWidth / 2; 
    }
    float top() 
    { 
        return y() - paddleHeight / 2; 
    }
    float bottom() 
    { 
        return y() + paddleHeight / 2; 
    }
};

struct Ball {
    CircleShape shape;
    Vector2f velocity;
    int random;

    Ball(float x, float y, bool level) 
    {
        shape.setPosition(x, y);
        shape.setRadius(ballRadius);
        shape.setOrigin(ballRadius, ballRadius);
        random = rand() % 2;

        if (level == 0)
        {
            shape.setFillColor(Color::Red);
            if(random == 0)
                velocity = { -ballSpeed, -ballSpeed };
            else
                velocity = { ballSpeed, -ballSpeed };
        }
        else
        {
            shape.setFillColor(Color::Yellow);
            if(random == 0)
                velocity = { -(ballSpeed + 120), -(ballSpeed + 120) };
            else
                velocity = { (ballSpeed + 120), -(ballSpeed + 120) };
        }
        
    }

    void update(Time dt) {
        shape.move(velocity * dt.asSeconds());

        if (left() < 0 || right() > windowWidth)
            velocity.x = -velocity.x;
        if (top() < 0)
            velocity.y = -velocity.y;
    }



    float x()
    {
        return shape.getPosition().x;
    }
    float y()
    {
        return shape.getPosition().y;
    }
    float left()
    {
        return x() - ballRadius / 2;
    }
    float right()
    {
        return x() + ballRadius / 2;
    }
    float top()
    {
        return y() - ballRadius / 2;
    }
    float bottom()
    {
        return y() + ballRadius / 2;
    }
};

struct Brick 
{
    RectangleShape shape;

    Brick(float x, float y, bool color) 
    {
        shape.setPosition(x, y);
        shape.setSize({ brickWidth, brickHeight });
        if(color == 0)
            shape.setFillColor(Color::Yellow);
        else
            shape.setFillColor(Color::Magenta);
        shape.setOrigin(brickWidth / 2, brickHeight / 2);
    }

    bool isIntersecting(Ball& ball) 
    {
        return shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds());
    }

};


// function for bricks at level2
void bricksforLevel2(vector<Brick> &bricks2)
{
    
    for (int row = 0; row < brickRows; ++row) {
        for (int col = 0; col < brickCols; ++col) {
            float x = (col + 0.9f) * (brickWidth + brickSpacing) - 70;
            float y = (row + 0.9f) * (brickHeight + brickSpacing) + 50;

            bricks2.emplace_back(x, y, 1);

        }
    }
}

int main() {

    srand(time(nullptr));

    // video mode full screen
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Brick Breaker", Style::Close | Style::Resize | Style::Fullscreen);
    window.setFramerateLimit(60);

    // welcome screen ******************************************************
    Texture welcomebackground;
    if (!welcomebackground.loadFromFile("welcomeBG.png")) {
        std::cerr << "Failed to load background image\n";
        return 1;
    }
    Sprite welcomebackgroundSprite(welcomebackground);
    welcomebackgroundSprite.setScale(
        static_cast<float>(windowWidth) / welcomebackgroundSprite.getLocalBounds().width,
        static_cast<float>(windowHeight) / welcomebackgroundSprite.getLocalBounds().height
    );


    //openning sound
    SoundBuffer buffer;
    buffer.loadFromFile("Opening.mp3");
    Sound sound;
    sound.setBuffer(buffer);

    
    window.clear();
    window.draw(welcomebackgroundSprite);
    sound.play();
    window.display();
    // waiting ENTER to continue
    while (!Keyboard::isKeyPressed(Keyboard::Key::Enter));
    sound.stop();


    // LEVEL 1 message *******************************************************
    Texture level1;
    if (!level1.loadFromFile("Level1.png")) {
        std::cerr << "Failed to load background image\n";
        return 1;
    }
    Sprite level1Sprite(level1);
    level1Sprite.setScale(
        static_cast<float>(windowWidth) / level1Sprite.getLocalBounds().width,
        static_cast<float>(windowHeight) / level1Sprite.getLocalBounds().height
    );
    window.clear();
    window.draw(level1Sprite);
    window.display();
    Sleep(2500);


    // Loading Image for LEVEL 2  *********************************************************
    Texture level2;
    if (!level2.loadFromFile("Level2.png")) {
        std::cerr << "Failed to load background image\n";
        return 1;
    }
    Sprite level2Sprite(level2);
    level2Sprite.setScale(
        static_cast<float>(windowWidth) / level2Sprite.getLocalBounds().width,
        static_cast<float>(windowHeight) / level2Sprite.getLocalBounds().height
    );
    

    // Background Image for LEVEL1 game
    Texture backgroundTexture;
    
    if (!backgroundTexture.loadFromFile("background1.jpg")) {
        std::cerr << "Failed to load background image\n";
        return 1;
    }

    Sprite backgroundSprite(backgroundTexture);
    
    backgroundSprite.setScale(
        static_cast<float>(windowWidth) / backgroundSprite.getLocalBounds().width,
        static_cast<float>(windowHeight) / backgroundSprite.getLocalBounds().height
    );


    // Background Image for LEVEL2 game
    Texture newbackgroundTexture;
    if (!newbackgroundTexture.loadFromFile("Background2.jpg")) {
        std::cerr << "Failed to load background image\n";
    }

    Sprite newbackgroundSprite(newbackgroundTexture);
    newbackgroundSprite.setScale(
        static_cast<float>(windowWidth) / newbackgroundSprite.getLocalBounds().width,
        static_cast<float>(windowHeight) / newbackgroundSprite.getLocalBounds().height
    );


    //Paddle1 is for level 1 & Paddle2 is for level 2
    Paddle paddle1(windowWidth / 2, windowHeight - 50, 0);
    Paddle paddle2(windowWidth / 2, windowHeight - 50, 1);

    //Ball1 is for level 1 & ball2 is for level 2
    Ball ball1(windowWidth / 2, windowHeight / 2, 0);
    Ball ball2(windowWidth / 2, windowHeight / 2, 1);
    int score = 0;


    //Bricks For level1 
    vector<Brick> bricks;
    vector<Brick> bricks2;
    for (int row = 0; row < brickRows; ++row) {
        for (int col = 0; col < brickCols; ++col) {
            float x = (col + 0.9f) * (brickWidth + brickSpacing) - 100;
            float y = (row + 0.9f) * (brickHeight + brickSpacing) + 50;
            bricks.emplace_back(x, y, 0);
        }
    }

    Font font;
    if (!font.loadFromFile("font1.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // scoring text
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(60);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition((windowWidth)/2 - 110, (windowHeight/2));

    int a = 1;

    Clock clock;
    int timerSeconds = 4200; // Initial timer value

    // Timer text
    Text timerText;
    timerText.setFont(font);
    timerText.setCharacterSize(60);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition((windowWidth) / 2 - 100, (windowHeight / 2) + 100);
    int time = 60;


    // sound for new level
    SoundBuffer buff;
    buff.loadFromFile("NewlevelSound.WAV");
    Sound sound1;
    sound1.setBuffer(buff);
    sound1.play();

    // sound when tile is break 
    SoundBuffer buffer2;
    buffer2.loadFromFile("Tilebreak2.WAV");
    Sound sound2;
    sound2.setBuffer(buffer2);
    
    SoundBuffer buffer3;
    buffer3.loadFromFile("GameOver2.wav");
    Sound sound3;
    sound3.setBuffer(buffer3);

    int level = 1;
    bool flag = 1;


    // Game loop ----- Press ESCAPE to exit anytime
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            else if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
                window.close();
        }
        while (flag)
        {
            Time dt = seconds(1.0f / 60.0f);
            Time dt1 = clock.restart();
            float deltaTime = dt1.asSeconds();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
                else if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
                    window.close();
            }

            if (level == 1)
            {
                ball1.update(dt);
                paddle1.update(dt, 0);
            }
            else
            {
                ball2.update(dt);
                paddle2.update(dt, 1);
            }

            timerSeconds -= deltaTime;
            if (timerSeconds % 70 == 0)
            {
                time--;
            }


            if (time <= 40) {  // when timmer of 60sec ends next level will begin
                if (level == 1)
                {
                    window.clear();
                    time = 60;
                    timerSeconds = 4200;
                    level = 2;
                    bricksforLevel2(bricks2);
                    window.clear();
                    window.draw(level2Sprite);
                    window.display();
                    Sleep(2500);
                    
                    sound1.play();
                }
                else
                {
                    // Game over
                    flag = 0;
                }
            }

            int c = 0;
            if (level == 1)
            {
                // for level1
                c = 0;
                // checking for breaks and ball intersection
                for (auto& brick : bricks) {
                    if (brick.isIntersecting(ball1)) {
                        score = score + 10;
                        
                        // Remove brick
                        brick.shape.setPosition(-100, -100); // Move the brick off-screen
                        if (c == 0)
                        {
                            sound2.play();
                            ball1.velocity.y = -ball1.velocity.y;
                            c++;
                        }
                    }
                }

                if (ball1.bottom() > windowHeight) {
                    flag = 0;
                }
               
                if (paddle1.shape.getGlobalBounds().intersects(ball1.shape.getGlobalBounds())) { 
                    ball1.velocity.y = -ball1.velocity.y;
                }
            }
            else
            {
                // for level2
                c = 0;
                // checking for breaks and ball intersection
                for (auto& brick : bricks2) {
                    if (brick.isIntersecting(ball2)) {
                        score = score + 15;
                        
                        // Remove brick
                        brick.shape.setPosition(-100, -100); // Move the brick off-screen
                        if (c == 0)
                        {
                            sound2.play();
                            ball2.velocity.y = -ball2.velocity.y;
                            c++;
                        }
                    }
                }

                if (ball2.bottom() > windowHeight) {
                    // Game over
                    flag = 0;
                }
                if (paddle2.shape.getGlobalBounds().intersects(ball2.shape.getGlobalBounds())) {
                    ball2.velocity.y = -ball2.velocity.y;
                }
            }

            scoreText.setString("Score: " + std::to_string(score));
            timerText.setString("Time: " + std::to_string(time) + "s");

            window.clear();

            

            if (level == 1)
            {
                // for level1
                window.draw(backgroundSprite);
                window.draw(scoreText);
                window.draw(timerText);
                window.draw(paddle1.shape);
                window.draw(ball1.shape);
            }
            else
            {
                // for level2
                window.draw(newbackgroundSprite);
                window.draw(scoreText);
                window.draw(timerText);
                window.draw(paddle2.shape);
                window.draw(ball2.shape);
            }

            if (level == 1)
            {
                // for level1
                for (auto& brick : bricks) {
                    window.draw(brick.shape);
                }
            }
            else
            {
                // for level2
                for (auto& brick : bricks2) {
                    window.draw(brick.shape);
                }
            }

            window.display();
        }
        

        // GameOver code

        // background
        Texture ending;
        if (!ending.loadFromFile("gameoverBG.jpg")) {
            std::cerr << "Failed to load background image\n";
            return 1;
        }

        Sprite endingSprite(ending);

        endingSprite.setScale(
            static_cast<float>(windowWidth) / endingSprite.getLocalBounds().width,
            static_cast<float>(windowHeight) / endingSprite.getLocalBounds().height
        );
                
        window.clear();
        
        if (a == 1) // playes sound only once in a loop
        {
            sound3.play();
            a++;
        }

        // scoring
        Text endingScore;
        endingScore.setFont(font);
        endingScore.setCharacterSize(220);
        endingScore.setFillColor(sf::Color::Red);
        endingScore.setPosition((windowWidth) / 2 - 700, (windowHeight / 2) - 400);

        endingScore.setString("GAME OVER!");

        window.draw(endingSprite);
        window.draw(endingScore);
        

        endingScore.setCharacterSize(60);
        endingScore.setFillColor(sf::Color::White);
        endingScore.setPosition((windowWidth) / 2 - 220, (windowHeight / 2));
        endingScore.setString("Your Score is " + to_string(score) + ".");
        window.draw(endingScore);


        // FILLING
        // storing highest score in the file and check and update highest score
        int highestScore = 0;
        fstream file("score.txt", ios::in);
        if (file.is_open()) {
            file >> highestScore;
            file.close();
        }
        if (highestScore < score)
        {
            highestScore = score;
            fstream file("score.txt", ios::out);
            if (file.is_open()) {
                file << highestScore;
                file.close();
            }
        }

        endingScore.setPosition((windowWidth) / 2 - 270, (windowHeight / 2) + 100);
        endingScore.setString("Highest Score is " + to_string(highestScore) + ".");
        window.draw(endingScore);

        endingScore.setCharacterSize(40);
        endingScore.setFillColor(sf::Color::Green);
        endingScore.setPosition((windowWidth) / 2 - 260, (windowHeight / 2) + 600);
        endingScore.setString("Press ESCAPE to EXIT...");
        window.draw(endingScore);
       
        window.display();
    }
    return 0;
}
