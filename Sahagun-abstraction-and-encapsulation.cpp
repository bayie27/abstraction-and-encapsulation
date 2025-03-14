#include <iostream>
#include <regex>
#include <vector>

using namespace std;

// Validates if the input is an integer and converts it to an integer if valid.
bool isValidInteger(const string& input, int& output) {
    try {
        size_t pos;
        output = stoi(input, &pos);
        if (pos == input.length()) { // Ensures entire string is converted
            return true;
        }
    } catch (const exception&) {
        return false; // If conversion fails, return false
    }
    return false;
}

// Validates if the input is within a specified menu number range.
bool isValidMenuNumber(const string& input, int& output, int min, int max) {
    if (input.find(' ') != string::npos) return false; // Rejects input with spaces
    if (isValidInteger(input, output)) {
        return output >= min && output <= max; // Ensures number is within range
    }
    return false;
}

// Validates if the input is a valid decimal number format (for salary and hours)
bool isValidDecimal(const string& input, double& output) {
    regex decimalRegex("^\\d+(\\.\\d{1,2})?$"); // Regular expression for decimal numbers
    if (regex_match(input, decimalRegex)) {
        output = stod(input);
        return true;
    }
    return false;
}

// Validates if the ID is valid (no whitespace, alphanumeric only)
bool isValidID(const string& id) {
    // Check for whitespace
    if (id.find(' ') != string::npos) {
        return false;
    }
    
    // Check if ID contains only alphanumeric characters
    regex idRegex("^[a-zA-Z0-9]+$");
    return regex_match(id, idRegex);
}

// Abstract base class for Employee (Abstraction)
class Employee {
    private:
        string id;
        string name;
        
    public:
        
    Employee(string empID, string empName) : id(empID), name(empName) {}
    
    // Pure virtual functions (Abstraction)
        virtual double calculateSalary() const = 0;
        virtual void displayPayrollReport() const = 0;
        
        // Virtual destructor
        virtual ~Employee() {}
        
        // Getter methods (Encapsulation)
        string getId() const {
            return id;
        }
        
        string getName() const {
            return name;
        }
};

// Derived class for Full-time employees
class FullTimeEmployee : public Employee {
    private:
        double salary; // Fixed monthly salary
        
    public:
        // Constructor
        FullTimeEmployee(string id, string name, double monthlySalary)
            : Employee(id, name), salary(monthlySalary) {}
        
        // Override calculateSalary method
        double calculateSalary() const override {
            return salary;
        }
        
        // Override displayPayrollReport method
        void displayPayrollReport() const override {
            cout << "Employee: " << getName() << " (ID: " << getId() << ")" << endl;
            cout << "Fixed Monthly Salary: $" << salary << endl;
        }
};

// Derived class for Part-time employees
class PartTimeEmployee : public Employee {
    private:
        double hourlyWage;
        double hoursWorked;
        
    public:
        // Constructor
        PartTimeEmployee(string id, string name, double wage, double hours)
            : Employee(id, name), hourlyWage(wage), hoursWorked(hours) {}
        
        // Override calculateSalary method
        double calculateSalary() const override {
            return hourlyWage * hoursWorked;
        }
        
        // Override displayPayrollReport method
        void displayPayrollReport() const override {
            cout << "Employee: " << getName() << " (ID: " << getId() << ")" << endl;
            cout << "Hourly Wage: $" << hourlyWage << endl;
            cout << "Hours Worked: " << hoursWorked << endl;
            cout << "Total Salary: $" << calculateSalary() << endl;
        }
};

// Derived class for Contractual employees
class ContractualEmployee : public Employee {
    private:
        double paymentPerProject;
        int projectsCompleted;
        
    public:
        // Constructor
        ContractualEmployee(string id, string name, double payment, int projects)
            : Employee(id, name), paymentPerProject(payment), projectsCompleted(projects) {}
        
        // Override calculateSalary method
        double calculateSalary() const override {
            return paymentPerProject * projectsCompleted;
        }
        
        // Override displayPayrollReport method
        void displayPayrollReport() const override {
            cout << "Employee: " << getName() << " (ID: " << getId() << ")" << endl;
            cout << "Contract Payment Per Project: $" << paymentPerProject << endl;
            cout << "Projects Completed: " << projectsCompleted << endl;
            cout << "Total Salary: $" << calculateSalary() << endl;
        }
};

// PayrollSystem class to manage employees
class PayrollSystem {
    private:
        vector<Employee*> employees;
        
        // Helper function to check if an ID already exists
        bool isIdUnique(const string& id) const {
            return none_of(employees.begin(), employees.end(),
                          [&id](const Employee* emp) { return emp->getId() == id; });
        }
        
