#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

// Regex----------------------------------------------------------------------------------------------------------------

regex add_student_pattern(R"(^Add student (\S+) (\S+) (\d{10}) (\d{10}) (\S+) (\d{9}) (\d{4})$)");
regex add_professor_pattern(R"(^Add professor (\S+) (\S+) (\d{10}) (\d{10}) (\S+) (\d+)$)");
regex add_TA_pattern(R"(^Add TA (\S+) (\S+) (\d{10}) (\d{10}) (\S+) (\d{9}) (\d{4}) (\S+)$)");
regex create_class_pattern("^Create course (\\S+)$");
regex add_student_to_course_pattern(R"(^Enroll student (\S+) (\S+) (\d{9}) in course (/S+)$)");
regex assign_professor_pattern(R"(^Assign professor (\S+) (\S+) (\d{9}) to course (\S+)$)");
regex assign_course_grade_pattern(R"(^Assign grade (\S+) for course (\S+) for student (\S+) (\S+) (\d{9}) $)");
regex show_student_info_pattern(R"(^Find student (\S+) (\S+) (\d{9})$)");
regex show_professor_info_pattern(R"(^Find professor (\S+) (\S+) (\d{10})$)");
regex show_course_info_pattern("^Find course (\\S+)$");
regex show_course_students_pattern("^List students in course (\\w+)$");
regex assign_assistant_to_course_pattern(R"(^Assign assistant (\S+) (\S+) (\d{9}) to course (\S+)$)");

// Model----------------------------------------------------------------------------------------------------------------

class Person
{
protected:
    string first_name;
    string last_name;
    string national_ID;
    string phone_number;
public:
    Person(string& fname, string& lname, string& n_ID, string& phone_NO) : first_name(fname), last_name(lname), national_ID(n_ID), phone_number(phone_NO) {}
    virtual void display_info() = 0;
};

class Student : public Person
{
protected:
    string major;
    string student_ID;
    string entrance_year;
public:
    Student(string& fname, string& lname, string& n_ID, string& phone_NO, string& major_, string& s_ID, string& e_year) : Person(fname, lname, n_ID, phone_NO), major(major_), student_ID(s_ID), entrance_year(e_year) {}
    void display_info() override
    {

    }
};

class Professor : public Person
{
private:
    string department;
    int salary;
public:
    Professor(string& fname, string& lname, string& n_ID, string& phone_NO, string& dep, string& salary_) : Person(fname, lname, n_ID, phone_NO), department(dep), salary(stoi(salary_)) {}
    void display_info() override
    {

    }
};

class Assistant : public Student
{
private:
    string role;
public:
    Assistant(string& fname, string& lname, string& n_ID, string& phone_NO, string& major_, string& s_ID, string& e_year, string& role_) : Student(fname, lname, n_ID, phone_NO, major_, s_ID, e_year), role(role_) {}
    void display_info() override
    {

    }
};

class Course
{
private:
    string course_name;
    Professor* instructor;
    Assistant* assistant;
    vector<Student*> students;
public:
    void display_course_info()
    {

    }
};

// Controller-----------------------------------------------------------------------------------------------------------

class Controller
{
private:
    vector<Student*> students;
    vector<Professor*> professors;
    vector<Assistant*> assistants;
    vector<Course*> courses;
public:
    void add_student(string first_name, string last_name, string national_ID, string phone, string major, string student_ID, string year)
    {
        students.push_back(new Student(first_name, last_name, national_ID, phone, major, student_ID, year));
        cout << "Student added successfully!" << endl;
    }
    void add_professor(string first_name, string last_name, string national_ID, string phone, string department, string salary)
    {
        professors.push_back(new Professor(first_name, last_name, national_ID, phone, department, salary));
        cout << "professor added successfully!" << endl;
    }
    void add_TA(string first_name, string last_name, string national_ID, string phone, string major, string student_ID, string year, string role)
    {
        assistants.push_back(new Assistant(first_name, last_name, national_ID, phone, major, student_ID, year, role));
        cout << "Assistant added successfully!" << endl;
    }
};

// Int Main-------------------------------------------------------------------------------------------------------------

int main()
{
    Controller AHA;
    string command;
    smatch match;
    while (true)
    {
        getline(cin, command);
        if(command == "exit")
            return 0;
        else if (regex_match(command, match, add_student_pattern))
            AHA.add_student(match[1], match[2], match[3], match[4], match[5], match[6], match[7]);
        else if (regex_match(command, match, add_professor_pattern))
            AHA.add_professor(match[1], match[2], match[3], match[4], match[5], match[6]);
        else if (regex_match(command, match, add_TA_pattern))
            AHA.add_TA(match[1], match[2], match[3], match[4], match[5], match[6], match[7], match[8]);
        else
            cout << "OOPs, something went wrong!" << endl;
    }
}