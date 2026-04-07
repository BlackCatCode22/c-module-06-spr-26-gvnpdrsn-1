// zooChallenge.cpp
// Created by GP on 4/7/2026.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

string genBirthDay(int age, const string& season) {
    int currentYear = 2026;
    int birthYear = currentYear - age;
    string date = "-01-01";

    if (season == "spring") date = "-03-15";
    else if (season == "summer") date = "-06-15";
    else if (season == "fall")   date = "-09-15";
    else if (season == "winter") date = "-12-15";

    return to_string(birthYear) + date;
}

string genUniqueID(const string& species, int number) {
    string prefix;
    if (species == "Hyena") prefix = "Hy";
    else if (species == "Lion") prefix = "Li";
    else if (species == "Tiger") prefix = "Ti";
    else if (species == "Bear") prefix = "Be";

    return prefix + (number < 10 ? "0" : "") + to_string(number);
}

class Animal {
protected:
    string id, name, species, birthSeason, birthDate, color, sex, origin;
    int age, weight;

public:
    Animal(const string& i, const string& n, const string& s, int a, const string& b,
           const string& c, const string& sx, int w, const string& o)
        : id(i), name(n), species(s), age(a), birthSeason(b), color(c), sex(sx),
          weight(w), origin(o)
    {
        birthDate = genBirthDay(age, birthSeason);
    }

    string getSpecies() const { return species; }

    void print(ostream& out) const {
        string arrivalDate = "2026-04-07";
        out << id << "; " << name << "; birth date " << birthDate << "; "
            << color << "; " << sex << "; " << weight << " pounds; from "
            << origin << "; arrived " << arrivalDate << endl;
    }

    virtual ~Animal() {}
};

class Hyena : public Animal {
public:
    Hyena(const string& id, const string& name, int age, const string& birth,
          const string& color, const string& sex, int weight, const string& origin)
        : Animal(id, name, "Hyena", age, birth, color, sex, weight, origin) {}
};

class Lion : public Animal {
public:
    Lion(const string& id, const string& name, int age, const string& birth,
         const string& color, const string& sex, int weight, const string& origin)
        : Animal(id, name, "Lion", age, birth, color, sex, weight, origin) {}
};

class Tiger : public Animal {
public:
    Tiger(const string& id, const string& name, int age, const string& birth,
          const string& color, const string& sex, int weight, const string& origin)
        : Animal(id, name, "Tiger", age, birth, color, sex, weight, origin) {}
};

class Bear : public Animal {
public:
    Bear(const string& id, const string& name, int age, const string& birth,
         const string& color, const string& sex, int weight, const string& origin)
        : Animal(id, name, "Bear", age, birth, color, sex, weight, origin) {}
};

