#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

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
    vector <Drink*> drinks;
    vector <Dessert*> desserts;
    vector <Main_Dish*> main_dishes;
    int drink_already_exists_index(const string& name)
    {
        if (drinks.empty())
            return -1;

        int index = -1;
        for (Drink* food: drinks)
        {
            index++;
            if (food->get_name() == name)
            {
                return index;
            }
        }
        return -1;
    }
    int dessert_already_exists_index(const string& name)
    {
        if (desserts.empty())
            return -1;

        int index = -1;
        for (Dessert* food: desserts)
        {
            index++;
            if (food->get_name() == name)
            {
                return index;
            }
        }
        return -1;
    }
    int main_dish_already_exists_index(const string& name)
    {
        if (main_dishes.empty())
            return -1;

        int index = -1;
        for (Main_Dish* food: main_dishes)
        {
            index++;
            if (food->get_name() == name)
            {
                return index;
            }
        }
        return -1;
    }
public:
    void add_drink (string name, int base_price, int volume)
    {
        if (drink_already_exists_index(name) != -1)
        {
            cout << "Item already exists." << endl;
            return;
        }
        drinks.push_back(new Drink(name, base_price, volume));
        cout << name << " added!" << endl;
    }

    void add_dessert (string name, int base_price, int calories)
    {
        if (dessert_already_exists_index(name) != -1)
        {
            cout << "Item already exists." << endl;
            return;
        }
        desserts.push_back(new Dessert(name, base_price, calories));
        cout << name << " added!" << endl;
    }

    void add_main_dish (string name, int base_price, int weight)
    {
        if (main_dish_already_exists_index(name) != -1)
        {
            cout << "Item already exists." << endl;
            return;
        }
        main_dishes.push_back(new Main_Dish(name, base_price, weight));
        cout << name << " added!" << endl;
    }

    void print_final_price(string name)
    {
        int drink_index = drink_already_exists_index(name);
        int dessert_index = dessert_already_exists_index(name);
        int main_dish_index = main_dish_already_exists_index(name);
        if (drink_index == -1 && dessert_index == -1 && main_dish_index == -1)
        {
            cout << "Item doesn't exist." << endl;
            return;
        }
        if (drink_index != -1)
        {
            cout << name << ": " << floor(drinks[drink_index]->get_final_price()) << endl;
            return;
        }
        else if (dessert_index != -1)
        {
            cout << name << ": " << floor(desserts[dessert_index]->get_final_price()) << endl;
            return;
        }
        else
        {
            cout << name << ": " << floor(main_dishes[main_dish_index]->get_final_price()) << endl;
            return;
        }
    }

    void print_total_price()
    {
        int sum = 0;
        for (Drink* d: drinks)
            sum += d->get_final_price();
        for (Dessert* d: desserts)
            sum += d->get_final_price();
        for (Main_Dish* m: main_dishes)
            sum += m->get_final_price();
        cout << "Total: " << (int)sum << endl;
    }

    void remove_item(string name)
    {
        int drink_index = drink_already_exists_index(name);
        int dessert_index = dessert_already_exists_index(name);
        int main_dish_index = main_dish_already_exists_index(name);
        if (drink_index == -1 && dessert_index == -1 && main_dish_index == -1)
        {
            cout << "Item doesn't exist." << endl;
            return;
        }
        if (drink_index != -1)
            drinks.erase(drinks.begin() + drink_index);
        else if (dessert_index != -1)
            desserts.erase(desserts.begin() + dessert_index);

        else
            main_dishes.erase(main_dishes.begin() + main_dish_index);
        cout << name << " deleted." << endl;
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
            controller.print_final_price(match[1]);
        }

        else if (regex_match(command, match, print_total_cost_pattern))
        {
            controller.print_total_price();
        }

        else if (regex_match(command, match, remove_item_pattern))
        {
            controller.remove_item(match[1]);
        }
    }
}