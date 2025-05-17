#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

// Regex----------------------------------------------------------------------------------------------------------------

regex add_student_pattern(R"(^Add student (\S+) (\S+) (\d{10}) (\d{10}) (\S+)(?: (\S+))?(?: (\S+))? (\d{9}) (\d{4})$)");
regex add_professor_pattern(R"(^Add professor (\S+) (\S+) (\d{10}) (\d{10}) (\S+)(?: (\S+))?(?: (\S+))? (\d+)$)");
regex add_TA_pattern(R"(^Add TA (\S+) (\S+) (\d{10}) (\d{10}) (\S+)(?: (\S+))?(?: (\S+))? (\d{9}) (\d{4}) (\S+)$)");
regex create_class_pattern(R"(^Create course (\S+)(?: (\S+))?(?: (\S+))?$)");
regex add_student_to_course_pattern(R"(^Enroll student (\S+) (\S+) (\d{9}) in course (\S+)(?: (\S+))?(?: (\S+))?$)");
regex assign_professor_pattern(R"(^Assign professor (\S+) (\S+) (\d{9}) to course (\S+)(?: (\S+))?(?: (\S+))?$)");
regex assign_course_grade_pattern(R"(^Assign grade (\S+) for course (\S+)(?: (\S+))?(?: (\S+))? for student (\S+) (\S+) (\d{9}) $)");
regex show_student_info_pattern(R"(^Find student (\S+) (\S+) (\d{9})$)");
regex show_professor_info_pattern(R"(^Find professor (\S+) (\S+) (\d{10})$)");
regex show_course_info_pattern(R"(^Find course (\S+)(?: (\S+))?(?: (\S+))?$)");
regex show_course_students_pattern(R"(^List students in course (\S+)(?: (\S+))?(?: (\S+))?$)");
regex assign_assistant_to_course_pattern(R"(^Assign assistant (\S+) (\S+) (\d{9}) to course (\S+)(?: (\S+))?(?: (\S+))?$)");

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
    string get_first_name() {return first_name;}
    string get_last_name() {return last_name;}
    string get_national_ID() {return national_ID;}
    string get_phone_number() {return phone_number;}
};

class Student : public Person
{
protected:
    string specialization;
    string student_ID;
    string entrance_year;
    float GPA;
public:
    Student(string& fname, string& lname, string& n_ID, string& phone_NO, string& specialization_, string& s_ID, string& e_year) : Person(fname, lname, n_ID, phone_NO), specialization(specialization_), student_ID(s_ID), entrance_year(e_year) {GPA = 0.0f;}
    string get_specialization() {return specialization;}
    string get_student_ID() {return student_ID;}
    string get_entrance_year() {return entrance_year;}
    float get_GPA() {return GPA;}
    vector<pair<string, float>> courses;
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
    Assistant(string& fname, string& lname, string& n_ID, string& phone_NO, string& specialization_, string& s_ID, string& e_year, string& role_) : Student(fname, lname, n_ID, phone_NO, specialization_, s_ID, e_year), role(role_) {}
    void display_info() override
    {

    }
};

class Course
{
private:
    string course_name;
    Professor* instructor;
    vector<Assistant*> assistants;
    vector<Student*> students;
public:
    Course(string& c_name) : course_name(c_name) {instructor = nullptr;}
    string get_name() {return course_name;}
    void add_student(Student* student)
    {
        students.push_back(student);
    }
    void assign_professor(Professor* professor)
    {
        instructor = professor;
    }
    void add_assistant(Assistant* assistant)
    {
        assistants.push_back(assistant);
    }
    vector<Assistant*> get_assistants() {return assistants;}
    Professor* get_professor() {return instructor;}
    vector<Student*> get_students() {return students;}
    void display_course_info()
    {

    }
};

// Controller-----------------------------------------------------------------------------------------------------------

