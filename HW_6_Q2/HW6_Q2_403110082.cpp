#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <iomanip>

using namespace std;

// Regex----------------------------------------------------------------------------------------------------------------

regex add_node_pattern("add node (\\w+)\\s*");
regex add_resistor_pattern(R"(add resistor (\w+) (\w+) (\w+) (\w+)\s*)");
regex add_VS_pattern(R"(add voltage source (\w+) (\w+) (\w+)\s*)");
regex add_ground_pattern("add ground (\\w+)\\s*");
regex read_current_pattern("read current (\\w+)\\s*");
regex read_voltage_pattern("read voltage (\\w+)\\s*");
regex read_node_V_pattern("read node voltage (\\w+)\\s*");

// Model----------------------------------------------------------------------------------------------------------------

class Node
{
private:
    string name;
    int number;
    float voltage;
public:
    Node(string& tag, int NO) : name(tag), number(NO) {voltage = 0;}
    string get_name() const {return name;}
    void set_voltage(float v) {voltage = v;}
    float get_voltage() const {return voltage;}
};

class Component
{
protected:
    string name;
public:
    Node* node_1;
    Node* node_2;
    Component(string& tag, Node* first, Node* second) : name(tag), node_1(first), node_2(second) {}
    virtual float get_voltage() = 0;
    virtual float get_current() = 0;
};

class Voltage_Source : public Component
{
public:
    float voltage;
    float current;
    Voltage_Source(string& tag, Node* f, Node* s, float V) : Component(tag, f, s), voltage(V) {}
    string get_name() const {return name;}
    float get_voltage() override
    {
        return voltage;
    }
    float get_current() override
    {
        return current;
    }
};

class Resistor : public Component
{
public:
    float resistance;
    Resistor(string& tag, Node* f, Node* s, float R) : Component(tag, f, s), resistance(R) {}
    string get_name() const {return name;}
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
    float determinant(vector<vector<float>> matrix)
    {
        int n = matrix.size();
        float det = 1;
        for (int i = 0; i < n; i++)
        {
            int max_row = i;
            for (int k = i + 1; k < n; k++)
            {
                if (abs(matrix[k][i]) > abs(matrix[max_row][i]))
                {
                    max_row = k;
                }
            }
            if (max_row != i)
            {
                swap(matrix[i], matrix[max_row]);
                det *= -1;
            }
            // Gaussian elimination
            for (int j = i + 1; j < n; j++)
            {
                float factor = matrix[j][i] / matrix[i][i];
                for (int k = i; k < n; k++)
                {
                    matrix[j][k] -= factor * matrix[i][k];
                }
            }
            det *= matrix[i][i];
        }
        return det;
    }
    vector<float> solve_cramer(vector<vector<float>> G, vector<float> I) {
        int n = G.size();
        vector<float> solution(n);
        float det_G = determinant(G);

        for (int i = 0; i < n; i++) {
            auto G_temp = G;
            for (int j = 0; j < n; j++) {
                G_temp[j][i] = I[j];
            }
            float det_temp = determinant(G_temp);
            solution[i] = det_temp / det_G;
        }
        return solution;
    }
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
            if (n->get_name() == node_2)
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
            if (n->get_name() == node_2)
                N2 = n;
        }
        VS = new Voltage_Source(name, N1, N2, V);
    }
    void read_current(const string& name)
    {
        analyze_voltages();
        if (name == "VIN")
        {
            cout << "VIN current = " << fixed << setprecision(2) << VS->get_current() << " amps" << endl;
            return;
        }
        for (Resistor* R: resistors)
        {
            if (R->get_name() == name)
            {
                cout << R->get_name() << " current = " << fixed << setprecision(2) << R->get_current() << " amps" << endl;
                return;
            }
        }
    }
    void read_voltage(const string& name)
    {
        analyze_voltages();
        if (name == "VIN")
        {
            cout << "VIN voltage = " << fixed << setprecision(2) << VS->get_voltage() << " volts" << endl;
            return;
        }
        for (Resistor* R: resistors)
        {
            if (R->get_name() == name)
            {
                cout << R->get_name() << " voltage = " << fixed << setprecision(2) << R->get_voltage() << " volts" << endl;
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
                cout << n->get_name() << " voltage = " << fixed << setprecision(2) << n->get_voltage() << " volts" << endl;
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
    void analyze_voltages()
    {
        vector<Node*> non_ground_nodes;
        for (Node* node: nodes)
            if (node != ground)
                non_ground_nodes.push_back(node);
        int nodes_count = non_ground_nodes.size();
        vector<vector<float>> G(nodes_count + 1, vector<float>(nodes_count + 1, 0));
        vector<float> I(nodes_count + 1, 0);

        unordered_map<string, int> node_index;
        int index = 0;
        for (Node* node: non_ground_nodes)
            node_index[node->get_name()] = index++;

        for (Resistor* R: resistors)
        {
            Node* node_1 = R->node_1;
            Node* node_2 = R->node_2;
            float conductance = 1/R->resistance;
            if (node_1 != ground && node_2 != ground)
            {
                int i = node_index[node_1->get_name()];
                int j = node_index[node_2->get_name()];
                G[i][i] += conductance;
                G[j][j] += conductance;
                G[i][j] -= conductance;
                G[j][i] -= conductance;
            }
            else if (node_1 == ground)
            {
                int j = node_index[node_2->get_name()];
                G[j][j] += conductance;
            }
            else if (node_2 == ground)
            {
                int i = node_index[node_1->get_name()];
                G[i][i] += conductance;
            }
        }

        Node* node_1_VS = VS->node_1;
        Node* node_2_VS = VS->node_2;
        int vs_row = nodes_count;

        if (node_1_VS != ground)
        {
            int i = node_index[node_1_VS->get_name()];
            G[vs_row][i] = 1;
            G[i][vs_row] = 1;
        }

        if (node_2_VS != ground)
        {
            int j = node_index[node_2_VS->get_name()];
            G[vs_row][j] = -1;
            G[j][vs_row] = -1;
        }

        I[vs_row] = VS->voltage;

        vector<float> node_voltages = solve_cramer(G, I);
        for (int i = 0; i < nodes_count; i++)
            non_ground_nodes[i]->set_voltage(node_voltages[i]);
        ground->set_voltage(0);
        VS->current = -node_voltages[nodes_count];
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
            controller.add_VS("VIN", stof(match[1]), match[2], match[3]);
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