#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include<regex>
#include <set>
using namespace std;




struct PairCoord {
    int coordX = 0;
    int coordY = 0;
    PairCoord(int x, int y) : coordX(x), coordY(y) {}
};

class Figure {
protected:
    enum class AvailableColor {
        PURPLE  ,
        GREEN  ,
        BLUE  ,
        ORANGE
    } consoleColor;
    vector<shared_ptr<string>> vecOfPoints;
    static int counterOfId ;
    int id;
    PairCoord coordOrigin;
    int widthOfBoard;
    int heightOfBoard;
    shared_ptr<shared_ptr<shared_ptr<string>[]>[]> grid;
    bool isOnboard = 0;
    bool isFullColored = 0;

public:
    Figure(int x, int y, string userConsoleColor,  bool isFullColored = 0);
    virtual ~ Figure(){};

    void virtual checkTheSameFigure();
    void virtual editCoord(int x, int y);
    bool  getFillMode();

    int& GetID() {
        return id;
    }
    virtual void editParam(int param1, int param2) {};
    virtual void editParam(int param1) {};


    virtual void editColor(string& newcolor) {
        if ( newcolor == colorToString()) {
            throw std::runtime_error("There isn't any change in color.");
        }
        setConsoleColor(newcolor);
        vecOfPoints.clear();
        this->addShapeToBoard();

    }




    bool isPointBelongToFigure(int x, int y) {
        for (shared_ptr<string> currentCoord : vecOfPoints) {
            if(currentCoord == grid[y][x]) {
                return 1;
            }
        }
        return 0;
    }

    void setConsoleColor(string& userColor) {
        if ( userColor == "puple") {
            consoleColor = AvailableColor::PURPLE;
        } else if (userColor == "green") {
                consoleColor = AvailableColor::GREEN;
        } else if (userColor ==  "orange") {
            consoleColor = AvailableColor::ORANGE;
        } else if (userColor == "blue"){
                consoleColor = AvailableColor::BLUE;
        }
    }
    string colorToString() {
        switch (consoleColor)
        {
        case AvailableColor::PURPLE: return "purple";
        case AvailableColor::GREEN: return "green";
        case AvailableColor::BLUE: return "blue";
        case AvailableColor::ORANGE: return "orange";
        default: return "";    }}

    string getConsoleColor() {
        switch(consoleColor) {
            case AvailableColor::PURPLE:
                return "\033[35m";
            break;
            case AvailableColor::GREEN:
                return "\033[32m";
            break;
            case AvailableColor::ORANGE:
                return "\033[38;5;214m";
            break;
            case AvailableColor::BLUE:
               return "\033[36m";
            break;
        }
    }

    virtual Figure* addShapeToBoard(bool isRemoved = false) = 0;
    void virtual displayGeneralInfo() = 0;

    bool checkWithinBoard(int coordX,int coordY) {
        if((0 <= coordX && coordX < widthOfBoard) && (0 <= coordY && coordY < heightOfBoard)) {
            isOnboard = 1;
            return 1;
        }
        return 0;
    }



    void alteringVectorOfPoints(int x,int y, string fillChar);
    void clearVecOfPoints() {
        vecOfPoints.clear();
    }

    bool virtual operator== (Figure*& figure) = 0;
};


class Board {
private:
    int width;
    int height;
    shared_ptr<shared_ptr<shared_ptr<string>[]>[]> grid ;
    vector<Figure*> vecOfFigure;
    Board(){};
    vector<shared_ptr<string>> vecOfLoadedPoints;
    //delete from here
    //to herO
public:
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    static Board& getInstance() {
        static Board instance;
        return instance;
        }
    void initialize(int w, int h) {
            width = w;
            height = h;
            grid =  shared_ptr<shared_ptr<shared_ptr<string>[]>[]>(new shared_ptr<shared_ptr<string>[]>[height] );
            for (int i = 0; i< height; i++) {
                grid[i] = shared_ptr<shared_ptr<string>[]>(new shared_ptr<string>[width]);
                for ( int j = 0; j< width; j++) {
                    grid[i][j] = shared_ptr<string>(new string(" " ));
                }
            }


    }




    void addFigureToTheArray(Figure* ptrOnShape) {
        if(ptrOnShape!=nullptr) {
            vecOfFigure.push_back(ptrOnShape);

            }
         else {
             cout<<"\033[31mFigure was not added cause it is outside the board\033[0m";
            }
        }
    void removeFigure(Figure* futureDeletedFigure) {
        if (!vecOfFigure.empty()) {
            for (size_t i = 0; i < vecOfFigure.size(); ++i) {
                if (vecOfFigure[i] == futureDeletedFigure) {
                    vecOfFigure.erase(vecOfFigure.begin() + i);
                    break;
                }
            }
            futureDeletedFigure->addShapeToBoard(true);
            futureDeletedFigure->clearVecOfPoints();
        } else {
            cout<<"You have not typed anything yet"<<endl;
        }
    };
    void editFigure(Figure* alteredFigure) {
        alteredFigure->addShapeToBoard(true);


    }