class Controller
{
    vector<Student*> students;
    vector<Professor*> professors;
    vector<Assistant*> assistants;
    vector<Course*> courses;
public:
    void add_student(string first_name, string last_name, string national_ID, string phone, string specialization, string student_ID, string year)
    {
        int student_index = -1;
        for (int i = 0; i < students.size(); i++)
        {
            if (students[i]->get_first_name() == first_name && students[i]->get_last_name() == last_name && students[i]->get_student_ID() == student_ID)
            {
                student_index = i;
                break;
            }
        }
        if (student_index != -1)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        students.push_back(new Student(first_name, last_name, national_ID, phone, specialization, student_ID, year));
        cout << "Student added successfully!" << endl;
    }
    void add_professor(string first_name, string last_name, string national_ID, string phone, string department, string salary)
    {
        professors.push_back(new Professor(first_name, last_name, national_ID, phone, department, salary));
        cout << "professor added successfully!" << endl;
    }
    void add_TA(string first_name, string last_name, string national_ID, string phone, string specialization, string student_ID, string year, string role)
    {
        assistants.push_back(new Assistant(first_name, last_name, national_ID, phone, specialization, student_ID, year, role));
        cout << "Assistant added successfully!" << endl;
    }
    void add_course(string course_name)
    {
        courses.push_back(new Course(course_name));
        cout << "Course created successfully!" << endl;
    }
    void add_student_to_course(string first_name, string last_name, string student_ID, string course_name)
    {
        int course_index = -1;
        for (int i = 0; i < courses.size(); i++)
        {
            if (courses[i]->get_name() == course_name)
            {
                course_index = i;
                break;
            }
        }
        if (course_index == -1)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        for (Student* student: courses[course_index]->get_students())
        {
            if (student->get_first_name() == first_name && student->get_last_name() == last_name && student->get_student_ID() == student_ID)
            {
                cout << "OOPs, something went wrong!" << endl;
                return;
            }
        }
        int student_index = -1;
        for (int i = 0; i < students.size(); i++)
        {
            if (students[i]->get_first_name() == first_name && students[i]->get_last_name() == last_name && students[i]->get_student_ID() == student_ID)
            {
                student_index = i;
                break;
            }
        }
        if (student_index == -1)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        courses[course_index]->add_student(students[student_index]);
        students[student_index]->courses.emplace_back(course_name, 0.0f);
        cout << "Student enrolled in the course successfully!" << endl;
    }
    void assign_professor_to_course(string first_name, string last_name, string national_ID, string course_name)
    {
        int course_index = -1;
        for (int i = 0; i < courses.size(); i++)
        {
            if (courses[i]->get_name() == course_name)
            {
                course_index = i;
                break;
            }
        }
        if (course_index == -1)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        if (courses[course_index]->get_professor() == nullptr)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        int professor_index = -1;
        for (int i = 0; i < professors.size(); i++)
        {
            if (professors[i]->get_first_name() == first_name && professors[i]->get_last_name() == last_name && professors[i]->get_national_ID() == national_ID)
            {
                professor_index = i;
            }
        }
        if (professor_index == -1)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        courses[course_index]->assign_professor(professors[professor_index]);
        cout << "Professor assigned to the course successfully!" << endl;
    }
    void show_student_info(string first_name, string last_name, string student_ID)
    {
        int student_index = -1;
        for (int i = 0; i < students.size(); i++)
        {
            if (students[i]->get_first_name() == first_name && students[i]->get_last_name() == last_name && students[i]->get_student_ID() == student_ID)
            {
                student_index = i;
                break;
            }
        }
        int assistant_index = -1;
        for (int i = 0; i < assistants.size(); i++)
        {
            if (assistants[i]->get_first_name() == first_name && assistants[i]->get_last_name() == last_name && assistants[i]->get_student_ID() == student_ID)
            {
                assistant_index = i;
                break;
            }
        }
        if (student_index == -1 && assistant_index == -1)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        if (student_index != -1)
        {
            Student* student  = students[student_index];
            cout << "Name: " << student->get_first_name() << " " << student->get_last_name() << endl;
            cout << "National ID: " << student->get_national_ID() << endl;
            cout << "Phone number: " << student->get_phone_number() << endl;
            cout << "Student ID: " << student->get_student_ID() << endl;
            cout << "Major: " << student->get_specialization() << endl;
            cout << "Entrance Year: " << student->get_entrance_year() << endl;
            cout << "GPA: " << student->get_GPA() << endl;
            cout << "Courses:" << endl;
            for (const auto& course: student->courses)
            {
                if (course.second == 0.0f)
                    cout << course.first << ": " << "[Not Graded]" << endl;
                else
                    cout << course.first << ": " << course.second << endl;
            }
        }
        else if (assistant_index != -1)
        {
            Assistant* assistant  = assistants[assistant_index];
            cout << "Name: " << assistant->get_first_name() << " " << assistant->get_last_name() << endl;
            cout << "National ID: " << assistant->get_national_ID() << endl;
            cout << "Phone number: " << assistant->get_phone_number() << endl;
            cout << "Student ID: " << assistant->get_student_ID() << endl;
            cout << "Major: " << assistant->get_specialization() << endl;
            cout << "Entrance Year: " << assistant->get_entrance_year() << endl;
            cout << "GPA: " << assistant->get_GPA() << endl;
            cout << "Courses:" << endl;

        }
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
        {
            string specialization = match[5];
            if (match[6] != "")
                specialization += " ";
            specialization += match[6];
            if (match[7] != "")
                specialization += " ";
            specialization += match[7];
            AHA.add_student(match[1], match[2], match[3], match[4], specialization, match[8], match[9]);
        }
        else if (regex_match(command, match, add_professor_pattern))
        {
            string department = match[5];
            if (match[6] != "")
                department += " ";
            department += match[6];
            if (match[7] != "")
                department += " ";
            department += match[7];
            AHA.add_professor(match[1], match[2], match[3], match[4], match[5], match[8]);
        }
        else if (regex_match(command, match, add_TA_pattern))
        {
            string specialization = match[5];
            if (match[6] != "")
                specialization += " ";
            specialization += match[6];
            if (match[7] != "")
                specialization += " ";
            specialization += match[7];
            AHA.add_TA(match[1], match[2], match[3], match[4], match[5], match[8], match[9], match[10]);
        }
        else if (regex_match(command, match, create_class_pattern))
        {
            string course_name = match[1];
            if (match[2] != "")
                course_name += " ";
            course_name += match[2];
            if (match[3] != "")
                course_name += " ";
            course_name += match[3];
            AHA.add_course(course_name);
        }
        else if (regex_match(command, match, show_student_info_pattern))
        {

        }
        else
            cout << "OOPs, something went wrong!" << endl;
    }
}