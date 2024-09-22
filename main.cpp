#include <iostream>
#include <regex>
#include <cstring>
#include <sstream>
#include <vector>
using namespace std;

const string RED = "\033[31m";
const string GOLD = "\033[33m";
const string PURPLE = "\033[35m";
const string DARKGREEN = "\033[32m";
const string LIGHTGREEN = "\033[92m";
const string RESET = "\033[0m";


// think about how to handle varray of airplane



class ParseInfo{// change just to valid and get date and flightNo
protected:
    int amountOfAirplanes;
    string generalFlightInfoAsString;
    string date;
    string flightNo;
    int amountOfSeatsPerRow;
    double priceOfSeatTypeOne;
    double priceOfSeatTypeTwo;
    string segmentationSeatTypeOne;
    string segmentationSeatTypeTwo;

public:
    ParseInfo(const int& numAiplanes): amountOfAirplanes(numAiplanes){};
    ParseInfo(const string& generalInfo): generalFlightInfoAsString(generalInfo){validationOfGeneralInfo();setAttributes();};

     void validationOfGeneralInfo() {
         regex pattern(R"(\d{2}\.\d{2}\.\d{4} [A-Z]{2}\d{3} \d+ \d+-\d+ \d+\$ \d+-\d+ \d+\$)");
         if (!regex_match(generalFlightInfoAsString, pattern)) {
             cout << "There is an error in file" << endl;
         }
     }

    void setAttributes() {
         stringstream ss(generalFlightInfoAsString);
         string token;
         vector<string> tokens;
         while (ss >> token) {
             tokens.push_back(token);
         }

         date =tokens[0] ;
         flightNo = tokens[1];
     }
     //     amountOfSeatsPerRow = stoi(tokens[2]);
     //     segmentationSeatTypeOne = tokens[3];
     //     tokens[4].pop_back();
     //     priceOfSeatTypeOne = stod(tokens[4]);
     //     segmentationSeatTypeTwo = tokens[5];
     //     tokens[6].pop_back();
     //     priceOfSeatTypeTwo = stod(tokens[6]);
     // }

};

class Airplane: public ParseInfo {
private:
    int edgeOfSeatSegmentation;
    int amountOfColumns;
    bool** seatAvailabilityMatrix;// matrix

public:
    Airplane(const string& genInfo): ParseInfo(genInfo) {
        setAttributes();
        setEdgeOfSegmentation();
        setAmontOfColumns();
        dynamicallySetWithInitializeAvailabilityMatrix();
    }
    void setAttribute() {
        stringstream ss(generalFlightInfoAsString);
        string token;
        vector<string> tokens;
        while (ss >> token) {
            tokens.push_back(token);
        }

        amountOfSeatsPerRow = stoi(tokens[2]);
        segmentationSeatTypeOne = tokens[3];
        tokens[4].pop_back();
        priceOfSeatTypeOne = stod(tokens[4]);
        segmentationSeatTypeTwo = tokens[5];
        tokens[6].pop_back();
        priceOfSeatTypeTwo = stod(tokens[6]);
    }

    void setEdgeOfSegmentation() {
        string rangeSegmentationOne = segmentationSeatTypeOne;
        size_t indexOfEdgeOfSeatSegmenrarionInGivenString = rangeSegmentationOne.find_last_of('-');
        edgeOfSeatSegmentation = stoi(rangeSegmentationOne.substr(indexOfEdgeOfSeatSegmenrarionInGivenString + 1));
    }

    void setAmontOfColumns() {
        string rangeSegmentationTwo = segmentationSeatTypeTwo;
        size_t indexOfEdgeOfSeatSegmenrarionInGivenString = rangeSegmentationTwo.find_last_of('-');
        amountOfColumns = stoi(rangeSegmentationTwo.substr(indexOfEdgeOfSeatSegmenrarionInGivenString + 1));
    }

    void dynamicallySetWithInitializeAvailabilityMatrix() {
        seatAvailabilityMatrix = new bool*[amountOfSeatsPerRow];
        for ( int row = 0; row < amountOfSeatsPerRow; row ++ ) {
            seatAvailabilityMatrix[row] = new bool[amountOfColumns];
        }
        for(int i = 0; i < amountOfSeatsPerRow; i ++) {
            for(int j = 0; j <  amountOfColumns; j ++) {
                seatAvailabilityMatrix[i][j] = 0;
            }
        }
    }


    void displayAvailabilityOfSeat() {
        string COLOR;
      ;
        for(int i = amountOfSeatsPerRow-1; i >= 0; i--) {
            cout << GOLD << char('A'+i) << "|" << RESET ;
            for (int j = 0; j < amountOfColumns; j ++) {
                if( j< edgeOfSeatSegmentation && seatAvailabilityMatrix[i][j] == 0) {
                    COLOR = LIGHTGREEN;
                } else if (j >= edgeOfSeatSegmentation && seatAvailabilityMatrix[i][j] == 0) {
                    COLOR = DARKGREEN;
                } else {
                    COLOR = RED;
                }

                // add handle with avaibility by logic of matrix aij
                if (j == amountOfColumns - 1)  {
                    cout << COLOR<< " □|" << RESET<<endl;
                } else {
                    cout << COLOR<< " □|" << RESET;
                }
            }
            if (i == 0) {
                cout << "  ";
                for (int k = 1; k <= amountOfColumns; k++) {
                    if(k > 10) {
                        cout<<GOLD << k <<" "<< RESET;
                    }else {
                        cout<<GOLD<< " "<< k <<" "<< RESET;
                    }
                }
            }
        }
        cout<<"\n";
    }

    void changeAvailabilityMatrix(const string& coordinates) {// book the ticket
        regex pattern("(\\d+)([A-Za-z]+)");
        smatch match;
        int coordColumn;
        char coordRow;
        if (regex_match(coordinates, match, pattern)) {
            coordColumn =  stoi(match[1].str());
            coordRow = match[2].str()[0];
        }else {
            cout << "\nIncorrect input"<<endl;
        }

        int interpretedCoordRow = int (coordRow - 'A');
        seatAvailabilityMatrix[interpretedCoordRow][coordColumn -1] = 1;
    }

    double getPriceOfTheGivenSeat(const string& coordinates) {
        regex pattern("(\\d+)([A-Za-z]+)");
        smatch match;
        int coordColumn;
        if (regex_match(coordinates, match, pattern)) {
            coordColumn =  stoi(match[1].str());
        }else {
            cout << "\nIncorrect input"<<endl;
        }
        if (coordColumn < edgeOfSeatSegmentation) {
            return priceOfSeatTypeOne;
        }else {
            return priceOfSeatTypeTwo;
        }

    };


};


class Ticket : public ParseInfo {
private:
    int const STARTIDNUMERATION = 1000;
    int IdConfirmation;
    mutable string PassengerName;
    string seatIndex;
    mutable int counterForIdConfiramtion;
public:
    Ticket generateIdConfirmation (Airplane& airplaneObj, const string& indexOfSeat, const string& Passenger) {

        PassengerName = Passenger;
        seatIndex = indexOfSeat;
        IdConfirmation = STARTIDNUMERATION + ( counterForIdConfiramtion++);


    }



};


class CommandLineInterface {
    // command for easier realisation in main
};



// put this part to airplane
int main(){
    Airplane airObj("11.12.2022 FQ12 6 1-10 100$ 11-50 50$");
    airObj.displayAvailabilityOfSeat();
    airObj.changeAvailabilityMatrix("2B");
    airObj.displayAvailabilityOfSeat();
    return 0;
}

