// ProjectTwo.cpp
// Adrienne Ayala
// CS 300
// This program reads course information from a CSV file and organizes it in a binary search tree. 
//It allows users to load the data, print a list of courses, and view details of specific courses, including their prerequisites.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

//initalize vector
static vector<string> splitCSV(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}
//initalize struct
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};
//initalize node
struct Node {
    Course course;
    Node* left;
    Node* right;

    explicit Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
};

//  Binary Search Tree implementation
class BinarySearchTree {
private:
    Node* root = nullptr;

    void addNode(Node* node, const Course& course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) node->left = new Node(course);
            else addNode(node->left, course);
        } else if (course.courseNumber > node->course.courseNumber) {
            if (node->right == nullptr) node->right = new Node(course);
            else addNode(node->right, course);
        } else {
            node->course = course;
        }
    }
//alphabetical order 
    void inOrder(Node* node) const {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.title << "\n";
        inOrder(node->right);
    }
//searches for course number
    Node* searchNode(Node* node, const string& key) const {
        Node* current = node;
        while (current != nullptr) {
            if (key == current->course.courseNumber) return current;
            if (key < current->course.courseNumber) current = current->left;
            else current = current->right;
        }
        return nullptr;
    }
//destroys tree
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ~BinarySearchTree() { clear(); }
//clears tree
    void clear() {
        destroy(root);
        root = nullptr;
    }
// checks if tree is empty
    bool empty() const { return root == nullptr; }
//inserts course into tree
    void insert(const Course& course) {
        if (root == nullptr) root = new Node(course);
        else addNode(root, course);
    }
//prints course list
    void printCourseList() const {
        if (empty()) {
            cout << "Please load the data structure first (option 1).\n";
            return;
        }
        cout << "Here is a sample schedule:\n";
        inOrder(root);
    }
//prints course details
    bool printCourse(const string& courseNumberInput) const {
        if (empty()) {
            cout << "Please load the data structure first (option 1).\n";
            return false;
        }

        string key = courseNumberInput;
        Node* found = searchNode(root, key);

        if (found == nullptr) {
            cout << "Course " << key << " not found.\n";
            return false;
        }

        const Course& c = found->course;
        cout << c.courseNumber << ", " << c.title << "\n";

       // Finds pre reqs by commas
        if (c.prerequisites.empty()) {
            cout << "Prerequisites: None\n";
        } else {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < c.prerequisites.size(); ++i) {
                cout << c.prerequisites[i];
                if (i + 1 < c.prerequisites.size()) cout << ", ";
            }
            cout << "\n";
        }
        return true;
    }
};

// Loads courses from a CSV file into the BST
static bool loadCourses(const string& fileName, BinarySearchTree& bst) {
    ifstream infile(fileName);
    if (!infile.is_open()) {
        cout << "No file with that name, try again\n";
        return false;
    }

    bst.clear();

    string line;
    int count = 0;

    while (getline(infile, line)) {
        if (line.size() == 0) continue;

        vector<string> t = splitCSV(line);
        if (t.size() < 2) continue;

        Course c;
        c.courseNumber = t[0];
        c.title = t[1];

        for (size_t i = 2; i < t.size(); ++i) {
            if (!t[i].empty()) c.prerequisites.push_back(t[i]);
        }

        bst.insert(c);
        count++;
    }

    cout << "Data Structure Loaded!\n";
    return count > 0;
}

//main menu loop
int main() {
    BinarySearchTree bst;
    bool loaded = false; // makes sure data is loaded before printing course list or details

    cout << "Course Planner.\n";

    while (true) {
        // Menu
        cout << "1. Load Data Structure. \n";
        cout << "2. Print Course List. \n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n";
        cout << "Please pick an option ";

        string choiceLine;
        getline(cin, choiceLine);

        int choice = -1;
        try { choice = stoi(choiceLine); }
        catch (...) { choice = -1; }

        if (choice == 1) {
            cout << "Enter input filename: ";
            string fileName;
            getline(cin, fileName);
            loaded = loadCourses(fileName, bst);
        }
        else if (choice == 2) {
            if (!loaded) cout << "Please load the data structure first (option 1).\n";
            else bst.printCourseList();
        }
        else if (choice == 3) {
            if (!loaded) {
                cout << "Please load the data structure first (option 1).\n";
            } else {
                cout << "What course do you want to know about? ";
                string courseNum;
                getline(cin, courseNum);
                bst.printCourse(courseNum);
            }
        }
        else if (choice == 9) {
            cout << "Exiting!\n";
            break;
        }
        else {
            cout << choiceLine << " is not a valid option.\n";
        }
    }

    return 0;
}