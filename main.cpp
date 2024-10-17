#include <iostream>
#include <memory>
#include <sstream>
#include <fstream>
#include<regex>
#include <set>
using namespace std;



class Board;

struct PairCoord {
    int coordX = 0;
    int coordY = 0;
    PairCoord(int x, int y) : coordX(x), coordY(y) {}
};

class Figure {
protected:
    vector<shared_ptr<char>> vecOfPoints;
    static int counterOfId ;
    int id;
    PairCoord coordOrigin;
    int widthOfBoard;
    int heightOfBoard;
    shared_ptr<shared_ptr<shared_ptr<char>[]>[]> grid;
    bool isOnboard = 0;

public:
    Figure(int x, int y);

    void virtual checkTheSameFigure();



    virtual Figure* addShapeToBoard(bool isRemoved = false) {};
    void virtual displayGeneralInfo() {
        cout<<"Display available shapes:\n> Rectangle: parameters: origin coordinate (left bottom vertex), width, height; \n> Triangle: parameters: origin coordinate (top vertex), height;\n> Circle: parameters: origin coordinate (centre of circle), radius;\n> Line: parameters: origin coordinate, side lenght, angle;\n";
    };

    bool checkWithinBoard(int coordX,int coordY) {
        if((0 <= coordX && coordX < widthOfBoard) && (0 <= coordY && coordY < heightOfBoard)) {
            isOnboard = 1;
            return 1;
        }
        return 0;
    }

    void alteringVectorOfPoints(int x,int y, char fillChar);
    void clearVecOfPoints() {
        vecOfPoints.clear();
    }

    bool virtual operator== (Figure*& figure) {};
};


class Board {
private:
    int width;
    int height;
    shared_ptr<shared_ptr<shared_ptr<char>[]>[]> grid = nullptr;
    vector<Figure*> vecOfFigure;
    Board(){};
    vector<shared_ptr<char>> vecOfLoadedPoints;
    //delete from here
    //to here
public:
    static Board& getInstance() {
        static Board instance;
        return instance;
        }
    void initialize(int w, int h) {
            width = w;
            height = h;
            grid =  shared_ptr<shared_ptr<shared_ptr<char>[]>[]>(new shared_ptr<shared_ptr<char>[]>[height] );
            for (int i = 0; i< height; i++) {
                grid[i] = shared_ptr<shared_ptr<char>[]>(new shared_ptr<char>[width]);
                for ( int j = 0; j< width; j++) {
                    grid[i][j] = shared_ptr<char>(new char(' '));
                }
            }


    }
    void addFigureToTheArray(Figure* ptrOnShape) {
        if(ptrOnShape!=nullptr) {
            vecOfFigure.push_back(ptrOnShape);
            cout<< "\033[35mFigure was successfully added\033[0m\n";
            }
         else {
             cout<<"\033[31mFigure was not added cause it is outside the board\033[0m";
            }
        }


    void removeLastFigure() {
        if (!vecOfFigure.empty()) {
            Figure* lastAddedFigure = vecOfFigure.back();
            lastAddedFigure->addShapeToBoard(true);
            lastAddedFigure->clearVecOfPoints();// maybe not
            vecOfFigure.pop_back();
            cout<<"The last figure was removed"<<endl;
        } else {
            cout<<"You have not typed anything yet"<<endl;
        };
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

    shared_ptr<shared_ptr<shared_ptr<char>[]>[]> GetGrid() {
        return grid;
    }

    void ChangeGridAndAddNewPoints(shared_ptr<shared_ptr<shared_ptr<char>[]>[]>&& newGrid, vector<shared_ptr<char>>&& vecOfLoadedPoint, int h,int w) {
        cout<< "The board is cleared"<<endl;
        vecOfFigure.clear();
        grid = newGrid;
        height= h;
        width = w;
        vecOfLoadedPoints = vecOfLoadedPoint;
    }
    const vector<Figure*>& getVecFig() {
        return vecOfFigure;
    }

    void printBoard() {
        for (int k = 0; k < height; k++) {
            for (int g = 0; g < width; g++) {
                // int counter = grid[k][g].use_count();;
                // cout<< grid[k][g].use_count();
                cout<< grid[k][g];
                if (g == width - 1) {
                    cout<< endl;
                }
            }
        }

    }

    void clearBoard() {
        cout<< "The board is cleared"<<endl;
        vecOfFigure.clear();
        this->initialize(width,height);
    }

};


Figure::Figure(int x, int y) : coordOrigin(x, y),
          widthOfBoard(Board::getInstance().getWidth()),
          heightOfBoard(Board::getInstance().getHeight()),
          grid(Board::getInstance().GetGrid()) {
    coordOrigin.coordY = heightOfBoard - coordOrigin.coordY - 1;
};
void Figure:: alteringVectorOfPoints(int x,int y, char fillChar) {
    if(checkWithinBoard(x,y)) {
        if (fillChar == ' ') {
            if(*grid[y][x] == '*' && (grid[y][x].use_count())<=2 ) {
                *grid[y][x] = fillChar;
            }
            vecOfPoints.erase(find(vecOfPoints.begin(), vecOfPoints.end(), grid[y][x]));
        } else {
            *grid[y][x] = fillChar;
            vecOfPoints.push_back(grid[y][x]);

        }
    }
};

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
            int biggestY;
            int biggestX;

