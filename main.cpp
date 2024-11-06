#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include<vector>
#include <deque>
using namespace sf;
using namespace std;

class Snake {
private:
    Vector2f position;
    RectangleShape bodySegment;

public:
    Snake(Vector2f startposition) {
        bodySegment.setSize(Vector2f(20,20));
        bodySegment.setFillColor(Color::Green);
        bodySegment.setPosition(startposition);
        position = startposition;
    }
    Vector2f getPosition() {
        return position;
    }
    void setPosition(Vector2f newPosition) {
        position = newPosition;
    }
    RectangleShape getShape() {
        return bodySegment;
    }
    void moveSingleBodySegment() {
        bodySegment.setPosition(position);
    }


};


class Apple {
private:
    RectangleShape appearenceOfApple;
public:
    Apple() {
        Vector2f startPosition(120,20);
        appearenceOfApple.setSize(Vector2f(20,20));
        appearenceOfApple.setFillColor(Color::Red);
        appearenceOfApple.setPosition(startPosition);
    };

    void setPosition(Vector2f newPosition) {
        appearenceOfApple.setPosition(newPosition);


    }
    RectangleShape getAppeareanceOfApple() {
        return appearenceOfApple;
    };
};

class Border {
private:
    RectangleShape borderShape;
public:
    Border(Vector2f position, Vector2f size) {
        borderShape.setSize(size);
        borderShape.setFillColor(Color::Magenta);
        borderShape.setPosition(position);
    }
    RectangleShape getShape() {
        return borderShape;
    }
};

class Engine{
private:
    Vector2f window_size;
    RenderWindow window;
    const unsigned int FPS = 60;
    static const Time timeOfFrame;
    vector<Snake> snakeSegments;// body segments
    int moveDirection;
    int speed;
    deque<int> directionExecutionOrder;
    Apple apple;
    vector<Border> BorderSegments;
    Font gameFont;
    Text gameText;
    Text scoreText;
    int addAmountOfBodySegment;
    Time timeSincelastMove;
    int totalEatenApples;
    bool isGameOver = false;
public:
    enum Direction { UP, RIGHT,LEFT,DOWN};


void moveApple() {
    Vector2f applePossiblePosition = Vector2f(window_size.x /20 -2, window_size.y/20-2);
    Vector2f newApplePosition;
    bool validLocation = false;
    srand(time(nullptr));
    do {
        validLocation = false;
        newApplePosition.x = (float)(1+rand()/ ((RAND_MAX +1u)/ (int)applePossiblePosition.x)) * 20;
        newApplePosition.y = (float)(1+rand()/ ((RAND_MAX +1u)/ (int)applePossiblePosition.y)) * 20;
        for (Snake& s: snakeSegments) {
            if (s.getShape().getGlobalBounds().intersects(Rect<float>(newApplePosition.x,newApplePosition.y,20,20))) {
                validLocation = true;
                break;
            }
        }

        for (auto & b : BorderSegments) {
            if (b.getShape().getGlobalBounds().intersects(
                Rect<float>(newApplePosition.x, newApplePosition.y, 20, 20))) {
                validLocation = true;
                break;
                }
        }

    }while(validLocation);
        apple.setPosition(newApplePosition);
}

void setBorder() {
    BorderSegments.push_back(Border(Vector2f(0,0),Vector2f(window_size.x,20)));
    BorderSegments.push_back(Border(Vector2f(0,window_size.y -20),Vector2f(window_size.x,20)));
    BorderSegments.push_back(Border(Vector2f(0,0),Vector2f(20,window_size.y)));
    BorderSegments.push_back(Border(Vector2f(window_size.x -20,0),Vector2f(20,window_size.y)));


}




    Engine() {
        window_size = Vector2f(800,600);
        window.create(VideoMode(window_size.x, window_size.y),"Snake", Style::Default);
        window.setFramerateLimit(FPS);
    gameFont.loadFromFile("/Users/annanechytailenko/Desktop/Object orientired programming/setUpSFML/slant_regular.ttf");
    InrefaceText(&gameText, gameFont,"Snake", 25,Color::Blue);
    FloatRect boundOfText = gameText.getLocalBounds();
    gameText.setPosition(Vector2f(window_size.x/ 2 - boundOfText.width/2,-9));

    InrefaceText(&scoreText, gameFont,"Score: 0", 25,Color::Blue);
    scoreText.setPosition(Vector2f(15,-9));

    start();



    };

    void start() {
        speed = 2;
        moveDirection = Direction::RIGHT;
        addAmountOfBodySegment = 0;
        timeSincelastMove = Time::Zero;
        directionExecutionOrder.clear();
        totalEatenApples = 0;
        setBorder();
        newSnake();
        moveApple();
        isGameOver =  false;
    }


    void InrefaceText(Text* text, const Font& font, const String& textString, int size, Color color) {
        text->setFont(font);
        text->setString(textString);
        text->setCharacterSize(size);
        text->setFillColor(color);
    }

    void addDirection(int newDirection) {
        if (directionExecutionOrder.empty()) {
            directionExecutionOrder.emplace_back(newDirection);
        } else {
            if(directionExecutionOrder.back() != newDirection) {
                directionExecutionOrder.emplace_back(newDirection);
            }
        }
    }


