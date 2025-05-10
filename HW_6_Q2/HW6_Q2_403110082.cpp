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
    float voltage;
public:
    Node(string& tag, int NO) : name(tag), number(NO) {voltage = 0.0f;}
    string get_name() const {return name;}
    void set_voltage(float v) {voltage = v;}
    float get_voltage() const {return voltage;}
};

class Component
{
protected:
    string name;
    Node* node_1;
    Node* node_2;
public:
    Component(string& tag, Node* first, Node* second) : name(tag), node_1(first), node_2(second) {}
    virtual float get_voltage() = 0;
    virtual float get_current() = 0;
};

class Voltage_Source : public Component
{
private:
    float voltage;
public:
    Voltage_Source(string& tag, Node* f, Node* s, float V) : Component(tag, f, s), voltage(V) {}
    string get_name() const {return name;}
    float get_voltage() override
    {
        return voltage;
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
    string get_name() const {return name;}
    Resistor(string& tag, Node* f, Node* s, float R) : Component(tag, f, s), resistance(R) {}
    float get_voltage() override
    {
        return abs(node_1->get_voltage() - node_2->get_voltage());
    }
    float get_current() override
    {
        return abs(node_1->get_voltage() - node_2->get_voltage()) / resistance;
    }
};

// Controller-----------------------------------------------------------------------------------------------------------
int node_NO = 0;

class Controller
{
private:
    vector<Node*> nodes;
    Voltage_Source* VS;
    vector<Resistor*> resistors;
    Node* ground = nullptr;
public:
    void add_node(string name)
    {
        nodes.push_back(new Node(name, ++node_NO));
    }
    void add_resistor(string name, float R, string node_1, string node_2)
    {
        Node* N1 = nullptr;
        Node* N2 = nullptr;
        for (Node* n: nodes)
        {
            if (n->get_name() == node_1)
                N1 = n;
            else if (n->get_name() == node_2)
                N2 = n;
        }
        resistors.push_back(new Resistor(name, N1, N2, R));
    }
    void add_VS(string name, float V, string node_1, string node_2)
    {
        Node* N1 = nullptr;
        Node* N2 = nullptr;
        for (Node* n: nodes)
        {
            if (n->get_name() == node_1)
                N1 = n;
            else if (n->get_name() == node_2)
                N2 = n;
        }
        VS = new Voltage_Source(name, N1, N2, V);
    }
    void read_current(const string& name)
    {
        if (name == "VIN")
        {
            cout << "VIN current = " << VS->get_current();
            return;
        }
        for (Resistor* R: resistors)
        {
            if (R->get_name() == name)
            {
                cout << R->get_name() << " current = " << R->get_current() << " amps" << endl;
                return;
            }
        }
    }
    void read_voltage(const string& name)
    {
        if (name == "VIN")
        {
            cout << "VIN current = " << VS->get_voltage();
            return;
        }
        for (Resistor* R: resistors)
        {
            if (R->get_name() == name)
            {
                cout << R->get_name() << " voltage = " << R->get_voltage() << " Volts" << endl;
                return;
            }
        }
    }
    void read_node_voltage(const string& name)
    {
        for (Node* n: nodes)
        {
            if (n->get_name() == name)
            {
                cout << n->get_name() << " voltage = " << n->get_voltage() << " Volts" << endl;
                return;
            }
        }
    }
    void add_ground(const string& name)
    {
        for (Node* n: nodes)
        {
            if (n->get_name() == name)
            {
                ground = n;
                return;
            }
        }
    }
};

// Int Main-------------------------------------------------------------------------------------------------------------

int main()
{
    Controller controller;
    string command;
    smatch match;
    while (true)
    {
        getline(cin, command);
        if (command == "end")
            return 0;
        else if (regex_match(command, match, add_node_pattern))
            controller.add_node(match[1]);
        else if (regex_match(command, match, add_resistor_pattern))
            controller.add_resistor(match[2], stof(match[1]), match[3], match[4]);
        else if (regex_match(command, match, add_VS_pattern))
            controller.add_VS("My_VS", stof(match[1]), match[2], match[3]);
        else if (regex_match(command, match, add_ground_pattern))
            controller.add_ground(match[1]);
        else if (regex_match(command, match, read_current_pattern))
            controller.read_current(match[1]);
        else if (regex_match(command, match, read_voltage_pattern))
            controller.read_voltage(match[1]);
        else if (regex_match(command, match, read_node_V_pattern))
            controller.read_node_voltage(match[1]);
    }
}