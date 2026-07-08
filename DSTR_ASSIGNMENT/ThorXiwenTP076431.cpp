//Thor Xiwen TP076431
//File naming: TP076431_Thor Xiwen_CT077_LW1.cpp
//dynamic array, merge sort, binary and linear search

//compile and run command (need to save thn only can run)
//g++ -std=c++11 -o student_sys ThorXiwenTP076431.cpp
//.\student_sys

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <string>

using namespace std;

//student struct
struct student {
    string StudentID;
    string FullName;
    string ProgrammeCode;
    string YearOfStudy;
    double CGPA;
    string ContactNumber;
};

struct programme {
    string ProgrammeCode;
    string ProgrammeName;
    string Faculty;
    string DurationYears;
};

//dynamic array class
//start with a fix block of memory, whn get full, create a bigger block
//use new[] and delete[] manually -- NO std::vector
class DynamicArray {
    private: 
        student* data; //pointer to array in heap memory
        int size;  // how many record store here
        int capacity; // how many slots are allocated

        void resize() {
            int newCapacity = capacity * 2; 
            student* tmp = new student [newCapacity];
            for (int i = 0; i < size; i++) {
                tmp[i] = data[i];
            }
            delete[] data;
            data = tmp;
            capacity = newCapacity;
        }

        //Merge sort (using 0(n log n)
        //1. split array in half (left half, right half)
        //2. recursively sort both half
        //3. merge two sorted halves back into one sorted array

    void mergeByCGPA(student* arr, int left, int mid, int right, bool ascending) {
        int num1 = mid - left + 1;
        int num2 = right - mid;
 
        // Temporary arrays — no STL, just plain new[]
        student* Left = new student[num1];
        student* Right = new student[num2];
 
        for (int i = 0; i < num1; i++) Left[i] = arr[left + i];
        for (int j = 0; j < num2; j++) Right[j] = arr[mid + 1 + j];
 
        int i = 0, j = 0, k = left;
 
        while (i < num1 && j < num2) {
            bool pickLeft;
            if (ascending)
                pickLeft = (Left[i].CGPA <= Right[j].CGPA);
            else
                pickLeft = (Left[i].CGPA >= Right[j].CGPA);
 
            if (pickLeft) arr[k++] = Left[i++];
            else          arr[k++] = Right[j++];
        }
        while (i < num1) arr[k++] = Left[i++];
        while (j < num2) arr[k++] = Right[j++];
 
        delete[] Left;
        delete[] Right;
    }