            string typeOfShape = "Rectangular";

        public:
            Rectangle( int x, int y, int width, int height): Figure(x,y),width(width),height(height) {
                biggestX = coordOrigin.coordX + width;
                biggestY = heightOfBoard - (-1* coordOrigin.coordY +  heightOfBoard  - 1 + height) - 1;
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

            Figure* addShapeToBoard(bool isRemoved = false) override {
                char fillChar = isRemoved ? ' ' : '*';

                for (int i = coordOrigin.coordY; i >= biggestY; i-- ) {
                    alteringVectorOfPoints(coordOrigin.coordX,i,fillChar);
                    alteringVectorOfPoints(biggestX,i,fillChar);
                }
                for (int j = coordOrigin.coordX; j <= biggestX; j++ ) {
                    alteringVectorOfPoints(j,coordOrigin.coordY,fillChar);
                    alteringVectorOfPoints(j,biggestY,fillChar);
                }
                if (isOnboard) {
                    return this;
                }
                return nullptr;
            }
        };

        class Circle: public Figure {
        private:
            int radius;
            string typeOfShape = "Circle";
        public:
            Circle(int x, int y, int radius): Figure(x,y),radius(radius) {
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
                char fillChar = isRemoved ? ' ' : '*';

                if (radius == 1) {
                    for (int y = originY - 1; y <= originY + 1; ++y) {
                        for (int x = originX - 1; x <= originX + 1; ++x) {
                            if (abs(x - originX) + abs(y - originY) == 1) {
                                alteringVectorOfPoints(x,y,fillChar);
                            }
                        }
                    }
                }
                else {

                    for (int y = originY - radius; y <= originY + radius; ++y) {
                        for (int x = originX - radius; x <= originX + radius; ++x) {
                            double distance = sqrt((x - originX) * (x - originX) + (y - originY) * (y - originY));
                            if (distance > radius - 0.5 && distance < radius + 0.5) {
                                alteringVectorOfPoints(x,y,fillChar);
                            }
                        }
                    }
                }
                if (isOnboard) {
                    return this;
                }
                return nullptr;
            }

        };


        class Triangle: public Figure {
        private:
            int heighOfTriangle;
            int smallestY;
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

            Triangle(int x, int y, int heighOfTriangle): Figure(x,y),heighOfTriangle(heighOfTriangle) {
                smallestY = heightOfBoard - (-1* coordOrigin.coordY +  heightOfBoard  - 1 - heighOfTriangle) - 1 ;
                checkTheSameFigure();
                id  = counterOfId;
                counterOfId++;
            };

