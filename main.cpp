#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

using namespace std;

int readFromFile(const string& fileName);
string bin_str16_2(unsigned short value);
void writeToFile(const string& fileName, int value);
unsigned char extract8MiddleBits(unsigned short value);

// Körning av programmet
int main () {
    string fileName = "DataFile.dat";

    writeToFile(fileName, 12549);
    writeToFile(fileName, 12346);
    writeToFile(fileName, 63786);
    writeToFile(fileName, 102);
    writeToFile(fileName, 9023);

    readFromFile(fileName);

    return 0;
}

// Omvandlare till 8 bits som returnerar sträng
template <typename T>
string bin_str8(T n) {
    bitset<8> bs(n);
    return bs.to_string();
}

// Extraherar åtta mittersta bitar från ett 16 bitars värde.
unsigned char extract8MiddleBits(unsigned short value) {
    value = value >> 4;
    unsigned short newValue = value & 0xFF;
    return newValue;
}

// Konvertering till hexdecimala tal i string
string to_hex(unsigned short x){
    stringstream s;
    s << hex << x;
    return s.str();
}

// Tar little endian från hexen och sätter den före big endian
unsigned short toLittleEndian(unsigned short big) {
    short little = (((big & 0xFF) << 8) | ((big & 0xFF00)>>8));
    return little;
}

// Omvandlare till 16 bits som returnerar sträng
string bin_str16_2(unsigned short value) {
    string result;
    int count = 0;
    for(int i = 1; i < 32769; i*=2){
        if((value & i) == i) {
            result = "1" + result;
        }
        else {
            result = "0" + result;
        }
        if (++count % 4 == 0) {
            result = " " + result;
        }
    }
    return result;
}

// Metod för att läsa av filer samt kalla på de olika metoderna
int readFromFile(const string& fileName) {
    unsigned short value;
    unsigned short result;
    unsigned short newValue;

    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Could not open the file - '" << fileName << "'" << endl;
        return EXIT_FAILURE;
    }

    while (file >> value) {
        cout << "Reading value: '" << value << "' from file '" << fileName << "'" "\n";
        cout << "   In Hex: " << to_hex(value) << "\n";
        result = toLittleEndian(value);
        cout << "   In reversed endian: " << to_hex(result) << endl;

        cout << "Extracting and copying eight middle bits from reversed endian " << endl;
        newValue = extract8MiddleBits(result);
        cout << "   Before extraction:" << bin_str16_2(result) << endl;
        cout << "   After extraction: " << bin_str8(newValue) << endl;
        cout << "   Storing new value from extraction in file: 'ExtractedBitsFile.dat'" << endl;

        writeToFile("ExtractedBitsFile.dat", newValue);
        cout << "   Storing new value in file: 'ReversedEndianFile.dat'" << endl;
        writeToFile("ReversedEndianFile.dat", result);
        cout << "--------------------------------------------------- " << endl;
    }
    cout << endl;
    file.close();
    return 0;
}

// Metod för att skriva till en fil
void writeToFile(const string& fileName, int value) {
    ofstream file;
    file.open (fileName, ofstream::app);
    file << value << "\n";
    file.close();
    cout << "   Writing value: '" << value<< "' to file: '"<< fileName << "'" << endl;
    cout << "--------------------------------------------------- " << endl;
}