 void updateDirection() {

  if (timeSincelastMove.asSeconds() >= seconds(1.f / float(speed)).asSeconds()) {
      Vector2f thisSectionPosition = snakeSegments[0].getPosition();
      Vector2f lastSectionPosition = thisSectionPosition;


      if (!directionExecutionOrder.empty()) {

          switch (moveDirection) {
              case Direction::UP:
                  if (directionExecutionOrder.front() != Direction::DOWN) {
                      moveDirection = directionExecutionOrder.front();
                  }
              break;
              case Direction::DOWN:
                  if (directionExecutionOrder.front() != Direction::UP) {
                      moveDirection = directionExecutionOrder.front();
                  }
              break;
              case Direction::LEFT:
                  if (directionExecutionOrder.front() != Direction::RIGHT) {
                      moveDirection = directionExecutionOrder.front();
                  }
              break;
              case Direction::RIGHT:
                  if (directionExecutionOrder.front() != Direction::LEFT) {
                      moveDirection = directionExecutionOrder.front();
                  }
              break;
          }
          directionExecutionOrder.pop_front();
      }

      if(addAmountOfBodySegment) {
          addBodySegment();
          addAmountOfBodySegment --;
      }




      switch (moveDirection) {
          case Direction::RIGHT:
              snakeSegments[0].setPosition(Vector2f(thisSectionPosition.x + 20, thisSectionPosition.y));
          break;
          case Direction::DOWN:
              snakeSegments[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y + 20));
          break;
          case Direction::LEFT:
              snakeSegments[0].setPosition(Vector2f(thisSectionPosition.x - 20, thisSectionPosition.y));
          break;
          case Direction::UP:
              snakeSegments[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y - 20));
          break;
      }


      for (int s = 1; s < snakeSegments.size(); s++) {
          thisSectionPosition = snakeSegments[s].getPosition();
          snakeSegments[s].setPosition(lastSectionPosition);
          lastSectionPosition = thisSectionPosition;
      }


      for (auto & s : snakeSegments) {
          s.moveSingleBodySegment();
      }
      if(snakeSegments[0].getShape().getGlobalBounds().intersects(apple.getAppeareanceOfApple().getGlobalBounds())) {
          addAmountOfBodySegment+=4;
          totalEatenApples +=1;
          scoreText.setString("Score: " + to_string(totalEatenApples));
          speed++;
          moveApple();
      }


      for (int s = 1; s < snakeSegments.size(); s++) {
          if (snakeSegments[0].getShape().getGlobalBounds().intersects(snakeSegments[s].getShape().getGlobalBounds())) {
              isGameOver = true;
          }
      }

      for (auto & b : BorderSegments) {
          if (snakeSegments[0].getShape().getGlobalBounds().intersects(b.getShape().getGlobalBounds())) {
              isGameOver = true;
          }
      }




    timeSincelastMove = Time::Zero;
  }
}
    void input() {
        Event event{};

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == Keyboard::Up) {
                    addDirection(Direction::UP);
                } else if (event.key.code == Keyboard::Down) {
                    addDirection(Direction::DOWN);
                } else if (event.key.code == Keyboard::Left) {
                    addDirection(Direction::LEFT);
                } else if (event.key.code == Keyboard::Right) {
                    addDirection(Direction::RIGHT);
                }
                else if(event.key.code == Keyboard::Enter) {
                    start();
                }
            }
        }
    }

    void drawToBoard() {
        window.clear(Color::Black);
        window.draw(apple.getAppeareanceOfApple());


        for (Border& b: BorderSegments) {
            window.draw(b.getShape());
        }


        for(Snake& s : snakeSegments) {
            window.draw(s.getShape());
        }
        if(isGameOver) {
            window.clear(sf::Color::Black);
            Text endText;
            InrefaceText(&endText, gameFont,"Game Over", 50,Color::Blue);
            FloatRect boundOfText = gameText.getLocalBounds();
            endText.setPosition(Vector2f(330 ,250));
            window.draw(endText);

        } else {
            window.draw(gameText);
            window.draw (scoreText);
        }


        window.display();
    };
    void run() {
        Clock timeForLoop;
        while (window.isOpen()) {
            Time deltaTime = timeForLoop.restart();

            if (isGameOver) {
                input();
                drawToBoard();
                sleep(milliseconds(2));
                continue;
            }
            timeSincelastMove += deltaTime;
            input();
            updateDirection();
            drawToBoard();

        }
    }
        void newSnake() {
           snakeSegments.clear() ;
            snakeSegments.emplace_back(Vector2f(100,100)); //start position
            snakeSegments.emplace_back(Vector2f(80,100));
            snakeSegments.emplace_back(Vector2f(60,100));

        }

        void addBodySegment() {
            Vector2f newBodySegmentPosition = snakeSegments[snakeSegments.size()-1].getPosition();
            snakeSegments.emplace_back(newBodySegmentPosition);
    }


};



const sf::Time  Engine::timeOfFrame = seconds(1.f/60.f);
int main() {
    Engine engine;
    engine.run();
    return 0;
}
