#include <iostream>
#include <regex>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <fstream>
#include <fcntl.h>   // For open()
#include <unistd.h>  // For close()
#include <stdexcept>
#include <cstring>
#include <string>
using namespace std;

const string RED = "\033[31m";
const string GOLD = "\033[33m";
const string PURPLE = "\033[35m";
const string DARKGREEN = "\033[32m";
const string LIGHTGREEN = "\033[92m";
const string RESET = "\033[0m";


// think about how to handle varray of airplane
class FileHandler;
class Ticket;
class Airplane;
class BookHandler;
struct pair_hash;

struct pair_hash {
    size_t operator()(const pair<string, string>& p) const {
        return hash<string>()(p.first) ^ hash<string>()(p.second);
    }
};

class FileHandler {
private:
    int fileDescriptor;
    vector<string> linesFromFile;

public:
    FileHandler(const string& fileName) {
        fileDescriptor = open(fileName.c_str(), O_RDONLY);
        if (fileDescriptor == -1) {
            cout << "Error opening file: " + fileName;
        }
    }
    void readLines()  {
        const size_t bufferSize = 256;
        char buffer[bufferSize];
        string lineFromFile;
        bool skipFirstLine = true;


        ssize_t amountOfChar;
        do {
            amountOfChar = read(fileDescriptor, buffer, sizeof(buffer));
            if (amountOfChar == -1) {
                cerr << "Error reading file: " << strerror(errno) << std::endl;
            }
            for (size_t i = 0; i < amountOfChar; ++i) {
                if (buffer[i] == '\n') {
                    if (skipFirstLine) {
                        skipFirstLine = false;
                        lineFromFile.clear();
                    } else if (!lineFromFile.empty()) {
                        linesFromFile.push_back(lineFromFile);
                        lineFromFile.clear();
                    }
                } else {
                    lineFromFile += buffer[i];
                }
            }
        } while (amountOfChar > 0);

        // Add any remaining line after the loop if not empty
        if (!lineFromFile.empty()) {
            linesFromFile.push_back(lineFromFile);
        }
    }

      vector<std::string>& GetLinesFromFile()  {
        readLines();
        return linesFromFile;
    }

    ~FileHandler() { close(fileDescriptor);}
};

class ParseInfo {
protected:
    string date;
    string flightNo;
    int amountOfSeatsPerRow;
    string segmentationSeatTypeOne;
    string priceOfSeatTypeOne;
    string segmentationSeatTypeTwo;
    string priceOfSeatTypeTwo;
    string LineFromFile;

public:


    ParseInfo(const string LineFromFile): LineFromFile(LineFromFile) {
        validationOfGeneralInfo();
        setAttributes();
    }
    virtual ~ParseInfo() {}

     void validationOfGeneralInfo() {
        regex pattern(R"(\d{2}\.\d{2}\.\d{4} [A-Z]{2}\d{3} \d+ \d+-\d+ \d+\$ \d+-\d+ \d+\$)");
        if (!regex_match(LineFromFile, pattern)) {
            cout << "There is an error in content of file" << endl;

        }
    }

    void setAttributes() {
            stringstream ss(LineFromFile);
            string token;
            vector<string> tokens;
            while (ss >> token) {
                tokens.push_back(token);
            }

            date =tokens[0] ;
            flightNo = tokens[1];
            amountOfSeatsPerRow = stoi(tokens[2]);
            segmentationSeatTypeOne = tokens[3]; // maybe delete in the future
            priceOfSeatTypeOne = tokens[4];
            segmentationSeatTypeTwo = tokens[5];// maybe delete stoi in the future
            priceOfSeatTypeTwo = tokens[6];
        }


};

