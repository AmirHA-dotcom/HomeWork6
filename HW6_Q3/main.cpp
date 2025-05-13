#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

// Regex----------------------------------------------------------------------------------------------------------------

regex add_student_pattern(R"(^Add student (\w+) (\w+) (\d[10]) (\d[10]) (\w+) (\d[9]) (\d[4])$)");
regex add_professor_pattern(R"(^Add professor (\w+) (\w+) (\d(10)) (\d(10)) (\w+) (\d+)$)");
regex add_TA_pattern(R"(^Add TA (\w+) (\w+) (\d(10)) (\d(10)) (\w+) (\d(9)) (\d(4)) (\w+)$)");
regex create_class_pattern("^Create course (\\w+)$");
regex add_student_to_course_pattern(R"(^Enroll student (\w+) (\w+) (\d(9)) in course (//w+)$)");
regex assign_professor_pattern(R"(^Assign professor (\w+) (\w+) (\d(9)) to course (\w+)$)");
regex assign_course_grade_pattern(R"(^Assign grade (\w+) for course (\w+) for student (\w+) (\w+) (\d(9)) $)");
regex show_student_info_pattern(R"(^Find student (\w+) (\w+) (\d(9))$)");
regex show_professor_info_pattern(R"(^Find professor (\w+) (\w+) (\d(10))$)");
regex show_course_info_pattern("^Find course (\\w+)$");
regex show_course_students_pattern("^List students in course (\\w+)$");
regex assign_assistant_to_course_pattern(R"(^Assign assistant (\w+) (\w+) (\d(9)) to course (\w+)$)");

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
    vector<Course*> courses;
public:
    void add_student(string first_name, string last_name, string national_ID, string phone, string major, string student_ID, string year)
    {
        students.push_back(new Student(first_name, last_name, national_ID, phone, major, student_ID, year));
    cout << "Student added successfully!" << endl;
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
        else
            cout << "OOPs, something went wrong!" << endl;
    }
}