    //merge sort
    //using 0(n log n)
    void mergeSortByCGPA(student* arr, int l, int r, bool ascending) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSortByCGPA(arr, l, m, ascending);
        mergeSortByCGPA(arr, m + 1, r, ascending);
        mergeByCGPA(arr, l, m, r, ascending);
    }

    //helping for sort ID
    void mergeStudentID (student* arr, int left, int mid, int right){
        int num1 = mid-left +1;
        int num2 = right - mid;

        student* leftHalf= new student [num1];
        student* rightHalf = new student [num2];

        for (int i = 0; i < num1; i++) leftHalf[i] = arr[left + i];
        for (int j = 0; j < num2; j++) rightHalf[j] = arr[mid + 1 + j];
        int i = 0, j = 0, k = left;
        while (i < num1 && j < num2) {
            if (leftHalf[i].StudentID <= rightHalf[j].StudentID) 
                arr[k++] = leftHalf[i++];
            else 
                arr[k++] = rightHalf[j++];
        }
        while (i < num1) arr[k++] = leftHalf[i++];
        while (j < num2) arr[k++] = rightHalf[j++];
        delete[] leftHalf;
        delete[] rightHalf;
    }

    // add merge student ID for binary search
    // one step inside the process (for internal help use)
    void mergeSortByStudentID (student* arr, int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSortByStudentID(arr, l, m);
        mergeSortByStudentID(arr, m + 1, r);
        mergeStudentID(arr, l, m, r);
    }


    public: 
    //constructor
        DynamicArray (int initCap = 100) : size(0), capacity(initCap) {
            data = new student[capacity];
        }

        //destructor (for free heap memory)
        ~DynamicArray() {
            delete[] data; 
        }

        DynamicArray& operator = (const DynamicArray& other) {
            if (this == &other) return *this;
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new student[capacity];
            for (int i = 0; i < size; i++) data [i] = other.data[i];
            return *this;

        }

    //validate CGPA range and reject duplicate student ID
    //add (insert a new student)
    //1. check the CGPA isnt in 0.0 till 4.0
    // if CGPA is less then 0.0 or more than 4.0, will reject it
    bool add(const student& students) {
    //validate CGPA boundry
        if (students.CGPA < 0.0 || students.CGPA > 4.0)  {
            cout << "[ERROR] CGPA " << students.CGPA << "is out of the range of (must be 0.0-4.0)" <<" for Student ID: " << students.StudentID  << "\n";
            return false;
        }
    

        //check duplicate student ID
        for (int i = 0; i< size; i++) {
            if (data [i].StudentID == students.StudentID) {
                cout << "[ERROR] Duplicate Student ID: " << students.StudentID << ", cannot add. \n";
                return false;   
            }
        }

        if (size == capacity) resize();
        data [size++] = students;
        return true;
    }
    

        int getSize () const {
            return size;
        }
        const student& get (int i ) const {
            return data[i];
        }


        //sort ascending and descending
        //sort from low to high CGPA
        void sortAscending() {
            if (size <= 1)
                return;
            mergeSortByCGPA(data, 0, size - 1, true);
        }

        // soft from high to low CGPA
        void sortDescending() {
            if (size <= 1)
                return;
            mergeSortByCGPA(data, 0, size - 1, false);
        }

    //delete (find record duplicate studentID, remove it)
    bool deleteID (const string& id) {
        if (size == 0) {
            cout << "Array is empty, cannot delete. \n";
                return false;
        } for (int i = 0; i < size; i++) {
            if (data[i].StudentID == id) {
        // Shift everything left from position i+1
            for (int j = i; j < size - 1; j++)
                data[j] = data[j + 1];
                size--;
                return true;
            }
        } 
        cout << "[ERROR] Student ID: " << id << " not found, cannot delete. \n";
        return false;
    }

    //search name (by using lower keyword)
    int searchName (const string& keyword) const {
        int found = 0;
            for (int i = 0; i<size; i++){
                string lowerName = data[i].FullName;
                string lowerKeyword = keyword;
                for (int j=0; j< lowerName.size(); j++)
                    lowerName[j] = tolower(lowerName[j]);
                for (int j=0; j< lowerKeyword.size(); j++)
                    lowerKeyword[j] = tolower(lowerKeyword[j]);
                if (lowerName.find(lowerKeyword) != string::npos) {
                    cout << data[i].StudentID << "|" << data[i].FullName << "|" << data[i].ProgrammeCode << "|" << data[i].YearOfStudy << "|" << data[i].CGPA << "|" << data[i].ContactNumber << "\n";
                    found++;
            }
        } return found;
    }


    // helping to sort the STudentID 
    void sortByStudentID() {
        if (size <= 1)
            return;
        mergeSortByStudentID(data, 0, size - 1);
    }


    //Search
    //Linear search
    int linearSearchStudentID(const string& id) const {
        for (int i = 0; i < size; i++) {
            if (data[i].StudentID == id)
                return i;  // found
        }
        return -1;  // not found
}


    //binary search
    int binarySearchStudentID (const string& id) {
        int left = 0; //start from the first student
        int right = size-1; //end at last student

        while (left <= right) {
        int mid = (left + right) / 2;
        if (data[mid].StudentID == id)     return mid;
        else if (data[mid].StudentID < id) left = mid + 1;
        else                               right = mid - 1;
    }
        return -1;  // not found
    }


    // this section is to helping load the dataset file
    void loadCSV (const string& filename) {
        ifstream file (filename);
        if (!file.is_open()){
            cout << "[ERROR] Failed to open file: " << filename << "\n";
            return;
        }
        string line;
        getline (file,line); //skip header

        while (getline (file, line)) {
            if (line.empty()) continue;

                    stringstream ss(line);
        student s;
        string cgpaStr;

        getline(ss, s.StudentID, ',');
        getline(ss, s.FullName, ',');
        getline(ss, s.ProgrammeCode, ',');
        getline(ss, s.YearOfStudy, ',');
        getline(ss, cgpaStr, ',');
        getline(ss, s.ContactNumber, ',');

        s.CGPA = stod(cgpaStr); // convert string to double

        add(s); // uses your existing add() with validation
    }

    file.close();
    cout << "[OK] Loaded: " << filename << "\n";
    }


    void displayAll() const {
        if (size == 0){
            cout << "No record display. \n";
            return;
        }

        cout << "\n No.  ID | Name | Programme | Year | CGPA | Contact \n";
        for (int i = 0; i < size; i++) {
            cout << (i + 1) << " | "
                 << data[i].StudentID << " | "
                 << data[i].FullName << " | "
                 << data[i].ProgrammeCode << " | "
                 << data[i].YearOfStudy << " | "
                 << data[i].CGPA << " | "
                 << data[i].ContactNumber << "\n";
    }
    cout << "\nTotal records: " << size << "\n";
    }
};