    Figure* getLastFigure() {
        if (!vecOfFigure.empty()) {
            return vecOfFigure.back();
        }
        cout<<"You have not typed anything yet"<<endl;

    }

    void listDrawenShapes() {
        if ( vecOfFigure.empty()) {
            cout << "\033[36mCurently there is no shapes on the board\033[0m\n";
        } else {
            for(Figure* ptrOnFigure: vecOfFigure) {
                ptrOnFigure->displayGeneralInfo();
            }
        }
    }

    int getHeight() {
        return height;
    }
    int getWidth() {
        return width;
    }

    shared_ptr<shared_ptr<shared_ptr<string>[]>[]> GetGrid() {
        return grid;
    }

    void ChangeGridAndAddNewPoints(shared_ptr<shared_ptr<shared_ptr<string>[]>[]>&& newGrid, vector<shared_ptr<string>>&& vecOfLoadedPoint, int h,int w) {
        cout<< "The board is cleared"<<endl;
        vecOfFigure.clear();
        grid = newGrid;
        height= h;
        width = w;
        vecOfLoadedPoints = vecOfLoadedPoint;
    }
    vector<Figure*>& getVecFig() {
        return vecOfFigure;
    }

    void printBoard() {
        for (int k = 0; k < height; k++) {
            for (int g = 0; g < width; g++) {
                // int counter = grid[k][g].use_count();;
                // cout<< grid[k][g].use_count();
                cout<< *grid[k][g];
                if (g == width - 1) {
                    cout<< endl;
                }
            }
        }

    }
    Figure* selectByID(int ID) {
        Figure* currentFigure;
        for( int i =  vecOfFigure.size() - 1; i>= 0; --i) {
            currentFigure = vecOfFigure[i];
            if (currentFigure->GetID() == ID) {
                return currentFigure;
            }
        }
        if(vecOfFigure.empty()) {
            throw std::runtime_error("There isn't any figure in the board!");
        }
        throw  std::runtime_error("There is no such figure with given ID!");

    }

    int getOrderOfFigure(Figure* currentFigure) {
        for( int i =  vecOfFigure.size() - 1; i>= 0; --i) {
            if (currentFigure == vecOfFigure[i]) {
                return i;
            }
        }
        return -1;
    }

    Figure* selectForegroundFigureByCoord(int x,int y) {
        Figure* currentFigure;
        if (x < 0 || x > width || y < 0 || y > height) {
            throw  std::runtime_error("Point is out board");
        }
        for( int i =  vecOfFigure.size() - 1; i>= 0; --i) {
            currentFigure = vecOfFigure[i];
            if (currentFigure->isPointBelongToFigure(x,y)) {
                return currentFigure;
            }
        }
        throw  std::runtime_error("There is no figure in this point");
    }



    void clearBoard() {
        cout<< "The board is cleared"<<endl;
        vecOfFigure.clear();
        this->initialize(width,height);
    }

    void removeFirstOccurrence( Figure* value) {
        auto it = std::find(vecOfFigure.begin(), vecOfFigure.end(), value);
        if (it != vecOfFigure.end()) {
            vecOfFigure.erase(it);
        }
    }

};


Figure::Figure(int x, int y, string userConsoleColor,  bool isFullColored ) : coordOrigin(x, y),
          widthOfBoard(Board::getInstance().getWidth()),
          heightOfBoard(Board::getInstance().getHeight()),
          isFullColored(isFullColored),
          grid(Board::getInstance().GetGrid()) {
    coordOrigin.coordY = heightOfBoard - coordOrigin.coordY - 1;
    setConsoleColor(userConsoleColor);
};
void Figure:: alteringVectorOfPoints(int x,int y, string fillChar ) {
    if(checkWithinBoard(x,y)) {
        if (fillChar == " ") {
            if((*grid[y][x]).find('*') != string::npos  && (grid[y][x].use_count())<=2 ) {
                *grid[y][x] = fillChar;
            }
            vecOfPoints.erase(find(vecOfPoints.begin(), vecOfPoints.end(), grid[y][x]));

            if (*grid[y][x] != fillChar) {
                string color = Board::getInstance().selectForegroundFigureByCoord(x,y)->getConsoleColor();
                *grid[y][x] = string(color) +"*\033[0m";
            }
        } else {
            if(Board::getInstance().getOrderOfFigure(this)==-1) {
                // cout << "was here";
                *grid[y][x] = fillChar;
            }
            else if ( (grid[y][x].use_count()>1) &&(Board::getInstance().getOrderOfFigure(this) < Board::getInstance().getOrderOfFigure(Board::getInstance().selectForegroundFigureByCoord(x,y))) && ((Board::getInstance().getOrderOfFigure(this)!=-1))) {
                string color = Board::getInstance().selectForegroundFigureByCoord(x,y)->getConsoleColor();
                *grid[y][x] = string(color) +"*\033[0m";
            } else {
                *grid[y][x] = fillChar;
            }
            vecOfPoints.push_back(grid[y][x]);

            }

        }
    };
