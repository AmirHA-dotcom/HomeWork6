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
    string name;
    int number;
public:
    Node(string& tag, int NO) : name(tag), number(NO) {}
    float get_voltage
    {

    };
};

class Component
{
protected:
    string name;
    Node node_1;
    Node node_2;
public:
    Component(string& tag, Node& first, Node& second) : name(tag), node_1(first), node_2(second) {}
    virtual float get_voltage() = 0;
    virtual float get_current() = 0;
};

class Voltage_Source : public Component
{
private:
    float voltage;
public:
    Voltage_Source(string& tag, Node f, Node s, float V) : Component(tag, f, s), voltage(V) {}

    float get_voltage() override
    {

    }

    float get_current() override
    {

    }
};

class Resistor : public Component
{
private:
    float resistance;
public:
    Resistor(string& tag, Node f, Node s, float R) : Component(tag, f, s), resistance(R) {}

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
    vector<Node*> nodes;
    Voltage_Source* VS;
    vector<Resistor*> resistors;
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