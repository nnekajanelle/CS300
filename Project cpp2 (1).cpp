// Project cpp2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

// Struct to represent a course
struct Course {
    std::string courseId;
    std::string courseTitle;
    std::vector<std::string> prerequisites;
};

// Function to read course data from file and store in a vector
std::vector<Course> readCourseData(const std::string& filename) {
    std::vector<Course> courses; // Vector to store course objects
    // Open the file
    std::ifstream file(filename);
    // Check if the file is opened successfully
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    // Read each line from the file
    std::string line;
    while (std::getline(file, line)) {
        // Use stringstream to parse the line into course attributes
        std::stringstream ss(line);
        Course course;
        // Assuming courseId, courseTitle, and prerequisites are separated by commas
        std::getline(ss, course.courseId, ',');
        std::getline(ss, course.courseTitle, ',');
        // Read prerequisites and split
        std::string prerequisite;
        while (std::getline(ss, prerequisite, ',')) {
            course.prerequisites.push_back(prerequisite);
        }
        // Add the course to the vector
        courses.push_back(course);
    }
    // Close the file
    file.close();
    return courses;
}

// Function to print course list in alphanumeric order
void printAlphanumericCourseList(const std::vector<Course>& courses) {
    std::vector<Course> sortedCourses = courses;
    // Sort the courses alphanumerically based on courseId
    std::sort(sortedCourses.begin(), sortedCourses.end(), [](const Course& a, const Course& b) {
        return a.courseId < b.courseId;
        });
    // Print the sorted list
    std::cout << "Alphanumeric Course List:" << std::endl;
    for (const auto& course : sortedCourses) {
        std::cout << "Course ID: " << course.courseId
            << ", Title: " << course.courseTitle << std::endl;
    }
}

// Function to print course information based on courseNumber
void printCourseInformation(const std::vector<Course>& courses, const std::string& courseNumber) {
    // Find the course with the specified courseNumber
    auto it = std::find_if(courses.begin(), courses.end(),
        [courseNumber](const Course& course) {
            return course.courseId == courseNumber;
        });
    // Check if the course is found
    if (it != courses.end()) {
        // Print course information
        std::cout << "Course ID: " << it->courseId
            << ", Title: " << it->courseTitle << std::endl;
        // Print prerequisites
        if (!it->prerequisites.empty()) {
            std::cout << "Prerequisites: ";
            for (const auto& prerequisite : it->prerequisites) {
                std::cout << prerequisite << " ";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << "No prerequisites for this course." << std::endl;
        }
    }
    else {
        std::cerr << "Course not found with the specified ID: " << courseNumber << std::endl;
    }
}

int main() {
    std::vector<Course> courses; // Vector to store course objects
    while (true) {
        // Display menu options
        std::cout << "Welcome to the Course Planner" << std::endl;
        std::cout << "\nMenu Options:" << std::endl;
        std::cout << "1. Load Data Structure" << std::endl;
        std::cout << "2. Print Course List" << std::endl;
        std::cout << "3. Print Course Information" << std::endl;
        std::cout << "4. Exit" << std::endl;

        int choice;
        std::cout << "Enter your choice (1-4): ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string filename;
            std::cout << "Enter the file name containing course data: ";
            std::cin >> filename;
            courses = readCourseData(filename);
            std::cout << "Data loaded successfully." << std::endl;
            break;
        }
        case 2:
            if (!courses.empty()) {
                printAlphanumericCourseList(courses);
            }
            else {
                std::cout << "No course data loaded yet." << std::endl;
            }
            break;
        case 3: {
            if (!courses.empty()) {
                std::string courseNumber;
                std::cout << "Enter the course number to print information: ";
                std::cin >> courseNumber;
                printCourseInformation(courses, courseNumber);
            }
            else {
                std::cout << "No course data loaded yet." << std::endl;
            }
            break;
        }
        case 4:
            std::cout << "Exiting the program." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please enter a number between 1 and 4." << std::endl;
        }
    }
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
