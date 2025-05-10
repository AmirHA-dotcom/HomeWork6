#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

enum food_type {t_Drink, t_Dessert, t_Main};

// Regex----------------------------------------------------------------------------------------------------------------

regex add_drink_pattern(R"(^add drink (\w+) (\w+) (\w+)$)");
regex add_dessert_pattern(R"(^add dessert (\w+) (\w+) (\w+)$)");
regex add_main_dish_pattern(R"(^add main (\w+) (\w+) (\w+)$)");
regex print_final_cost_pattern("^print (\\w+)$");
regex print_total_cost_pattern("^sum$");
regex remove_item_pattern("^delete (\\w+)$");
smatch match;

// Model----------------------------------------------------------------------------------------------------------------

class Food
{
protected:
string name;
int base_price;
public:
    Food (string& food_name, int price) : name(food_name), base_price(price) {}
    string get_name() {return name;}
    int get_base_price() const {return base_price;}
};

class Drink: public Food
{
private:
    int volume;
public:
    Drink (string& name, int price, int volume) : Food(name, price), volume(volume) {}
    int get_final_price () const {return base_price + volume/10;}
};

class Dessert: public Food
{
private:
    int calories;
public:
    Dessert (string& name, int price, int calories) : Food(name, price), calories(calories) {}
    int get_final_price () const {return base_price + calories/50;}
};

class Main_Dish: public Food
{
private:
    int weight;
public:
    Main_Dish(string& name, int price, int weight) : Food(name, price), weight(weight) {}
    int get_final_price () const {return base_price + weight/20;}
};

// Controller-----------------------------------------------------------------------------------------------------------

class Controller
{
private:
    vector<pair<Food*, food_type>> foods;
    int food_already_exists_index(const string& name)
    {
        if (foods.empty())
            return -1;

        int index = -1;
        for (const auto& food: foods)
        {
            index++;
            if (food.first->get_name() == name)
            {
                return index;
            }
        }
        return -1;
    }
public:
    void add_drink (string name, int base_price, int volume)
    {
        if (food_already_exists_index(name) != -1)
        {
            cout << "Item already exists." << endl;
            return;
        }
        foods.push_back({new Drink(name, base_price, volume), t_Drink});
        cout << name << " added!" << endl;
    }

    void add_dessert (string name, int base_price, int calories)
    {
        if (food_already_exists_index(name) != -1)
        {
            cout << "Item already exists." << endl;
            return;
        }
        foods.push_back({new Dessert(name, base_price, calories), t_Dessert});
        cout << name << " added!" << endl;
    }

    void add_main_dish (string name, int base_price, int weight)
    {
        if (food_already_exists_index(name) != -1)
        {
            cout << "Item already exists." << endl;
            return;
        }
        foods.push_back({new Main_Dish(name, base_price, weight), t_Main});
        cout << name << " added!" << endl;
    }

    void print_final_price(string name)
    {
        int index = food_already_exists_index(name);
        if (index == -1)
        {
            cout << "Item doesn't exist." << endl;
            return;
        }

    }
};

// Int Main-------------------------------------------------------------------------------------------------------------

int main()
{
    Controller controller;
    string command;
    while (true)
    {
        getline(cin, command);

        if (command == "end")
            return 0;

        else if (regex_match(command, match, add_drink_pattern))
        {
            controller.add_drink(match[1], stoi(match[2]), stoi(match[3]));
        }

        else if (regex_match(command, match, add_dessert_pattern))
        {
            controller.add_dessert(match[1], stoi(match[2]), stoi(match[3]));
        }

        else if (regex_match(command, match, add_main_dish_pattern))
        {
            controller.add_main_dish(match[1], stoi(match[2]), stoi(match[3]));
        }

        else if (regex_match(command, match, print_final_cost_pattern))
        {

        }

        else if (regex_match(command, match, print_total_cost_pattern))
        {

        }

        else if (regex_match(command, match, remove_item_pattern))
        {

        }
    }
}