int main() {
    vector<Animal*> animals;
    map<string,int> count;
    vector<string> hyenaNames, lionNames, tigerNames, bearNames;
    int hy=0, li=0, ti=0, be=0;

    ifstream nameFile("C:/cStuff/animalNames.txt");
    if (!nameFile) {
        cerr << "Error opening animalNames.txt" << endl;
        return 1;
    }

    string lineName, current;
    while (getline(nameFile, lineName)) {
        lineName.erase(0, lineName.find_first_not_of(" \t"));
        string lcLine = lineName;
        transform(lcLine.begin(), lcLine.end(), lcLine.begin(), ::tolower);

        if (lcLine.find("hyena") != string::npos) { current="hyena"; continue; }
        if (lcLine.find("lion") != string::npos)  { current="lion"; continue; }
        if (lcLine.find("tiger") != string::npos) { current="tiger"; continue; }
        if (lcLine.find("bear") != string::npos)  { current="bear"; continue; }
        if (lineName.empty()) continue;

        stringstream ss(lineName);
        string name;
        while (getline(ss, name, ',')) {
            name.erase(0, name.find_first_not_of(" "));
            name.erase(name.find_last_not_of(" ")+1);

            if(current=="hyena") hyenaNames.push_back(name);
            else if(current=="lion") lionNames.push_back(name);
            else if(current=="tiger") tigerNames.push_back(name);
            else if(current=="bear") bearNames.push_back(name);
        }
    }

    string folder = "C:/cStuff/";
    if(!fs::exists(folder)) {
        cout << "Folder does not exist. Creating folder: " << folder << endl;
        fs::create_directories(folder);
    }

    string outPath = folder + "zooPopulation.txt";
    ofstream outFile(outPath);
    if(!outFile) {
        cerr << "Error: cannot open file for writing: " << outPath << endl;
        return 1;
    }

    cout << "Writing report to: " << outPath << endl;

    ifstream inFile("C:/cStuff/arrivingAnimals.txt");
    if(!inFile){
        cerr << "Error opening arrivingAnimals.txt" << endl;
        return 1;
    }

    cout << "Processing Animals...\n" << endl;
    string line;
    while (getline(inFile, line)) {
        int age=0, weight=0;
        string sex, species, birthSeason="unknown", color, origin;

        string part1, part2, part3, part4, part5;
        stringstream ss(line);
        getline(ss, part1, ','); getline(ss, part2, ','); getline(ss, part3, ',');
        getline(ss, part4, ','); getline(ss, part5);

        string year, oldWord;
        stringstream first(part1);
        first >> age >> year >> oldWord >> sex >> species;

        transform(species.begin(), species.end(), species.begin(), ::tolower);
        transform(part2.begin(), part2.end(), part2.begin(), ::tolower);

        if(part2.find("spring")!=string::npos) birthSeason="spring";
        else if(part2.find("summer")!=string::npos) birthSeason="summer";
        else if(part2.find("fall")!=string::npos) birthSeason="fall";
        else if(part2.find("winter")!=string::npos) birthSeason="winter";

        int pos = part3.find(" color");
        if(pos!=-1) color=part3.substr(0,pos);
        color.erase(0, color.find_first_not_of(" "));

        string pounds;
        stringstream weightStream(part4);
        weightStream >> weight >> pounds;

        if(part5.size() >= 5) origin = part5.substr(5);
        origin.erase(0, origin.find_first_not_of(" "));

        if(species=="hyena" && hy < hyenaNames.size()){
            string id = genUniqueID("Hyena", hy+1);
            animals.push_back(new Hyena(id, hyenaNames[hy], age, birthSeason, color, sex, weight, origin));
            hy++; count["Hyena"]++;
        } else if(species=="lion" && li < lionNames.size()){
            string id = genUniqueID("Lion", li+1);
            animals.push_back(new Lion(id, lionNames[li], age, birthSeason, color, sex, weight, origin));
            li++; count["Lion"]++;
        } else if(species=="tiger" && ti < tigerNames.size()){
            string id = genUniqueID("Tiger", ti+1);
            animals.push_back(new Tiger(id, tigerNames[ti], age, birthSeason, color, sex, weight, origin));
            ti++; count["Tiger"]++;
        } else if(species=="bear" && be < bearNames.size()){
            string id = genUniqueID("Bear", be+1);
            animals.push_back(new Bear(id, bearNames[be], age, birthSeason, color, sex, weight, origin));
            be++; count["Bear"]++;
        }
    }

    auto printHabitat = [&](const string& habitat, const string& speciesName){
        outFile << habitat << ":\n";
        cout << habitat << ":\n";
        for(auto a: animals)
            if(a->getSpecies() == speciesName){
                a->print(outFile);
                a->print(cout);
            }
        outFile << "\nTotal " << speciesName << "s: " << count[speciesName] << "\n\n";
        cout << "\nTotal " << speciesName << "s: " << count[speciesName] << "\n\n";
    };

    printHabitat("Hyena Habitat", "Hyena");
    printHabitat("Lion Habitat", "Lion");
    printHabitat("Tiger Habitat", "Tiger");
    printHabitat("Bear Habitat", "Bear");

    outFile.flush();
    outFile.close();

    cout << "Report written to zooPopulation.txt" << endl;

    for(auto a: animals) delete a;
    return 0;
}