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

enum Type {student, professor, assistant};

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
    vector<string> assigned_courses;
    string get_department() {return department;}
    int get_salary() {return salary;}
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
    vector<string> assigned_courses;
    string get_role() {return role;}
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
private:
    vector<pair<Person*, Type>> people;
    vector<Course*> courses;
    int find_person_index(Person* person, Type type)
    {
        if (type == professor)
        {
            for (int i = 0; i < people.size(); i++)
            {
                if (people[i].second == professor)
                {
                    if (people[i].first->get_first_name() == person->get_first_name() &&
                            people[i].first->get_last_name() == person->get_last_name() &&
                            people[i].first->get_national_ID() == person->get_national_ID())
                        return i;
                }
            }
        }
        else
        {
            Student* student1 = dynamic_cast <Student*> (person);
            for (int i = 0; i < people.size(); i++)
            {
                if (people[i].second == student || people[i].second == assistant)
                {
                    Student* student2 = dynamic_cast <Student*> (people[i].first);
                    if (student2->get_first_name() == student1->get_first_name() &&
                            student2->get_last_name() == student1->get_last_name() &&
                            student2->get_student_ID() == student1->get_student_ID())
                        return i;
                }
            }
        }
        return -1;
    }
    int find_course_index(string course_name)
    {
        for (int i = 0; i < courses.size(); i++)
        {
            if (courses[i]->get_name() == course_name)
                return i;
        }
        return -1;
    }
