#include <iostream>
#include <string>

class Pipe {
private:
    int id;
    std::string name;
    double length;
    int diameter;
    bool inRepair;
public:
    Pipe(int newId = 0);
    int getId() const;
    int getDiameter() const;
    void input(int chosenDiameter=0);
    void print() const;
    void setRepair(bool value);
    bool matchesName(const std::string& text) const;
    bool matchesRepair(bool value) const;
    friend std::ostream& operator<<(std::ostream& out,const Pipe& pipe);
    friend std::istream& operator>>(std::istream& in,Pipe& pipe);
};