class Airplane: public ParseInfo {
private:
    unordered_map<string,shared_ptr<Airplane>> MapOfAirplanes;
    int edgeOfSeatSegmentation;
    int amountOfColumns;
    bool** seatAvailabilityMatrix;// matrix
    vector<int> vectorOfId;



public:
    Airplane(const string& LineFromFile): ParseInfo( LineFromFile) {
       setEdgeOfSegmentation();
        setAmontOfColumns();
        dynamicallySetWithInitializeAvailabilityMatrix();
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
    string GetFlightNo() const  {
        return flightNo;
    }
    string GetDate() const  {
        return date;
    }
    void AddVectorId(int Id) {
        vectorOfId.push_back(Id);
    }
    vector<int>& GetVectorId() {
        return vectorOfId;
    }


    void displaySalon() {
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


    void InterpretSeatcoordToMatrix( const string coordinates, int& coordColumn, char&  coordRow) {
        regex pattern("(\\d+)([A-Za-z]+)");
        smatch match;

        if (regex_match(coordinates, match, pattern)) {
            coordColumn = stoi(match[1].str());
            coordRow = match[2].str()[0] ;  // Convert the row character to an integer index (0-based)
        } else {
            cout << "\nIncorrect input" << endl;
        }
        coordRow = int(coordRow - 'A');

    }

    void bookAvailabilityMatrix(const string& coordinates) {  // book the ticket
        int coordColumn;
        char coordRow;  // Changed to int to match the expected row index type
        InterpretSeatcoordToMatrix(coordinates, coordColumn, coordRow);



        // add the case when there is no such place
        if (seatAvailabilityMatrix[coordRow][coordColumn -1] == 1) {
            cout << "\nThis place has been already booked"<<endl;
        } else {
            seatAvailabilityMatrix[coordRow][coordColumn -1] = 1;
            cout <<"The changes in seatAvailabilityMatrix"<< endl;
        };
    }
    void returnAvailabilityMatrix(const string& coordinates) {
        int coordColumn;
        char coordRow;
        InterpretSeatcoordToMatrix(coordinates,coordColumn,coordRow);
        seatAvailabilityMatrix[coordRow][coordColumn -1] = 0;
        cout<< "Seat was successfully returned"<<endl;

    }

    string getPriceOfTheGivenSeat(const string& coordinates) {
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

    void listAvailableSeats() {
        cout<<"Free seats:"<< endl;
        for (int i = 0; i < amountOfSeatsPerRow; i++) {
            for (int j = 0; j < amountOfColumns; j++) {
                if (seatAvailabilityMatrix[i][j] == 0) {
                    char interpretedCoordRow = 'A' + i;
                    string interpretedCoord =  to_string(j+1) + interpretedCoordRow;
                    if ( i  == amountOfSeatsPerRow - 1 && j == amountOfColumns - 1) {
                        cout<<interpretedCoord<<" "<<getPriceOfTheGivenSeat(interpretedCoord)<<";"<<endl;
                    } else {
                        cout<<interpretedCoord<<" "<<getPriceOfTheGivenSeat(interpretedCoord)<<", ";
                    }
                }
            }
        }
    };

// check availability of certain seat
};


class Ticket {
private:
    string FlightNo;
    string date;
    static int counterIdConfirmation;
    int IdConfirmation;
    string PassengerName;
    string seatIndex;
public:
    Ticket(const string& date,const string& FlightNo,const string& seatIndex, const string& PassengerName): date(date),FlightNo(FlightNo),PassengerName(PassengerName), seatIndex(seatIndex){
        counterIdConfirmation ++;
        IdConfirmation = counterIdConfirmation;
    }
    string& getCoordOfASeat() {
        return seatIndex;
    }

    int GetIdConfirmation() const{
        return IdConfirmation;
    }
    string GetPassengerName() const {
        return PassengerName;
    }
    string GetSeat() const {
        return seatIndex;
    }

    void setFlightDateSeat( string& Flight, string& Date, string& SeatIndex)  {
        Flight = FlightNo;
        Date = date;
        SeatIndex = seatIndex;
    }
    string GetDate() const {
        return date;
    }



};


class BookHandler {
    unordered_map<pair<string,string>, shared_ptr<Airplane>,pair_hash> MapOfAirplanes; //flightNo and aiplaneobj
    unordered_map<int,shared_ptr<Ticket>> MapOfTickets; //Id confirmation and ticket obj
    unordered_map<string,vector<int>> MapOfPassenger; // passenger name and its ids

public :
    BookHandler(const string& nameOfTheFile) {
    FileHandler InfoFile(nameOfTheFile);

    for( const auto& LineFromFile : InfoFile.GetLinesFromFile() ) {
        shared_ptr<Airplane> ptrOnAir = make_shared<Airplane>(LineFromFile);
        MapOfAirplanes[{ptrOnAir->GetFlightNo(),ptrOnAir->GetDate()}] = ptrOnAir;
    }
}

    void check(const string& flightAndDate){
        regex pattern(R"((\d{2}\.\d{2}\.\d{4})\s([A-Z]{2}\d{2}))");
        smatch match;
        string date;
        string flightNo;
        if (std::regex_match(flightAndDate, match, pattern)) {
            date = match[1].str();
            flightNo = match[2].str();
        } else {
            cout << "Invalid format\n";
        }

        shared_ptr<Airplane> ptrairobj = MapOfAirplanes[{flightNo,date}];
        if (ptrairobj== nullptr) {
            cout<< "There is no such flightNo";
        } else {
            ptrairobj->listAvailableSeats();
            ptrairobj->displaySalon();

        }

    }

    void book(const string dateFlightSeatPassenger) {
        regex pattern(R"((\d{2}\.\d{2}\.\d{4})\s([A-Z]{2}\d{2})\s(\d+[A-Z])\s([A-Za-z]+))");
        smatch match;
        string date;
        string flight;
        string seat;
        string name;
        if (std::regex_match(dateFlightSeatPassenger, match, pattern)) {
            date = match[1].str();
            flight = match[2].str();
            seat = match[3].str();
            name = match[4].str();
        } else {
            std::cout << "Invalid format\n";
        }
        shared_ptr<Ticket> ptrTicketObj = make_shared<Ticket>(date,flight,seat,name);
        MapOfTickets[ptrTicketObj->GetIdConfirmation()] = ptrTicketObj;
        vector<int>& vectorIdForUser = MapOfPassenger[name];
        vectorIdForUser.push_back(ptrTicketObj->GetIdConfirmation());
        shared_ptr<Airplane> ptrAirplaneObj = MapOfAirplanes[{flight,date}];
        if (ptrAirplaneObj == nullptr) {
            cout << "There is no such flight or no such file in this date";
        }
        ptrAirplaneObj->bookAvailabilityMatrix(seat);
        ptrAirplaneObj->AddVectorId(ptrTicketObj->GetIdConfirmation());
        ptrAirplaneObj->displaySalon();
        cout<<"Confirmed with ID "<< ptrTicketObj->GetIdConfirmation()<<endl;
    }

    void returnFunc(int&& IdConfirmation) {
        shared_ptr<Ticket>& ptrTicketObj  = MapOfTickets[IdConfirmation];
        string PassengerName = ptrTicketObj->GetPassengerName();
        vector<int>& vectorIdForUser = MapOfPassenger[PassengerName];
        vectorIdForUser.erase(remove(vectorIdForUser.begin(),vectorIdForUser.end(),IdConfirmation),vectorIdForUser.end());
        string seatIndex,flightNo,date;
        ptrTicketObj->setFlightDateSeat(flightNo,date,seatIndex);
        shared_ptr<Airplane> ptrAiplaneObj  = MapOfAirplanes[{flightNo,date}];
        ptrAiplaneObj->returnAvailabilityMatrix(seatIndex);
        vector<int>& vectorIdForFlight = ptrAiplaneObj->GetVectorId();
        vectorIdForFlight.erase(remove(vectorIdForFlight.begin(),vectorIdForFlight.end(),IdConfirmation),vectorIdForFlight.end());
        MapOfTickets.erase(IdConfirmation);
        cout<< "Confirmed: "<< ptrAiplaneObj->getPriceOfTheGivenSeat(seatIndex)<<" refund for "<<PassengerName<<endl;
        ptrAiplaneObj->displaySalon();
    }

    void viewByPassenger(string&& PassengerName) {
        vector<int>& vectorIdForUser = MapOfPassenger[PassengerName];
        size_t sizeOfVec  = vectorIdForUser.size();
        cout << PassengerName<<":\n";
        if(sizeOfVec == 0) {
            cout<< "There is no tickets for this passenger"<<endl;
        } else{
        int counter = 1;
        for (int Id : vectorIdForUser) {
            shared_ptr<Ticket> ptrTicketObj = MapOfTickets[Id];
            string seatIndex,flightNo,date;
            ptrTicketObj->setFlightDateSeat(flightNo,date,seatIndex);
            shared_ptr<Airplane> ptrAiplaneObj  = MapOfAirplanes[{flightNo,date}];
            cout<<counter<<". "<<"Flight "<< flightNo<<", "<< date<<", "<< "seat "<< seatIndex<<", "<<"price "<<ptrAiplaneObj->getPriceOfTheGivenSeat(seatIndex)<<endl;
            counter++;
        }
        }
    }
    void viewById(int&& Id) {
        shared_ptr<Ticket> ptrTicketObj = MapOfTickets[Id];
        if (ptrTicketObj == nullptr) {
            cout<<"There is no such IdConfirmation"<<endl;
        } else{
            string seatIndex,flightNo,date;
            ptrTicketObj->setFlightDateSeat(flightNo,date,seatIndex);
            shared_ptr<Airplane> ptrAiplaneObj  = MapOfAirplanes[{flightNo,date}];
            cout<<"Flight "<< flightNo<<", "<< date<<", "<< "seat "<< seatIndex<<", "<<"price "<<ptrAiplaneObj->getPriceOfTheGivenSeat(seatIndex)<<", "<< ptrTicketObj->GetPassengerName()<<endl;
        }
    }
    void viewByFlightNo(string&& flightNo) {
        for(auto genInfo : MapOfAirplanes) {
            if(genInfo.first.first == flightNo) {
                shared_ptr<Airplane> ptrAiplaneObj = MapOfAirplanes[{genInfo.first.first,genInfo.first.second}];
                vector<int>& vectorIdForFlight = genInfo.second->GetVectorId();
                for( int id: vectorIdForFlight) {
                    shared_ptr<Ticket> ptrTicketObj = MapOfTickets[id];
                    cout<< ptrTicketObj->GetSeat()<<" "<< ptrTicketObj->GetPassengerName()<<" "<<ptrAiplaneObj->getPriceOfTheGivenSeat(ptrTicketObj->GetSeat())<<"; ";
                }
            }

        }



    }

};

class Interface {
private:
    BookHandler bookSystem;
    vector<std::string> words;
public:
    Interface( BookHandler& bookSystem): bookSystem(bookSystem){};
    void command(const string& userLine) {
        stringstream ss(userLine);
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        if (words[0] == "check") {
            bookSystem.check(words[1]+" "+words[2]);
        } else if (words[0] == "book") {
            bookSystem.check(words[1]+" "+words[2]+" "+words[3]+" "+words[4]);
        }

    }
};

int Ticket::counterIdConfirmation = 10000;

// put this part to airplane
int main(){
    string UserCommand;


    BookHandler bookSystem("/Users/annanechytailenko/Desktop/Object orientired programming/Experimental/test.txt");

    bookSystem.check("11.12.2022 FQ12");
    bookSystem.book("11.12.2022 FQ12 1A AdamSmith");
    bookSystem.book("11.12.2022 FQ12 2A AdamSmith");
    bookSystem.book("11.12.2022 FQ12 21A AdamSmith");
    bookSystem.returnFunc(10001);
    bookSystem.viewByPassenger("AdamSmith");
    bookSystem.viewById(10003);
    bookSystem.viewByFlightNo("FQ12");
    Interface commandHandler(bookSystem);
    cout<<"Enter a command: ";
    getline(cin, UserCommand);
    commandHandler.command(UserCommand);

    return 0;
}