            Figure* addShapeToBoard(bool isRemoved = false) override {
                int x = coordOrigin.coordX;
                char fillChar = isRemoved ? ' ' : '*';

                for(int y = coordOrigin.coordY; y<= smallestY ;y++ ) {
                    if (y == coordOrigin.coordY) {
                        alteringVectorOfPoints(x,y,fillChar);
                    } else {
                        alteringVectorOfPoints(x,y,fillChar);
                        alteringVectorOfPoints(x- 2*(x- coordOrigin.coordX),y,fillChar);
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
        };

        class Line: public Figure {
        private:
            int sideLength;
            int smallestY;

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
            Line(int x, int y, int  sideLength, int angle): Figure(x,y),sideLength(sideLength) {
                smallestY = heightOfBoard - y -1 ;
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
                char fillChar = isRemoved ? ' ' : '*';
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

            void writeToFile() {
                file.seekp(0, std::ios::beg);
                const shared_ptr<shared_ptr<shared_ptr<char>[]>[]>& grid = Board::getInstance().GetGrid();
                for (size_t i = 0; i < Board::getInstance().getHeight(); ++i) {
                    shared_ptr<shared_ptr<char>[]>& row = grid[i];
                    for (size_t j = 0; j < Board::getInstance().getWidth(); ++j) {
                        shared_ptr<char> column = row[j];
                        if (column) {
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
                vector<shared_ptr<char>>vecOfPoints;
                auto grid =  shared_ptr<shared_ptr<shared_ptr<char>[]>[]>(new shared_ptr<shared_ptr<char>[]>[height] );

                for (size_t i = 0; i < height; ++i) {
                    grid[i] = shared_ptr<shared_ptr<char>[]>(new shared_ptr<char>[width]);
                    for (size_t j = 0; j < width; ++j) {
                        grid[i][j] = std::make_shared<char>(lines[i][j]);
                        if(*grid[i][j] == '*') {
                            vecOfPoints.push_back(grid[i][j]);
                        }
                    }
                }

                Board::getInstance().ChangeGridAndAddNewPoints(move(grid),move(vecOfPoints), height, width);
            }

        };
        class CommandHanler {
        private:
            Board& board = Board::getInstance();
            Figure figure = Figure(0,0);
            string userCommand;
            vector<string> words;
            std::set<std::string> validShapes = {"circle", "line", "triangle", "rectangle"};
            // see example that in experimentAssignment2 to make simplifiied interface
        public:
            void ParseUserCommand() {
                stringstream ss(userCommand);
                string word;
                while (ss >> word) {
                    words.push_back(word);
                }
            }

            void execurteUserCommand(string& userCommandLine) {
                userCommand =userCommandLine;
                ParseUserCommand();
                manyCommands();
            }

            void addFigureToBoard() {
                Figure* figure;
                try{
                    if(words[1]== "triangle") {
                        figure = new Triangle(stoi(words[2]), stoi(words[3]), stoi(words[4]));
                    } else if (words[1]== "rectangle") {
                        figure = new Rectangle(stoi(words[2]), stoi(words[3]), stoi(words[4]),stoi(words[5]));
                    } else if (words[1]== "circle") {
                        figure = new Circle(stoi(words[2]), stoi(words[3]), stoi(words[4]));
                    } else {
                        figure = figure = new Line(stoi(words[2]), stoi(words[3]), stoi(words[4]),stoi(words[5]));
                    }

                    board.addFigureToTheArray(figure->addShapeToBoard());
                } catch (const exception& e) {
                    std::cerr << e.what() << std::endl;
                }
            } ;


            void manyCommands() {
                if(words[0] == "shapes" && words.size() == 1 ) {
                    figure.displayGeneralInfo();
                } else if( words[0] == "draw" && words.size() == 1) {
                    board.printBoard();
                } else if (words[0] == "clear" && words.size() == 1) {
                    board.clearBoard();
                } else if( words[0] == "undo" && words.size() == 1) {
                    board.removeLastFigure();
                } else if( words[0] == "list" && words.size() == 1) {
                    board.listDrawenShapes();
                } else if(words[0]== "add" && validShapes.count(words[1])) {
                    regex pattern;
                    if((words[1] == "line" || words[1] == "rectangle") && words.size() == 6) {
                        pattern= "^[a-zA-Z]+\\s[a-zA-Z]+\\s-?\\d+\\s-?\\d+\\s-?\\d+\\s-?\\d+$";
                    } else if ((words[1]== "circle" || words[1] == "triangle") && words.size() == 5)  {
                        pattern = "^[a-zA-Z]+\\s+[a-zA-Z]+\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)$";
                    }
                    if (regex_match(userCommand, pattern)) {
                        addFigureToBoard();
                    } else {
                        cout << "The string is invalid due to parameters" << std::endl;
                    }
                } else if(words[0] == "save" && words.size() == 2) {
                    regex pattern("^.*\\.txt$");
                    if (regex_match(words[1], pattern)) {
                        try {
                            FileHandler fileWrite(words[1]);
                            fileWrite.writeToFile();
                            cout << "\033[36mInformation was succesfully writen to file\033[0m\n";
                        } catch (const exception& e) {
                            cerr << e.what() << endl;
                        }
                    } else {cout << "File should be with .txt extention";}
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

    while (true) {
    CommandHanler commandHandler;
    string userCommand;
    cout<<"Please, enter a command: ";
    getline(cin, userCommand);
    commandHandler.execurteUserCommand(userCommand);
    }
        return 0;
    }
