#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

// Regex----------------------------------------------------------------------------------------------------------------

regex add_node_pattern("^add node (\\W+)$");
regex add_resistor_pattern(R"(^add resistor (\w+) (\w+) (\w+) (\w+)$)");
regex add_VS_pattern(R"(^add voltage source (\w+) (\w+) (\w+)$)");
regex add_ground_pattern("^add ground (\\w+)$");
regex read_current_pattern("^read current (\\w+)$");
regex read_voltage_pattern("^read voltage (\\w+)$");
regex read_node_V_pattern("^read node voltage (\\w+)$");

// Model----------------------------------------------------------------------------------------------------------------

class Node
{
private:
    int number;
public:
    float get_voltage;
};

class Component
{
protected:
    string name;
    Node node_1;
    Node node_2;
public:
    virtual float get_voltage() = 0;
    virtual float get_current() = 0;
};

class Voltage_Source : public Component
{
public:
    float get_voltage() override
    {

    }

    float get_current() override
    {

    }
};

class Resistor : public Component
{
public:
    float get_voltage() override
    {

    }

    float get_current() override
    {

    }
};

// Controller-----------------------------------------------------------------------------------------------------------

class Controller
{
private:
    Voltage_Source VS;
    vector<Resistor> resistors;
public:

};

// Int Main-------------------------------------------------------------------------------------------------------------

int main()
{
    string command;
    smatch match;
    while (true)
    {
        if (command == "end")
            return 0;

        else if (regex_match(command, match, add_node_pattern))
        {

        }
        else if (regex_match(command, match, add_resistor_pattern))
        {

        }
        else if (regex_match(command, match, add_VS_pattern))
        {

        }
        else if (regex_match(command, match, add_ground_pattern))
        {

        }
        else if (regex_match(command, match, read_current_pattern))
        {

        }
        else if (regex_match(command, match, read_voltage_pattern))
        {

        }
        else if (regex_match(command, match, read_node_V_pattern))
        {

        }
    }
}