void Figure::  editCoord(int x, int y) {
    if ((x== coordOrigin.coordX && coordOrigin.coordY == heightOfBoard - y -1) && (Board::getInstance().getVecFig().size() -1 ==  Board::getInstance().getOrderOfFigure(this) ) ) {
        throw std::runtime_error("There isn't any changes in parameter and figure is already on foreground");
    } else {
        int OriginalCoordX = coordOrigin.coordX;
        int OriginalCoordY = coordOrigin.coordY;
        int locationindexInVecFig = Board::getInstance().getOrderOfFigure(this);


        Board::getInstance().removeFigure(this);

        coordOrigin.coordX = x;
        coordOrigin.coordY = heightOfBoard - y -1 ;

        this->addShapeToBoard();

        if(vecOfPoints.empty()) {
            cout << "You can not move to invalid shape";
            coordOrigin.coordX = OriginalCoordX;
            coordOrigin.coordY =  OriginalCoordY;
            this->displayGeneralInfo();
            vector<Figure*>& vec = Board::getInstance().getVecFig();
            vec.insert(vec.begin() + locationindexInVecFig, this);
            this->addShapeToBoard();
        } else {
            Board::getInstance().addFigureToTheArray(this);
        }


}
}





void Figure:: checkTheSameFigure() {
    const vector<Figure*>& vec = Board::getInstance().getVecFig();
    string shape = typeid(*this).name();
    for(Figure* figure: vec) {
        if(typeid(*this).name() == typeid(*figure).name()) {
            if(!this->operator==(figure)) {
                cout<< "\033[36mEverything is okay, there is no such figure\033[0m\n";
            }
            else {
                throw std::invalid_argument("There should not be the same figure");
            }
        }
    }
};





        class Rectangle: public Figure {
        private:
            int width;
            int height;


            string typeOfShape = "Rectangular";

        public:
            Rectangle( int x, int y, int width, int height, string userConsoleColor, bool isFullColored = 0): Figure(x,y, userConsoleColor,isFullColored),width(width),height(height) {

                checkTheSameFigure();
                id  = counterOfId;
                counterOfId++;

                // delete from here in future because we should consider to not add it to the board if it is out of board
            };
            bool operator==(Figure*& other) override {
                Rectangle* rectangleShape = dynamic_cast<Rectangle*>(other);
                return (this->width == rectangleShape->width && this->height == rectangleShape->height && this->coordOrigin.coordX == rectangleShape->coordOrigin.coordX && this->coordOrigin.coordY == rectangleShape->coordOrigin.coordY);
            }

            void displayGeneralInfo() override {
                cout<<"ID: "<< id << ", Type of figure: "<< typeOfShape<<", Parameters: origin coordinate (left bottom vertex): ("<<coordOrigin.coordX<<", "<< coordOrigin.coordY<<"), height: "<<  height<<", width: "<<width<<endl;

            }

            Figure* addShapeToBoard(bool isRemoved = false ) override {
                string fillChar = isRemoved ? " " : getConsoleColor()+"*"+"\033[0m";
                int biggestX = coordOrigin.coordX + width;
                int biggestY = heightOfBoard - (-1* coordOrigin.coordY +  heightOfBoard  - 1 + height) - 1;


                for (int current_coordX = coordOrigin.coordX; current_coordX<= biggestX; current_coordX++){
                if(current_coordX == coordOrigin.coordX || current_coordX == biggestX) {
                    for (int i = coordOrigin.coordY; i >= biggestY; i --) {
                        alteringVectorOfPoints(current_coordX,i,fillChar);
                    }
                } else {
                    alteringVectorOfPoints(current_coordX,coordOrigin.coordY,fillChar);
                    alteringVectorOfPoints(current_coordX,biggestY,fillChar);
                    if (isFullColored) {
                        for (int i = coordOrigin.coordY - 1; i > biggestY; i --) {
                            alteringVectorOfPoints(current_coordX,i,fillChar);
                        }
                    }
                }

            }
                if (isOnboard) {
                    return this;
                }
                return nullptr;
            }


            void editParam(int w, int h) override {
                if (width == w && height == h) {
                    throw std::runtime_error("There isn't any change in parameters.");
                }
                int originalWidth = width;int originalHeight = height;
                addShapeToBoard(true);
                width = w;height = h;
                vecOfPoints.clear();

                this->addShapeToBoard();
                if ( vecOfPoints.empty()) {
                    width = originalWidth;
                    height = originalHeight;
                    addShapeToBoard();
                    throw std::runtime_error("You cannot edit to an invalid shape.");
                }
            }
        };

        class Circle: public Figure {
        private:
            int radius;
            string typeOfShape = "Circle";
        public:
            Circle(int x, int y, int radius, string userConsoleColor, bool isFullColored = 0): Figure(x,y, userConsoleColor, isFullColored),radius(radius) {
                checkTheSameFigure();
                id  = counterOfId;
                counterOfId++;
            };
            bool operator==( Figure*& other) override {
                Circle* circleShape = dynamic_cast<Circle*>(other);
                return (this->radius == circleShape->radius && this->coordOrigin.coordX == circleShape->coordOrigin.coordX && this->coordOrigin.coordY == circleShape->coordOrigin.coordY);
            }
            void displayGeneralInfo() override {
                cout<<"ID: "<< id << ", Type of figure: "<< typeOfShape<<", Parameters: origin coordinate (centre of circle): ("<<coordOrigin.coordX<<", "<< coordOrigin.coordY<<"), radius: "<< radius<< endl;
            };

            Figure* addShapeToBoard(bool isRemoved = false) override {
                int originX = coordOrigin.coordX;
                int originY = coordOrigin.coordY;
                string fillChar = isRemoved ? " " : getConsoleColor()+"*"+"\033[0m";

                if (radius == 1) {
                    for (int y = originY - 1; y <= originY + 1; ++y) {
                        for (int x = originX - 1; x <= originX + 1; ++x) {
                            if (abs(x - originX) + abs(y - originY) == 1) {
                                alteringVectorOfPoints(x,y,fillChar);
                            }
                            if(isFullColored) {
                                if( x == originX && y == originY) {
                                    alteringVectorOfPoints(x,y,fillChar);
                                }
                            }
                        }
                    }
                }
                else {
                    int counter = 0 ;
                    int theMostRightX;

                    for (int y = originY - radius; y <= originY + radius; ++y) {
                        for (int x = originX - radius; x <= originX + radius; ++x) {
                            double distance = sqrt((x - originX) * (x - originX) + (y - originY) * (y - originY));
                            if (distance > radius - 0.5 && distance < radius + 0.5) {
                                if (y == originY - radius || y == originY + radius) {
                                    alteringVectorOfPoints(x,y,fillChar);
                                } else {
                                    if(counter == 0) {
                                        alteringVectorOfPoints(x,y,fillChar);
                                        theMostRightX = originX + (originX - x);
                                        alteringVectorOfPoints(theMostRightX,y,fillChar);
                                        if(isFullColored) {
                                            for(int i = x+1; i < theMostRightX; i++) {
                                                alteringVectorOfPoints(i,y,fillChar);
                                            }
                                        }

                                    }
                                    counter++;
                                }
                            }
                        }
                        counter = 0;
                    }
                }
                if (isOnboard) {
                    return this;
                }
                return nullptr;
            }
            void editParam(int r) override {
                if (radius == r) {
                    throw std::runtime_error("There isn't any change in parameters.");
                }
                int originalRadius = radius;
                addShapeToBoard(true);

                radius = r;
                vecOfPoints.clear();
                this->addShapeToBoard();
                if ( vecOfPoints.empty()) {
                    radius = originalRadius;
                    addShapeToBoard();
                    throw std::runtime_error("You cannot edit to an invalid shape.");
                }
            }
        };


        class Triangle: public Figure {
        private:
            int heighOfTriangle;
            string typeOfShape = "isosceles triangle";
        public:
            void displayGeneralInfo() override {
                // make it just to overload opetator offstream <<
                cout<<"ID: "<< id << ", Type of figure: "<< typeOfShape<<", Parameters: origin coordinate (top vertex): ("<<coordOrigin.coordX<<", "<< coordOrigin.coordY<<"), height: "<< heighOfTriangle<< endl;
            };
            bool operator==( Figure*& other) override {
                Triangle* triangleShape = dynamic_cast<Triangle *>(other);
                return (this->heighOfTriangle == triangleShape->heighOfTriangle &&this->coordOrigin.coordX == triangleShape->coordOrigin.coordX && this->coordOrigin.coordY == triangleShape->coordOrigin.coordY);
            }

            Triangle(int x, int y, int heighOfTriangle, string userConsoleColor, bool isFullColored = 0): Figure(x,y, userConsoleColor,isFullColored),heighOfTriangle(heighOfTriangle) {

                checkTheSameFigure();
                id  = counterOfId;
                counterOfId++;
            };

            Figure* addShapeToBoard(bool isRemoved = false) override {
                int x = coordOrigin.coordX;
                int smallestY = heightOfBoard - (-1* coordOrigin.coordY +  heightOfBoard  - 1 - heighOfTriangle) - 1 ;
                string fillChar = isRemoved ? " " : string(getConsoleColor())+"*"+"\033[0m";


                for(int y = coordOrigin.coordY; y<= smallestY ;y++ ) {
                    if (y == coordOrigin.coordY) {
                        alteringVectorOfPoints(x,y,fillChar);
                    } else {
                        alteringVectorOfPoints(x,y,fillChar);
                        alteringVectorOfPoints(x- 2*(x- coordOrigin.coordX),y,fillChar);

                        if (isFullColored) {
                            for(int current_X = x- 2*(x- coordOrigin.coordX) + 1; current_X < x ; current_X++){
                                alteringVectorOfPoints(current_X,y,fillChar);
                            }
                        }
                    }
                    x++;
                }
                for (int base = x - 2*(x - coordOrigin.coordX)+2 ; base<x-1; base++ ) {
                    alteringVectorOfPoints(base,smallestY,fillChar);
                }
                if (isOnboard) {
                    return this;
                }
                return nullptr;
            }

            void editParam( int h) override {
                if ( heighOfTriangle == h) {
                    throw std::runtime_error("There isn't any change in parameters.");
                }
                int originalHeight = heighOfTriangle;
                addShapeToBoard(true);

                heighOfTriangle = h;
                vecOfPoints.clear();
                this->addShapeToBoard();


                if (vecOfPoints.empty()) {
                    heighOfTriangle = originalHeight;
                    addShapeToBoard();
                    throw std::runtime_error("You cannot edit to an invalid shape.");
                }
            }

        };

        class Line: public Figure {
        private:
            int sideLength;


            enum class Angle {
                DEG_0 = 0,
                DEG_45 = 45,
                DEG_90 = 90,
                DEG_135 = 135,
                DEG_180 = 180,
                DEG_225 = 225,
                DEG_270 = 270,
                DEG_315 = 315,
                DEG_360 = 360
            } angle;

            string typeOfShape = "Line";
        public:
            void displayGeneralInfo() override {
                cout<<"ID: "<< id << ", Type of figure: "<< typeOfShape<<", Parameters: origin coordinate : ("<<coordOrigin.coordX<<", "<< coordOrigin.coordY<<"), side lenght: "<< sideLength<<", angle: " << static_cast<int>(angle)<<endl;
            }
            bool operator==(Figure*& other) override {
                Line* lineShape = dynamic_cast<Line *>(other);
                return (this->sideLength == lineShape->sideLength && this->coordOrigin.coordX == lineShape->coordOrigin.coordX && this->coordOrigin.coordY == lineShape->coordOrigin.coordY);
            }
            Line(int x, int y, int  sideLength, int angle, string userConsoleColor): Figure(x,y, userConsoleColor),sideLength(sideLength) {

                setAngle(angle);
                checkTheSameFigure();
                id  = counterOfId;
                counterOfId++;
            };

            void setAngle(int newAngle) {
                if (newAngle == 0 || newAngle == 45 || newAngle == 90 || newAngle == 135 ||
                    newAngle == 180 || newAngle == 225 || newAngle == 270 || newAngle == 315 || newAngle == 360) {
                    angle = static_cast<Angle>(newAngle);
                    } else {
                        throw std::invalid_argument("Invalid angle. Must be 0, 45, 90, ..., 360.");
                    }
            }


            Figure* addShapeToBoard  (bool isRemoved = false) override {
                int smallestY = coordOrigin.coordY  ;
                string fillChar = isRemoved ? " " : getConsoleColor()+"*"+"\033[0m";
                int slope = static_cast<int>(angle);
                int xMin = coordOrigin.coordX - sideLength;
                int xMedian = coordOrigin.coordX;
                int xMax = coordOrigin.coordX+sideLength;
                int yMedian = smallestY;
                int yMax = smallestY - sideLength;
                int yMin = smallestY + sideLength;
                int xLimit,xStart,yLimit, yStart;
                if ( slope==0 || slope == 180 || slope == 360) {
                    if( slope == 0 || slope == 360) {
                        xStart = xMedian;
                        xLimit = xMax;
                    } else if (slope == 180) {
                        xStart = xMin;
                        xLimit = xMedian;
                    }
                    for (int x = xStart; x< xLimit; x++) {
                        alteringVectorOfPoints(x,smallestY,fillChar);
                    }
                }
                if ( slope==90 || slope == 270) {
                    if( slope == 90 ) {
                        yStart =  yMedian;
                        yLimit = yMax;
                    } else if (slope == 270) {
                        yStart = yMin;
                        yLimit = yMedian;
                    }
                    for (int y = yStart; y> yLimit; y--) {
                        alteringVectorOfPoints(xMedian,y,fillChar);
                    }
                }
                if ( slope==135 || slope == 315) {
                    if( slope == 135 ) {
                        yStart =  yMax;
                        yLimit = yMedian;
                        xStart = xMin;
                    } else if (slope == 315) {
                        yStart = yMedian;
                        yLimit = yMin;
                        xStart = xMedian;
                    }
                    for (int y = yStart; y< yLimit; y++) {
                        alteringVectorOfPoints(xStart,y,fillChar);
                        xStart++;
                    }
                }
                if ( slope==225 || slope == 45) {
                    if( slope == 225 ) {
                        yStart =  yMin;
                        yLimit = yMedian;
                        xStart = xMin;
                    } else if (slope == 45) {
                        yStart = yMedian;
                        yLimit = yMax;
                        xStart = xMedian;
                    }
                    for (int y = yStart; y> yLimit; y--) {
                        alteringVectorOfPoints(xStart,y,fillChar);
                        xStart++;
                    }
                }

                if (isOnboard) {
                    return this;
                }
                return nullptr;
            }
            void editParam(int s, int a) override {


                if (sideLength == s && static_cast<int>(angle) == a) {
                    throw std::runtime_error("There isn't any change in parameters.");
                }
                int originalSideLength = sideLength;Angle originalAngle = angle;
                addShapeToBoard(true);

                setAngle(a); sideLength= s;
                vecOfPoints.clear();
                this->addShapeToBoard() ;
                if ( vecOfPoints.empty()) {
                    angle = originalAngle;
                    sideLength = originalSideLength;
                    addShapeToBoard();
                    throw std::runtime_error("You cannot edit to an invalid shape.");
                }
            }
        };

        class FileHandler {
            private :
                fstream file;
        public:
            FileHandler(const string& filename) {
                file.open(filename, std::ios::in | std::ios::out);
                if (!file.is_open()) {
                    file.open(filename, std::ios::out);
                    file.close();
                    file.open(filename, std::ios::in | std::ios::out);
                }
                if (!file.is_open()) {
                    throw std::runtime_error("Failed to open file");
                }
                std::cout << "File opened: " << filename << std::endl;
            }

            ~FileHandler() {
                if (file.is_open()) {
                    file.close();
                    std::cout << "File closed" << std::endl;
                }
            }



            void writeToFileHtml() {

                const auto& grid = Board::getInstance().GetGrid();


                file << "<!DOCTYPE html>\n<html>\n<head>\n<title>Colored Grid</title>\n</head>\n<body>\n<pre>\n";

                for (size_t i = 0; i < Board::getInstance().getHeight(); ++i) {
                    const auto& row = grid[i];
                    for (size_t j = 0; j < Board::getInstance().getWidth(); ++j) {
                        const auto& column = row[j];
                        std::string cellContent = *column;


                            if (cellContent.find("\033[35m") != std::string::npos) {
                                file << "<span style=\"color:purple;\">*</span>";
                            } else if (cellContent.find("\033[32m") != std::string::npos) {
                                file << "<span style=\"color:green;\">*</span>";
                            } else if (cellContent.find("\033[38;5;214m") != std::string::npos) {
                                file << "<span style=\"color:orange;\">*</span>";
                            } else if (cellContent.find("\033[36m") != std::string::npos) {
                                file << "<span style=\"color:cyan;\">*</span>";
                            } else{file<< *column;}

                    }
                    file << "\n";
                }


                file << "</pre>\n</body>\n</html>";
            }
            void writeToFileBB() {

                const auto& grid = Board::getInstance().GetGrid();




                for (size_t i = 0; i < Board::getInstance().getHeight(); ++i) {
                    const auto& row = grid[i];
                    for (size_t j = 0; j < Board::getInstance().getWidth(); ++j) {
                        const auto& column = row[j];
                        std::string cellContent = *column;


                        if (cellContent.find("\033[35m") != std::string::npos) {
                            file << "[color=purple]*[/color]";;
                        } else if (cellContent.find("\033[32m") != std::string::npos) {
                            file << "[color=green]*[/color]";
                        } else if (cellContent.find("\033[38;5;214m") != std::string::npos) {
                            file << "[color=orange]*[/color]";
                        } else if (cellContent.find("\033[36m") != std::string::npos) {
                            file << "[color=blue]*[/color]";
                        } else {
                            file << *column;
                        }
                    }
                    file << "\n";
                }



            }

            void readFromFile() {
                std::vector<std::string> lines;
                file.clear();
                file.seekg(0, std::ios::beg);
                std::string line;
                size_t width = 0;



                while (std::getline(file, line)) {
                    if (line.size() > width) {
                        width = line.size();
                    }
                    lines.push_back(line);
                }

                size_t height = lines.size();
                vector<shared_ptr<string>>vecOfPoints;
                shared_ptr<shared_ptr<shared_ptr<string>[]>[]> grid =  shared_ptr<shared_ptr<shared_ptr<string>[]>[]>(new shared_ptr<shared_ptr<string>[]>[height] );
                for (size_t i = 0; i < height; ++i) {
                    grid[i] = shared_ptr<shared_ptr<string>[]>(new shared_ptr<string>[width]);
                    for (size_t j = 0; j < width; ++j) {
                        std::stringstream ss;
                        ss << lines[i][j];
                        grid[i][j] = shared_ptr<string>(new string(ss.str()));

                        if(*grid[i][j] == "*") {
                            vecOfPoints.push_back(grid[i][j]);
                        }
                        else { throw std::invalid_argument("The file contains inappriate sign/s");}
                    }
                }

                Board::getInstance().ChangeGridAndAddNewPoints(move(grid),move(vecOfPoints), height, width);
            }

        };
        class CommandHanler {
        private:
            Board& board = Board::getInstance();
            string userCommand;
            Figure* selectedFigure = nullptr;
            vector<string> words;

        public:
            void ParseUserCommand() {
                stringstream ss(userCommand);
                string word;
                words.clear();
                while (ss >> word) {
                    words.push_back(word);
                }
            }

            void execurteUserCommand(string& userCommandLine) {
                userCommand =userCommandLine;
                ParseUserCommand();
                manyCommands();
            }

            void addFigureToBoard(bool isFullColored = false) {
                Figure* figure;
                try{
                    if(words[3]== "triangle") {
                        figure = new Triangle(stoi(words[4]), stoi(words[5]), stoi(words[6]), words[2], isFullColored);
                    } else if (words[3]== "rectangle") {
                        figure = new Rectangle(stoi(words[4]), stoi(words[5]), stoi(words[6]),stoi(words[7]),words[2],isFullColored);
                    } else if (words[3]== "circle") {
                        figure = new Circle(stoi(words[4]), stoi(words[5]), stoi(words[6]), words[2],isFullColored);
                    } else {
                        figure =  new Line(stoi(words[3]), stoi(words[4]), stoi(words[5]),stoi(words[6]),words[1]);
                    }

                    board.addFigureToTheArray(figure->addShapeToBoard());

                } catch (const exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            } ;


            void manyCommands() {
                if(words[0] == "shapes" && words.size() == 1 ) {
                    cout<<"Display available shapes:\n> Rectangle: parameters: origin coordinate (left bottom vertex), width, height; \n> Triangle: parameters: origin coordinate (top vertex), height;\n> Circle: parameters: origin coordinate (centre of circle), radius;\n> Line: parameters: origin coordinate, side lenght, angle;\n";
                } else if( words[0] == "draw" && words.size() == 1) {
                    board.printBoard();
                } else if (words[0] == "clear" && words.size() == 1) {
                    board.clearBoard();
                } else if( words[0] == "undo" && words.size() == 1) {
                    board.removeFigure(board.getLastFigure());
                } else if( words[0] == "list" && words.size() == 1) {
                    board.listDrawenShapes();
                } else if(words[0] == "select" ) {
                    regex pattern(R"(select \d+( \d+)?)");
                    if(regex_match(userCommand,pattern)) {
                        try {
                            if (words.size() == 2) {
                                selectedFigure = board.selectByID(stoi(words[1]));
                                selectedFigure->displayGeneralInfo();
                            } else {
                                selectedFigure = board.selectForegroundFigureByCoord(stoi(words[1]), board.getHeight() - stoi(words[2]) -1);
                                selectedFigure->displayGeneralInfo();
                            };
                        } catch (const std::runtime_error& e){
                            std::cout << "Caught an exception: " << e.what() << std::endl;
                        }
                    } else { cout << "The string is invalid due to parameters" << std::endl;}

                } else if ( words[0] == "remove" && words.size() == 1) {
                    if(selectedFigure == nullptr) {
                        cout << "\033[36mYou haven't selected any figure yet\033[0m\n";
                    } else {
                        board.removeFigure(selectedFigure);
                        cout << "\033[36mSelected figure was rremoved\033[0m\n";

                    }

                } else if (words[0] == "edit") {
                    if(selectedFigure == nullptr) {
                        cout << "\033[36mYou haven't selected any figure yet\033[0m\n";
                    } else {
                        std::regex pattern(R"(edit\s+\d+(?:\s+\d+)?)");
                        if(regex_match(userCommand,pattern)) {
                            try {
                                if(words.size() == 2) {
                                    selectedFigure->editParam(stoi(words[1]));
                                } else {
                                    selectedFigure->editParam(stoi(words[1]), stoi(words[2]));
                                }
                            }catch (const std::runtime_error& e){
                                std::cout << "Caught an exception: " << e.what() << std::endl;
                            }
                        }
                        else{cout<<"The string is invalid due to parameters" << std::endl;}
                    }
                }
                else if(words[0]=="move") {
                    std::regex pattern(R"(move \d+ \d+)");
                    if(selectedFigure == nullptr) {
                        cout << "\033[36mYou haven't selected any figure yet\033[0m\n";
                    }else if (regex_match(userCommand,pattern)) {
                        try {

                            selectedFigure->editCoord(stoi(words[1]),stoi(words[2]));

                        } catch (const std::runtime_error& e){
                            std::cout << "Caught an exception: " << e.what() << std::endl;
                        }
                    } else {cout<<"The string is invalid due to parameters" << std::endl;}

                }
                else if(words[0]== "add") {
                    regex pattern(R"(add (?:fill|frame) (?:purple|green|blue|orange) (circle \d+ \d+ \d+|rectangle \d+ \d+ \d+ \d+|triangle \d+ \d+ \d+)|add (?:purple|green|blue|orange) line \d+ \d+ \d+ \d+)");
                    if (regex_match(userCommand, pattern)) {
                        if (words[1] == "fill") {
                            addFigureToBoard(true);
                        } else {
                            addFigureToBoard();
                        }
                    } else {
                        cout << "The string is invalid due to parameters" << std::endl;
                    }
                } else if (words[0] == "paint") {
                    if(selectedFigure == nullptr) {
                        cout << "\033[36mYou haven't selected any figure yet\033[0m\n";
                    } else {
                        std::regex pattern(R"(paint (green|orange|purple|blue))");
                        try {
                            selectedFigure->editColor(words[1]);
                        }
                        catch (const std::runtime_error& e){
                            std::cout << "Caught an exception: " << e.what() << std::endl;
                        }

                    }
                }

                else if(words[0] == "save" && words.size() == 2) {
                    regex pattern1("^.*\\.html$");
                    if (regex_match(words[1], pattern1)) {
                        try {
                            FileHandler fileWrite(words[1]);
                            fileWrite.writeToFileHtml();
                            cout << "\033[36mInformation was succesfully writen to file\033[0m\n";
                        } catch (const exception& e) {
                            cerr << e.what() << endl;
                        }
                    }
                    regex pattern2("^.*\\.bb$");
                    if (regex_match(words[1], pattern2)) {
                        try {
                            FileHandler fileWrite(words[1]);
                            fileWrite.writeToFileBB();
                            cout << "\033[36mInformation was succesfully writen to file\033[0m\n";
                        } catch (const exception& e) {
                            cerr << e.what() << endl;
                        }
                    }

                } else if (words[0] == "load" && words.size() == 2) {
                    regex pattern("^.*\\.txt$");
                    if (regex_match(words[1], pattern)) {
                        try {
                            FileHandler fileWrite(words[1]);
                            fileWrite.readFromFile();
                            cout << "\033[36mInformation was succesfully loaded to file\033[0m\n";
                        } catch (const exception& e) {
                            cerr << e.what() << endl;
                        }
                    } else {cout << "File should be with .txt extention";}

                }
                else {
                        cout <<"\033[31mThere is no such command or syntax is wrong\033[0m";
                        cout << words[1];
                    }

                }

            };




    int Figure::counterOfId = 1000;


// ../TestFileWrite/example.txt
//
int main() {
    Board::getInstance().initialize(60,20);
    CommandHanler commandHandler;

    while (true) {
        string userCommand;
        cout<<"Please, enter a command: ";
        getline(cin, userCommand);
        commandHandler.execurteUserCommand(userCommand);

    }
        return 0;
    };
// add fill purple triangle 5 5 5
// add fill purple triangle 5 5 5
// add fill blue triangle 9 5 5
// add fill blue rectangle 0 0 13 7
// add fill orange rectangle 0 4 8 5
// select 1000
// move 0 0

// draw
// select 1001
// edit 8 5 5

// add frame green circle 5 5 5
// add frame green circle 5 5 6
// add frame green circle 5 5 7