        // Helper function to get employee ID with validation
        string getEmployeeId() {
            string id;
            bool validInput = false;
            
            while (!validInput) {
                cout << "Enter Employee ID: ";
                getline(cin, id);
                
                if (id.empty()) {
                    cout << "ID cannot be empty. Please try again." << endl;
                } else if (!isValidID(id)) {
                    cout << "Invalid ID format! ID must contain only alphanumeric characters: ID must contain only letters and numbers with no spaces or special characters." << endl;
                } else if (!isIdUnique(id)) {
                    cout << "Duplicate ID! Please enter a unique ID." << endl;
                } else {
                    validInput = true;
                }
            }
            return id;
        }
        
        // Helper function to get employee name
        string getEmployeeName() {
            string name;
            bool validInput = false;
            
            while (!validInput) {
                cout << "Enter Employee Name: ";
                getline(cin, name);
                
                if (!name.empty()) {
                    validInput = true;
                } else {
                    cout << "Name cannot be empty. Please try again." << endl;
                }
            }
            return name;
        }
        
        // Helper function to get valid decimal value (for salary and hours)
        double getValidDecimalInput(const string& prompt) {
            string input;
            double value;
            bool validInput = false;
            
            while (!validInput) {
                cout << prompt;
                getline(cin, input);
                
                if (isValidDecimal(input, value)) {
                    if (value > 0) {
                        validInput = true;
                    } else {
                        cout << "Value must be greater than zero. Please try again." << endl;
                    }
                } else {
                    cout << "Invalid format. Please enter a valid number." << endl;
                }
            }
            return value;
        }
        
        // Helper function to get valid number of projects
        int getValidNumericInput(const string& prompt) {
            string input;
            int value;
            bool validInput = false;
            
            while (!validInput) {
                cout << prompt;
                getline(cin, input);
                
                if (isValidInteger(input, value)) {
                    if (value >= 0) {
                        validInput = true;
                    } else {
                        cout << "Value cannot be negative. Please try again." << endl;
                    }
                } else {
                    cout << "Invalid input. Please enter a valid number." << endl;
                }
            }
            return value;
        }
        
    public:
        // Destructor to free memory
        ~PayrollSystem() {
            for (auto emp : employees) {
                delete emp;
            }
        }
        
        // Function to add a full-time employee
        void addFullTimeEmployee() {
            string id = getEmployeeId();
            string name = getEmployeeName();
            double salary = getValidDecimalInput("Enter Monthly Salary: $");
            
            employees.push_back(new FullTimeEmployee(id, name, salary));
            cout << "Full-time employee added successfully!" << endl;
        }
        
        // Function to add a part-time employee
        void addPartTimeEmployee() {
            string id = getEmployeeId();
            string name = getEmployeeName();
            double hourlyWage = getValidDecimalInput("Enter Hourly Wage: $");
            double hoursWorked = getValidDecimalInput("Enter Number of Hours Worked: ");
            
            employees.push_back(new PartTimeEmployee(id, name, hourlyWage, hoursWorked));
            cout << "Part-time employee added successfully!" << endl;
        }
        
        // Function to add a contractual employee
        void addContractualEmployee() {
            string id = getEmployeeId();
            string name = getEmployeeName();
            double paymentPerProject = getValidDecimalInput("Enter Payment Per Project: $");
            int projectsCompleted = getValidNumericInput("Enter Number of Projects Completed: ");
            
            employees.push_back(new ContractualEmployee(id, name, paymentPerProject, projectsCompleted));
            cout << "Contractual employee added successfully!" << endl;
        }
        
        // Function to display payroll report
        void displayPayrollReport() const {
            if (employees.empty()) {
                cout << "No employees to display." << endl;
                return;
            }
            
            cout << "------ Employee Payroll Report ------" << endl;
            
            for (const auto& emp : employees) {
                emp->displayPayrollReport();
                cout << endl;
            }
        }
};

int main() {
    PayrollSystem payrollSystem;
    string choice;
    
    do {
        // Display main menu
        cout << "\n=============================\n";
        cout << "    PAYROLL SYSTEM MENU    \n";
        cout << "=============================\n";
        cout << "[1] Full-time Employee\n";
        cout << "[2] Part-time Employee\n";
        cout << "[3] Contractual Employee\n";
        cout << "[4] Display Payroll Report\n";
        cout << "[5] Exit\n";
        cout << "=============================\n";
        cout << "Enter your choice: ";
        getline(cin, choice);
            
        int option;
        if (isValidMenuNumber(choice, option, 1, 5)) {
            switch (option) {
                case 1:
                    payrollSystem.addFullTimeEmployee();
                    break;
                case 2:
                    payrollSystem.addPartTimeEmployee();
                    break;
                case 3:
                    payrollSystem.addContractualEmployee();
                    break;
                case 4:
                    payrollSystem.displayPayrollReport();
                    break;
                case 5:
                    cout << "Exiting program. Goodbye!" << endl;
                    break;
            }
        } else {
            cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
        }
    } while (choice != "5");

    return 0;
}