public:
    void add_student(string first_name, string last_name, string national_ID, string phone, string specialization, string student_ID, string year)
    {
        Student* new_student = new Student(first_name, last_name, national_ID, phone, specialization, student_ID, year);
        Person* new_person =  new_student;
        int person_index = find_person_index(new_person, student);
        if (person_index != -1)
        {
            delete new_person;
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        people.emplace_back(new_person, student);
        cout << "Student added successfully!" << endl;
    }
    void add_professor(string first_name, string last_name, string national_ID, string phone, string department, string salary)
    {
        Professor*  new_professor = new Professor(first_name, last_name, national_ID, phone, department, salary);
        Person* new_person =  new_professor;
        int person_index = find_person_index(new_person, professor);
        if (person_index != -1)
        {
            delete new_person;
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        people.emplace_back(new_person, professor);
        cout << "Professor added successfully!" << endl;
    }
    void add_TA(string first_name, string last_name, string national_ID, string phone, string specialization, string student_ID, string year, string role)
    {
        Assistant* new_assistant = new Assistant(first_name, last_name, national_ID, phone, specialization, student_ID, year, role);
        Person* new_person =  new_assistant;
        int person_index = find_person_index(new_person, assistant);
        if (person_index != -1)
        {
            delete new_person;
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        people.emplace_back(new_person, assistant);
        cout << "Assistant added successfully!" << endl;
    }
    void add_course(string course_name)
    {

    }
    void add_student_to_course(string first_name, string last_name, string student_ID, string course_name)
    {

    }
    void assign_professor_to_course(string first_name, string last_name, string national_ID, string course_name)
    {

    }
    void show_student_info(string first_name, string last_name, string student_ID)
    {
        Student* new_student = new Student(first_name, last_name, (string &) "", (string &)"", (string &)"", student_ID, (string &)"");
        Person* new_person =  new_student;
        int person_index = find_person_index(new_person, student);
        if (person_index == -1)
        {
            delete new_person;
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        new_student = dynamic_cast <Student*> (people[person_index].first);
        cout << "Name: " << new_student->get_first_name() << " " << new_student->get_last_name() << endl;
        cout << "National ID: " << new_student->get_national_ID() << endl;
        cout << "Phone number: " << new_student->get_phone_number() << endl;
        cout << "Student ID: " << new_student->get_student_ID() << endl;
        cout << "Major: " << new_student->get_specialization() << endl;
        cout << "Entrance Year: " << new_student->get_entrance_year() << endl;
        cout << "GPA: " << new_student->get_GPA() << endl;
        cout << "Courses:" << endl;
        if (new_student->courses.empty())
            cout << "This student is not enrolled in any course." << endl;
        else
        {
            for (const auto& course: new_student->courses)
            {
                if (course.second == 0)
                    cout << course.first << ": [Not Graded]" << endl;
                else
                    cout << course.first << ": " << course.second << endl;
            }
        }
        if (people[person_index].second == assistant)
        {
            Assistant* new_assistant = dynamic_cast <Assistant*> (people[person_index].first);
            cout << "Role: " << new_assistant->get_role() << endl;
            cout << "Assigned Courses:" << endl;
            if (new_assistant->assigned_courses.empty())
                cout << "This student is not assistant in any course." << endl;
            else
                for (const auto& course: new_assistant->assigned_courses)
                    cout << course << endl;
        }
    }
    void show_professor_info(string first_name, string last_name, string national_ID)
    {
        Professor* new_professor = new Professor(first_name, last_name, national_ID, (string &) "", (string &) "",(string &) "");
        Person* new_person =  new_professor;
        int person_index = find_person_index(new_person, professor);
        if (person_index == -1)
        {
            delete new_person;
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        new_professor = dynamic_cast <Professor*> (people[person_index].first);
        cout << "Name: " << new_professor->get_first_name() << " " << new_professor->get_last_name() << endl;
        cout << "National ID: " << new_professor->get_national_ID() << endl;
        cout << "Phone number: " << new_professor->get_phone_number() << endl;
        cout << "Department: " << new_professor->get_department() << endl;
        cout << "Salary: " << new_professor->get_salary() << endl;
        cout << "Assigned Courses:" << endl;
        if (new_professor->assigned_courses.empty())
            cout << "" << endl;
        else
        {
            for (const auto& course: new_professor->assigned_courses)
                cout << course << endl;
        }
    }
    void show_course_info(string course_name)
    {
        int course_index = find_course_index(course_name);
        if (course_index == -1)
        {
            cout << "OOPs, something went wrong!" << endl;
            return;
        }
        Course* course = courses[course_index];
        cout << "Course: " << course->get_name() << endl;
        if (course->get_professor() == nullptr)
            cout << "[None]" << endl;
        else
            cout << "Instructor: " << course->get_professor()->get_first_name() << " " << course->get_professor()->get_last_name() << " " << course->get_professor()->get_national_ID() << endl;
        cout << "Assistants:" << endl;
        if (course->get_assistants().empty())
            cout << "There are no assistants for this course." << endl;
        else
        {
            for (Assistant *assistant: course->get_assistants())
                cout << assistant->get_first_name() << " " << assistant->get_last_name() << " " << assistant->get_student_ID() << endl;
        }
        cout << "Number of participants: " << course->get_students().size() << endl;
    }
};

// Helper Functions-----------------------------------------------------------------------------------------------------

string name_modifier(string name)
{
    for (char & i: name)
    {
        if (i == '_')
            i = ' ';
    }
    return name;
}

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
            if (match[6].matched) specialization += " " + match[6].str();
            if (match[7].matched) specialization += " " + match[7].str();
            string first_name = name_modifier(match[1]);
            string last_name = name_modifier(match[2]);
            AHA.add_student(first_name, last_name, match[3], match[4], specialization, match[8], match[9]);
        }
        else if (regex_match(command, match, add_professor_pattern))
        {
            string department = match[5];
            if (match[6].matched) department += " " + match[6].str();
            if (match[7].matched) department += " " + match[7].str();
            string first_name = name_modifier(match[1]);
            string last_name = name_modifier(match[2]);
            AHA.add_professor(first_name, last_name, match[3], match[4], match[5], match[8]);
        }
        else if (regex_match(command, match, add_TA_pattern))
        {
            string specialization = match[5];
            if (match[6].matched) specialization += " " + match[6].str();
            if (match[7].matched) specialization += " " + match[7].str();
            string first_name = name_modifier(match[1]);
            string last_name = name_modifier(match[2]);
            AHA.add_TA(first_name, last_name, match[3], match[4], match[5], match[8], match[9], match[10]);
        }
        else if (regex_match(command, match, create_class_pattern))
        {
            string course_name = match[1];
            if (match[2].matched) course_name += " " + match[2].str();
            if (match[3].matched) course_name += " " + match[3].str();
            AHA.add_course(course_name);
        }
        else if (regex_match(command, match, show_student_info_pattern))
        {
            string first_name = name_modifier(match[1]);
            string last_name = name_modifier(match[2]);
            AHA.show_student_info(first_name, last_name, match[3]);
        }
        else if (regex_match(command, match, show_professor_info_pattern))
        {
            string first_name = name_modifier(match[1]);
            string last_name = name_modifier(match[2]);
            AHA.show_professor_info(first_name, last_name, match[3]);
        }
        else if (regex_match(command, match, show_course_info_pattern))
        {
            string course_name = match[1];
            if (match[2].matched) course_name += " " + match[2].str();
            if (match[3].matched) course_name += " " + match[3].str();
            AHA.show_course_info(course_name);
        }
        else
            cout << "OOPs, something went wrong!" << endl;
    }
}