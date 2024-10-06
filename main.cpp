#include <iostream>
using namespace std;


int HEIGHT;
int WIDTH;

class Figure;


class Board {
private:
    int width;
    int height;
    char** grid = nullptr;
    // vector<*Figure> vecOfFig;
    Board() {};
    //delete from here
    //to here
public:
    static Board& getInstance() {
        static Board instance;
        return instance;
        }
    void initialize(int w, int h) {
        if (grid == nullptr) {
            width = w;
            height = h;
            grid = new char*[height];
            for (int i = 0; i< height; i++) {
                grid[i] = new char[width];
                for ( int j = 0; j< width; j++) {
                    grid[i][j] = ' ';
                }
            }
        }
    }


    void printFrame (int width, int height) {
        for (int i = 0; i< height; i++) {
            for ( int j = 0; j< width; j++) {
                if (i  == 0) {
                    if (j== 0) {
                        cout<<"┏";
                    } else if (j+1 == width) {
                        cout<<"┓"<<endl;
                    }else{cout<< "━";}

                } else if (i == height -1) {
                    if (j==0) {
                        cout<< "┗";
                    }else if (j+1 == width) {
                        cout<<"┛";
                    }else{
                        cout<< "━";
                    }
                    if (j+1 == width) {
                        cout<<endl;
                    }
                } else if (j == 0) {
                    cout<< "┃";
                } else if (j == width -1) {
                    cout<<"┃"<<endl;
                } else {
                    cout<<" ";
                }


            }
        }
        //to here
    }
    int getHeight() {
        return height;
    }
    int getWidth() {
        return width;
    }

    char** GetGrid() {
        return grid;
    }

    void printBoard() {
        for (int k = 0; k < height; k++) {
            for (int g = 0; g < width; g++) {
                cout<< grid[k][g];
                if (g == width - 1) {
                    cout<< endl;
                }
            }
        }

    }
};




struct PairCoord {
    int coordX;
    int coordY;
    PairCoord(int x, int y) : coordX(x), coordY(y) {}
};

class Figure {
protected:
    PairCoord coordOrigin;

    int widthOfBoard;
    int heightOfBoard;
    char** grid;
public:
    Figure(int x, int y) : coordOrigin(x, y),
          widthOfBoard(Board::getInstance().getWidth()),
          heightOfBoard(Board::getInstance().getHeight()),
          grid(Board::getInstance().GetGrid()) {
          coordOrigin.coordY = heightOfBoard - coordOrigin.coordY - 1 ;

    };

    virtual void  printOnBoard() {
        cout<<"bla bla";
    }
}




;



    class Rectangle: public Figure {
    private:
        int width;
        int height;
        int biggestY;
        int biggestX;

    public:
        Rectangle( int x, int y, int width, int height): Figure(x,y),width(width),height(height) {
            biggestX = coordOrigin.coordX + width;
            biggestY = heightOfBoard - (-1* coordOrigin.coordY +  heightOfBoard  - 1 + height) - 1;
        };

        void addRecTOBoard()  {
            // draw vertical line
            for (int i = coordOrigin.coordY; i >= biggestY; i-- ) {
                grid[i][coordOrigin.coordX] = '*';
                grid[i][biggestX] = '*';
            }
            for (int j = coordOrigin.coordX; j <= biggestX; j++ ) {
                grid[coordOrigin.coordY][j] = '*';
                grid[biggestY][j] = '*';
            }
        }
    };

class Circle: public Figure {
private:
    int radius;
public:
    Circle(int x, int y, int radius): Figure(x,y),radius(radius){};
    void addCirclaToBoard() {
        int originX = coordOrigin.coordX;
        int originY = coordOrigin.coordY;

        if (radius == 1) {
            for (int y = originY - 1; y <= originY + 1; ++y) {
                for (int x = originX - 1; x <= originX + 1; ++x) {
                    if (std::abs(x - originX) + std::abs(y - originY) == 1) {
                        grid[y][x]= '*';
                    }
                }
            }
        }
        else {

            for (int y = originY - radius; y <= originY + radius; ++y) {
                for (int x = originX - radius; x <= originX + radius; ++x) {
                    double distance = sqrt((x - originX) * (x - originX) + (y - originY) * (y - originY));
                    if (distance > radius - 0.5 && distance < radius + 0.5) {
                        grid[y][x]= '*';

                    }
                }
            }
        }
    }
};
class Triangle: public Figure {
private:
    int heighOfTriangle;
    int smallestY;
public:
    Triangle(int x, int y, int heighOfTriangle): Figure(x,y),heighOfTriangle(heighOfTriangle) {
        smallestY = heightOfBoard - (-1* coordOrigin.coordY +  heightOfBoard  - 1 - heighOfTriangle) - 1 ;
    };
    void addTriangleToBoard() {
        int x = coordOrigin.coordX;
        int biggestX = coordOrigin.coordX + heighOfTriangle;


        for(int y = coordOrigin.coordY; y<= smallestY ;y++ ) {
            if (y == coordOrigin.coordY) {
                grid[y][x] = '*';
                x ++;
            } else {
                grid[y][x] = '*';
                grid[y][x- 2*(x- coordOrigin.coordX)] = '*';
                x ++;
                if (y == smallestY) {
                    for (int base = x - 2*(x - coordOrigin.coordX) + 1; base<x; base++ ){
                        grid[smallestY][base] = '*';
                    }
                }
            }

        }

    }
};






int main() {
    Board::getInstance().initialize(50,20);
    Board& board = Board::getInstance();
    board.printFrame(50,20);

    Rectangle rectangular(5,6,19,5);
    rectangular.addRecTOBoard();
    board.printBoard();
    cout<<"ghvjkjdshfkjdshfkldhnf"<<endl;
    Circle circle(6,6,3);
    circle.addCirclaToBoard();
    board.printBoard();
    cout<<"ghvjkjdshfkjdshfkldhnf"<<endl;
    Triangle triangle(34,15,5);
    triangle.addTriangleToBoard();
    board.printBoard();

    return 0;
}