//linear o(n) 
string findProgrammeName (const string& code, programme* programmes, int progCount) {
    for (int i = 0; i < progCount; i++) {
        if (programmes[i].ProgrammeCode == code) {
            return programmes[i].ProgrammeName;
        } 
    } return "Unknown Programme";
}

string findFaculty (const string& code, programme* programmes, int progCount) {
    for (int i = 0; i< progCount; i++) {
        if (programmes[i].ProgrammeCode == code) {
            return programmes[i].Faculty;
        }
    }
    return "Unknown Faculty";
}

string findDuration(const string& code, programme* programmes, int progCount) {
    for (int i = 0; i < progCount; i++)
        if (programmes[i].ProgrammeCode == code)
            return programmes[i].DurationYears;
    return "?";
}


int main() {
    DynamicArray arr;

    //load programmes.csv
    programme programmes [30];
    int progCount = 0;

    ifstream progFile ("programmes.csv");
    if (progFile.is_open()) {
        string progLine;
        getline (progFile, progLine); //skip header
        while (getline (progFile, progLine)) {
            if (progLine.empty()) continue;
            stringstream ss(progLine);
            programme p;
            getline(ss, p.ProgrammeCode, ',');
            getline(ss, p.ProgrammeName, ',');  
            getline(ss, p.Faculty, ',');
            getline(ss, p.DurationYears, ',');
            programmes[progCount++] = p;
        } 
        progFile.close();
        cout << "Loaded programmes.csv (" << progCount << " programmes \n";
    } else {
        cout << "cannot open programmes dataset. \n";
    }

    // run the file one by one
    //only can test one file at the time
    int choiceDataset;
    cout <<"PLease select dataset to load (1-4) : \n";
    cout <<"1. students_500.csv\n";
    cout <<"2. students_2000.csv\n";
    cout <<"3. students_8000.csv\n";
    cout <<"4. students_30000.csv\n";
    cout <<"Enter your choice: ";
    cin >> choiceDataset;
    cin.ignore();

    //start timer for loading
    auto startLoad = chrono::high_resolution_clock::now();

    if (choiceDataset == 1) 
        arr.loadCSV ("students_500.csv");
    else if (choiceDataset == 2)
        arr.loadCSV ("students_2000.csv");
    else if (choiceDataset == 3)
        arr.loadCSV ("students_8000.csv");
    else if (choiceDataset == 4)
        arr.loadCSV ("students_30000.csv");
    else {
        cout << "Wrong choice! Please enter 1-4 only! \n";
    }

    //stop timer for loading
    auto endLoad = chrono::high_resolution_clock::now();
    long long durationLoad = chrono::duration_cast<chrono::microseconds>(endLoad - startLoad).count();
    cout << "Time taken to load dataset: " << durationLoad << " microseconds.\n";

    //MENU
    int choice;
    cout << "Total records loaded: " << arr.getSize () << "\n";
    do {
        cout << "\n          MENU            \n";
        cout << "1. Add student\n";
        cout << "2. Delete student by ID\n";
        cout << "3. Search student by name\n";
        cout << "4. Linear search by Student ID\n";
        cout << "5. Binary search by Student ID\n";
        cout << "6. Merge sort ascending (low to high CGPA)\n";
        cout << "7. Merge sort descending (high to low CGPA)\n";
        cout << "8. Display all records\n";
        cout << "9. Switch dataset for load\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            student newStudent;
            do {
                cout << "Enter Student ID: ";
                getline(cin, newStudent.StudentID);
                if (newStudent.StudentID.empty())
                    cout << "Student ID cannot be empty! Must from TP start\n";
            } while (newStudent.StudentID.empty());

            do {
                cout << "Enter full name: ";
                getline(cin, newStudent.FullName);
                if (newStudent.FullName.empty())
                    cout << "Full name cannot be empty! \n";
            } while (newStudent.FullName.empty());

            do {
                cout << "Enter Programme Code: ";
                getline(cin, newStudent.ProgrammeCode);
                if (newStudent.ProgrammeCode.empty())
                    cout << "Programme code cannot be empty! \n";
            } while (newStudent.ProgrammeCode.empty());

            do {
                cout << "Enter year of study: ";
                getline(cin, newStudent.YearOfStudy);
                if (newStudent.YearOfStudy.empty())
                    cout << "Year of study cannot be empty! \n";
            } while (newStudent.YearOfStudy.empty());

            do {
                cout << "Enter CGPA (0.0-4.0): ";
                string cgpaStr;
                getline(cin, cgpaStr);
                try {
                    newStudent.CGPA = stod(cgpaStr);
                    if (newStudent.CGPA < 0.0 || newStudent.CGPA > 4.0) {
                        cout << "CGPA must be between 0.0 and 4.0! \n";
                        newStudent.CGPA = -1; // invalid value to trigger retry
                    }
                } catch (const invalid_argument&) {
                    cout << "Invalid CGPA format! Please enter a number. \n";
                    newStudent.CGPA = -1; // invalid value to trigger retry
                }
            } while (newStudent.CGPA < 0.0 || newStudent.CGPA > 4.0);

            do {
                cout << "Enter contact number: ";
                getline(cin, newStudent.ContactNumber);
                if (newStudent.ContactNumber.empty())
                    cout << "Contact number cannot be empty! \n";
            } while (newStudent.ContactNumber.empty());

            //time adding student info
            auto start = chrono::high_resolution_clock::now();
            bool added = arr.add(newStudent);
            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
            
            if (added){
                cout << "Student added successfully. \n";
                cout << "Time taken: " << duration << " microseconds.\n";
            }

        //delete student (need to refer to the student dataset to get the student ID)
        } else if (choice == 2) {
            string id;
            do {
                cout << "Enter Student ID to delete: ";
                getline(cin, id);
                if (id.empty())
                    cout << "Student ID cannot be empty! Must from TP start\n";
            } while (id.empty());

            //time deleting student info
            auto start = chrono::high_resolution_clock::now();
            bool deleted = arr.deleteID(id);
            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds>(end - start).count();

            if (deleted) {
                cout << "[OK] Student deleted successfully.\n";
                cout << "Delete time: " << duration << " us\n";
            }


        //search student name
        } else if (choice == 3) {
        string name;
        bool validInput = false;

        do {
            cout << "Enter student name to search: ";
            cin.clear();
            getline(cin, name);

        // trim trailing whitespace/carriage return
            while (!name.empty() && (name.back() == '\r' || name.back() == ' '))
                name.pop_back();

            if (name.empty()) {
                cout << "Search name cannot be empty!\n";
               continue;
            }

        // reject if it looks like a Student ID (TP + digits)
        if (name.length() >= 3 && name[0] == 'T' && name[1] == 'P' && isdigit(name[2])) {
            cout << "That looks like a Student ID. Please enter a student NAME: \n";
            cout << "Use option 4 or 5 to search by Student ID.\n";
            continue;
        }

        validInput = true;

    } while (!validInput);
            auto start = chrono::high_resolution_clock::now();
            int found = 0;

            string lowerKey = name;
            for (int j=0; j <lowerKey.size(); j++)
                lowerKey[j] = tolower(lowerKey[j]);
            
            for (int i = 0; i< arr.getSize(); i++){
                string lowerName = arr.get(i).FullName;
                for (int j=0; j< lowerName.size(); j++)
                    lowerName[j] = tolower(lowerName[j]);

                if (lowerName.find(lowerKey) != string::npos) {
                    const student& students = arr.get(i);
                    string progName = findProgrammeName(students.ProgrammeCode, programmes, progCount);
                    string faculty  = findFaculty(students.ProgrammeCode, programmes, progCount);
                    string duration = findDuration(students.ProgrammeCode, programmes, progCount);
                    cout << "\nSearch Result:\n";
                    cout << "Student ID    : " << students.StudentID << "\n";
                    cout << "Name          : " << students.FullName << "\n";
                    cout << "Programme Code: " << students.ProgrammeCode << "\n";
                    cout << "Programme Name: " << progName << "\n";
                    cout << "Faculty       : " << faculty << "\n";
                    cout << "Duration      : " << duration << " years\n";
                    cout << "Year of Study : " << students.YearOfStudy << "\n";
                    cout << "CGPA          : " << students.CGPA << "\n";
                    cout << "Contact Number: " << students.ContactNumber << "\n";
                    cout << "                                  \n";
                    found++;
                }
            }

            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds> (end - start). count();
            cout << "Total: found: " << found << "\n";
            cout << "Search name duration: " <<duration << " microseconds.\n";

            //linear search student ID
        } else if (choice == 4) {
        string id;
        do {
            cout << "Enter Student ID to linear search (e.g. TP012345): ";
            cin.clear();
            getline(cin, id);

        // trim whitespace/carriage return
            while (!id.empty() && (id.back() == '\r' || id.back() == ' '))
            id.pop_back();

        // validate: must be TP + exactly 6 digits
            bool validID = (id.length() == 8 &&
                id[0] == 'T' && id[1] == 'P' &&
                isdigit(id[2]) && isdigit(id[3]) &&
                isdigit(id[4]) && isdigit(id[5]) &&
                isdigit(id[6]) && isdigit(id[7]));

        if (!validID)
            cout << "Wrong format! Please write TP number correctly (e.g. TP012345), rewrite again:\n";

    } while (id.length() != 8 ||
             id[0] != 'T' || id[1] != 'P' ||
             !isdigit(id[2]) || !isdigit(id[3]) ||
             !isdigit(id[4]) || !isdigit(id[5]) ||
             !isdigit(id[6]) || !isdigit(id[7]));

            auto start = chrono::high_resolution_clock::now();
            int idx = arr.linearSearchStudentID (id);
            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
            
            if (idx !=-1) {
                const student& students = arr.get(idx);
        // look up full programme name from programmes array
                cout << "\n Linear Search Result:\n";
                string progName = findProgrammeName(students.ProgrammeCode, programmes, progCount);
                string faculty  = findFaculty(students.ProgrammeCode, programmes, progCount);
                string duration = findDuration(students.ProgrammeCode, programmes, progCount);
                cout << "\nSearch Result:\n";
                cout << "Student ID    : " << students.StudentID << "\n";
                cout << "Name          : " << students.FullName << "\n";
                cout << "Programme Code: " << students.ProgrammeCode << "\n";
                cout << "Programme Name: " << progName << "\n";
                cout << "Faculty       : " << faculty << "\n";
                cout << "Duration      : " << duration << " years\n";
                cout << "Year of Study : " << students.YearOfStudy << "\n";
                cout << "CGPA          : " << students.CGPA << "\n";
                cout << "Contact Number: " << students.ContactNumber << "\n";
                cout << "                                  \n";
            } else {
                cout << " Student ID not found " << id << "not found. \n";
            } cout << "Linear search time: " << duration << "microseconds \n";

        //binary search student ID
        } else if (choice == 5) {
            string id;
            do {
                cout << "Enter student ID by using binary search: ";
                getline (cin,id);
                if (id.empty())
                    cout <<"Student ID cannot be empty! \n";
            } while (id.empty());

            auto start = chrono::high_resolution_clock::now();
            arr.sortByStudentID();
            int idx = arr.binarySearchStudentID(id);
            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
            if (idx != -1) {
                const student& students = arr.get(idx);
                string progName  = findProgrammeName(students.ProgrammeCode, programmes, progCount);
                string faculty   = findFaculty(students.ProgrammeCode, programmes, progCount);
                string duration2 = findDuration(students.ProgrammeCode, programmes, progCount);
                cout << "\nBinary Search Result:\n";
                cout << "Student ID    : " << students.StudentID << "\n";
                cout << "Name          : " << students.FullName << "\n";
                cout << "Programme Code: " << students.ProgrammeCode << "\n";
                cout << "Programme Name: " << progName << "\n";
                cout << "Faculty       : " << faculty << "\n";
                cout << "Duration      : " << duration2 << " years\n";
                cout << "Year of Study : " << students.YearOfStudy << "\n";
                cout << "CGPA          : " << students.CGPA << "\n";
                cout << "Contact Number: " << students.ContactNumber << "\n";
                cout << "                                 \n";
            } else {
                cout << "[NOT FOUND] Student ID: " << id << " not found.\n";
            } cout << "Binary search time: " << duration << " microseconds.\n";

        // merge sort ascending by CGPA
        } else if (choice == 6) {
            auto start = chrono::high_resolution_clock::now();
            arr.sortAscending();
            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds>
                                (end - start).count();

            cout << "Sorted ascending.\n";
            cout << "Sort ascending time: " << duration << " us\n";

            cout << "\n            Sorted: Low to High CGPA      \n";
            for (int i = 0; i < arr.getSize(); i++) {
                const student& students = arr.get(i);
                string progName  = findProgrammeName(students.ProgrammeCode, programmes, progCount);
                string faculty   = findFaculty(students.ProgrammeCode, programmes, progCount);
                string duration2 = findDuration(students.ProgrammeCode, programmes, progCount);
                cout << "\nNo. " << (i + 1) << "\n";
                cout << "Student ID    : " << students.StudentID << "\n";
                cout << "Name          : " << students.FullName << "\n";
                cout << "Programme Code: " << students.ProgrammeCode << "\n";
                cout << "Programme Name: " << progName << "\n";
                cout << "Faculty       : " << faculty << "\n";
                cout << "Duration      : " << duration2 << " years\n";
                cout << "Year of Study : " << students.YearOfStudy << "\n";
                cout << "CGPA          : " << students.CGPA << "\n";
                cout << "Contact Number: " << students.ContactNumber << "\n";
                cout << "------------------------------\n";
            }
            cout << "Total records      : " << arr.getSize() << "\n";
            cout << "Sort ascending time: " << duration << " microseconds\n";

            //sort descending by CGPA
        } else if (choice == 7) {
            auto start = chrono::high_resolution_clock::now();
            arr.sortDescending();
            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds>
                                (end - start).count();
            cout << "Sorted descending." << "\n";
            cout << "\nSorted: High to Low CGPA         \n";
            for (int i = 0; i < arr.getSize(); i++) {
                const student& students = arr.get(i);
                string progName  = findProgrammeName(students.ProgrammeCode, programmes, progCount);
                string faculty   = findFaculty(students.ProgrammeCode, programmes, progCount);
                string duration2 = findDuration(students.ProgrammeCode, programmes, progCount);
                cout << "\nNo. " << (i + 1) << "\n";
                cout << "Student ID    : " << students.StudentID << "\n";
                cout << "Name          : " << students.FullName << "\n";
                cout << "Programme Code: " << students.ProgrammeCode << "\n";
                cout << "Programme Name: " << progName << "\n";
                cout << "Faculty       : " << faculty << "\n";
                cout << "Duration      : " << duration2 << " years\n";
                cout << "Year of Study : " << students.YearOfStudy << "\n";
                cout << "CGPA          : " << students.CGPA << "\n";
                cout << "Contact Number: " << students.ContactNumber << "\n";
                cout << "                                \n";
            }
            cout << "Total records       : " << arr.getSize() << "\n";
            cout << "Sort descending time: " << duration << " microseconds\n";

        //display all records
        } else if (choice == 8) {
            auto start = chrono::high_resolution_clock::now();

            if (arr.getSize() == 0) {
            cout << "No record to display.\n";
        } else {
            const int PAGE_SIZE = 30; 
            int total = arr.getSize();
            int page = 0;
            int totalPages = (total + PAGE_SIZE - 1) / PAGE_SIZE;

            while (true) {
                int from = page * PAGE_SIZE;
                int to   = min(from + PAGE_SIZE, total);

            for (int i = from; i < to; i++) {
                const student& s = arr.get(i);
                string progName  = findProgrammeName(s.ProgrammeCode, programmes, progCount);
                string faculty   = findFaculty(s.ProgrammeCode, programmes, progCount);
                string duration2 = findDuration(s.ProgrammeCode, programmes, progCount);
                cout << "\nNo. " << (i + 1) << "\n";
                cout << "Student ID    : " << s.StudentID << "\n";
                cout << "Name          : " << s.FullName << "\n";
                cout << "Programme Code: " << s.ProgrammeCode << "\n";
                cout << "Programme Name: " << progName << "\n";
                cout << "Faculty       : " << faculty << "\n";
                cout << "Duration      : " << duration2 << " years\n";
                cout << "Year of Study : " << s.YearOfStudy << "\n";
                cout << "CGPA          : " << s.CGPA << "\n";
                cout << "Contact Number: " << s.ContactNumber << "\n";
                cout << "                               \n";
            }


            cout << "\nPage " << (page + 1) << " of " << totalPages
                 << "  |  Records " << (from + 1) << "-" << to
                 << " of " << total << "\n";
            cout << "Please press for choosing the seeing the list";
            cout << "[N]next  [P]prev  [Q]quit display: ";

            string nav;
            getline(cin, nav);
            if (!nav.empty()) nav[0] = tolower(nav[0]);

            if (nav == "n" && page < totalPages - 1) page++;
            else if (nav == "p" && page > 0)         page--;
            else if (nav == "q")                      break;
            else cout << "  (enter N, P, or Q)\n";
        }

        cout << "\nTotal records: " << total << "\n";
    }

    auto end = chrono::high_resolution_clock::now();
    long long duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
        cout << "Display time: " << duration << " microseconds.\n";

            // swicth dataset for load
        } else if (choice == 9) {
            cout << "Please select dataset to load: \n";
            cout << "1. students_500.csv\n";
            cout << "2. students_2000.csv\n";
            cout << "3. students_8000.csv\n";
            cout << "4. students_30000.csv\n";
            cout <<" Enter your choice:";
            int newData;
            cin >> newData;
            cin.ignore();

            DynamicArray newArr;
            //start timer before loading new dataset
            auto start = chrono::high_resolution_clock::now();

            //load dataset based on user choice
            if (newData == 1) newArr.loadCSV("students_500.csv");
            else if (newData == 2) newArr.loadCSV("students_2000.csv");
            else if (newData == 3) newArr.loadCSV("students_8000.csv");
            else if (newData == 4) newArr.loadCSV("students_30000.csv");
            else 
                cout << "Invalid choice! Please enter 1-4: \n";
            
            auto end = chrono::high_resolution_clock::now();
            long long duration = chrono::duration_cast<chrono::microseconds>
                                (end - start).count();
            arr = newArr;
            cout << "Total records loaded: " << arr.getSize() << "\n";
            cout << "Time taken to load new dataset: " << duration << " microseconds.\n";

        } else if (choice == 0) {
            cout << "Thank for checking, Goodbye!\n";

        } else {
            cout << "Invalid choice. Please enter 0-9.\n";
        }

    } while (choice != 0);
    return 0;
}


