/*
 * ============================================================
 *  MediCare Pharma Ltd. — Corporate Head Office
 *  Employee Management System
 *  C++ Implementation
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

using namespace std;

// ─────────────────────────────────────────────
//  Date / Time helper structures
// ─────────────────────────────────────────────

struct Date {
    int day, month, year;

    string toString() const {
        ostringstream oss;
        oss << setw(2) << setfill('0') << day << "/"
            << setw(2) << setfill('0') << month << "/"
            << year;
        return oss.str();
    }
};

struct TimePoint {
    int hour, minute;   // 24-hour format

    string toString() const {
        if (hour < 0) return "  --:--  ";          // absent marker
        ostringstream oss;
        oss << setw(2) << setfill('0') << hour << ":"
            << setw(2) << setfill('0') << minute;
        return oss.str();
    }

    bool isValid() const { return hour >= 0; }
};

// ─────────────────────────────────────────────
//  Attendance record for a single day
// ─────────────────────────────────────────────

struct DailyAttendance {
    Date        date;
    bool        isAbsent      = false;
    bool        isWeekend     = false;       // Friday in Bangladesh

    // Office entry / exit
    TimePoint   entryTime     = {-1, -1};
    TimePoint   exitTime      = {-1, -1};

    // Lunch break
    TimePoint   lunchOut      = {-1, -1};   // employee leaves for lunch
    TimePoint   lunchIn       = {-1, -1};   // employee returns from lunch
};

// ─────────────────────────────────────────────
//  Department enum  (12 departments)
// ─────────────────────────────────────────────

enum class Department {
    Sales,
    Marketing,
    Audit,
    Admin,
    InventoryManagement,
    Legal,
    Production,
    QualityControl,
    SupplyChain,
    HumanResource,
    Export,
    InternationalBusiness
};

string deptToString(Department d) {
    switch (d) {
        case Department::Sales:                return "Sales";
        case Department::Marketing:            return "Marketing";
        case Department::Audit:                return "Audit";
        case Department::Admin:                return "Admin";
        case Department::InventoryManagement:  return "Inventory Management";
        case Department::Legal:                return "Legal";
        case Department::Production:           return "Production";
        case Department::QualityControl:       return "Quality Control";
        case Department::SupplyChain:          return "Supply Chain";
        case Department::HumanResource:        return "Human Resource";
        case Department::Export:               return "Export";
        case Department::InternationalBusiness:return "International Business";
        default:                               return "Unknown";
    }
}

// ─────────────────────────────────────────────
//  Employee class
// ─────────────────────────────────────────────

class Employee {
public:
    // ── Core fields ──────────────────────────
    int         employeeId;
    string      name;
    double      salary;          // in BDT (Bangladeshi Taka)
    Department  department;
    Date        joiningDate;

    // Attendance log  (keyed: YYYYMM → vector of DailyAttendance)
    map<string, vector<DailyAttendance>> attendanceLog;

    // ── Constructor ──────────────────────────
    Employee(int id, const string& n, double sal,
             Department dept, Date joining)
        : employeeId(id), name(n), salary(sal),
          department(dept), joiningDate(joining) {}

    // ── Generate attendance for a given month ─
    //   year: e.g. 2024,  month: 1-12
    void generateMonthlyAttendance(int year, int month) {
        string key = to_string(year) + (month < 10 ? "0" : "") + to_string(month);
        if (attendanceLog.count(key)) return;   // already generated

        vector<DailyAttendance> records;
        int daysInMonth = daysInMonthCount(year, month);

        mt19937 rng(employeeId * 1000 + month * 100 + year);
        uniform_int_distribution<int> absentChance(1, 100);

        // Entry time variation: 8:45 – 9:15
        uniform_int_distribution<int> entryMinVar(0, 30);
        // Exit time variation:  5:00 – 5:30 PM
        uniform_int_distribution<int> exitMinVar(0, 30);
        // Lunch-out variation: 1:00 – 1:10 PM
        uniform_int_distribution<int> lunchOutVar(0, 10);
        // Lunch-in  variation: 2:30 – 2:45 PM
        uniform_int_distribution<int> lunchInVar(0, 15);

        int workDays = 0;

        for (int d = 1; d <= daysInMonth; d++) {
            DailyAttendance rec;
            rec.date = {d, month, year};

            // Friday = day-of-week == 5  (simple Zeller approximation)
            int dayOfWeek = dayOfWeekZeller(d, month, year);
            if (dayOfWeek == 5) {          // Friday → weekend
                rec.isWeekend = true;
                records.push_back(rec);
                continue;
            }

            // Stop after 22 working days
            if (workDays >= 22) {
                rec.isAbsent = true;       // extra days beyond 22 workdays
                records.push_back(rec);
                continue;
            }
            workDays++;

            // ~5 % absence probability
            if (absentChance(rng) <= 5) {
                rec.isAbsent = true;
                records.push_back(rec);
                continue;
            }

            // Present: fill times
            int entryMin  = 45 + entryMinVar(rng);  // 8:45 – 9:15
            rec.entryTime = {8 + entryMin / 60, entryMin % 60};

            int exitMin   = exitMinVar(rng);          // 17:00 – 17:30
            rec.exitTime  = {17, exitMin};

            // Lunch out: 13:00 – 13:10
            rec.lunchOut  = {13, lunchOutVar(rng)};
            // Lunch in:  14:30 – 14:45
            rec.lunchIn   = {14, 30 + lunchInVar(rng)};

            records.push_back(rec);
        }

        attendanceLog[key] = records;
    }

    // ── Display full monthly report ───────────
    void displayMonthlyReport(int year, int month) const {
        string key = to_string(year) + (month < 10 ? "0" : "") + to_string(month);

        cout << "MediCare Pharma Ltd. — Employee Monthly Report \n";

        cout << "  Employee ID   : " << employeeId << "\n";
        cout << "  Name          : " << name << "\n";
        cout << "  Department    : " << deptToString(department) << "\n";
        cout << "  Monthly Salary: BDT " << fixed << setprecision(2) << salary << "\n";
        cout << "  Period        : " << monthName(month) << " " << year << "\n";
        //cout << "──────────────────────────────────────────────────────────────────────\n";

        if (!attendanceLog.count(key)) {
            cout << "  [No attendance data for this period]\n";
            return;
        }

        const auto& records = attendanceLog.at(key);

        int presentDays = 0, absentDays = 0;

        cout << left
             << setw(5)  << "Day"
             << setw(12) << "Date"
             << setw(10) << "Status"
             << setw(10) << "Entry"
             << setw(10) << "Exit"
             << setw(12) << "LunchOut"
             << setw(12) << "LunchIn"
             << "\n";
        cout << string(71, '-') << "\n";

        for (const auto& rec : records) {
            if (rec.isWeekend) {
                cout << setw(5)  << rec.date.day
                     << setw(12) << rec.date.toString()
                     << setw(10) << "Weekend"
                     << "\n";
                continue;
            }
            // Skip days beyond 22 workdays that are marked absent just as filler
            string status;
            if (rec.isAbsent) {
                status = "ABSENT";
                absentDays++;
            } else {
                status = "Present";
                presentDays++;
            }

            cout << setw(5)  << rec.date.day
                 << setw(12) << rec.date.toString()
                 << setw(10) << status
                 << setw(10) << rec.entryTime.toString()
                 << setw(10) << rec.exitTime.toString()
                 << setw(12) << rec.lunchOut.toString()
                 << setw(12) << rec.lunchIn.toString()
                 << "\n";
        }

        cout << string(71, '-') << "\n";
        cout << "  Present Days : " << presentDays << " / 22\n";
        cout << "  Absent Days  : " << absentDays << "\n";

        // Per-day salary deduction for absences
        double perDay       = salary / 22.0;
        double deduction    = absentDays * perDay;
        double netSalary    = salary - deduction;

        cout << "  Gross Salary : BDT " << fixed << setprecision(2) << salary   << "\n";
        cout << "  Deduction    : BDT " << fixed << setprecision(2) << deduction << "  (" << absentDays << " day(s))\n";
        cout << "  Net Salary   : BDT " << fixed << setprecision(2) << netSalary << "\n";
       // cout << "══════════════════════════════════════════════════════════════════════\n";
    }

private:
    // ── Helper: days in month ─────────────────
    static int daysInMonthCount(int y, int m) {
        static const int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        bool leap = (y%4==0 && y%100!=0) || (y%400==0);
        return (m==2 && leap) ? 29 : days[m-1];
    }

    // ── Helper: Zeller's congruence → 0=Sun…6=Sat
    static int dayOfWeekZeller(int d, int m, int y) {
        if (m < 3) { m += 12; y--; }
        int k = y % 100, j = y / 100;
        int h = (d + (13*(m+1))/5 + k + k/4 + j/4 + 5*j) % 7;
        return (h + 6) % 7;   // 0=Mon…6=Sun; Friday=4
    }

    // ── Helper: month name ────────────────────
    static string monthName(int m) {
        static const string names[] = {
            "January","February","March","April","May","June",
            "July","August","September","October","November","December"
        };
        return (m>=1&&m<=12) ? names[m-1] : "Unknown";
    }
};

// ─────────────────────────────────────────────
//  Company  — holds all 2500 employees
// ─────────────────────────────────────────────

class MediCarePharma {
public:
    string companyName = "MediCare Pharma Ltd.";
    string headOffice  = "Dhaka Corporate Head Office";

    vector<Employee> employees;

    // ── Constructor: generates all 2500 employees
    MediCarePharma() {
        buildWorkforce();
    }

    // ── Look up employee by ID ────────────────
    Employee* findById(int id) {
        for (auto& e : employees)
            if (e.employeeId == id) return &e;
        return nullptr;
    }

    // ── Interactive query ─────────────────────
    void queryByIdAndMonth() {
        cout<< "MediCare Pharma — Attendance Query"<<endl;


        int id, month, year;
        cout << "  Enter Employee ID : "; cin >> id;
        cout << "  Enter Month (1-12): "; cin >> month;
        cout << "  Enter Year        : "; cin >> year;

        Employee* emp = findById(id);
        if (!emp) {
            cout << "  [ERROR] No employee found with ID " << id << "\n";
            return;
        }

        emp->generateMonthlyAttendance(year, month);
        emp->displayMonthlyReport(year, month);
    }

    // ── Print department summary ──────────────
    void printDepartmentSummary() const {
        map<Department, int> counts;
        for (const auto& e : employees) counts[e.department]++;
        cout<< "Department-wise Headcount "<<endl;

        for (auto& p : counts)
            cout << "  " << left << setw(30) << deptToString(p.first)
                 << ": " << p.second << "\n";
        cout << "  " << string(36,'-') << "\n";
        cout << "  " << left << setw(30) << "Total" << ": " << employees.size() << "\n";
    }

private:
    // ─ Bangladeshi names pool ─────────────────
    vector<string> maleNames = {
        "Rahim","Karim","Jibon","Habib","Faruk","Salam","Hasan","Hossain",
        "Rashed","Milon","Sumon","Belal","Noman","Liton","Rubel","Masum",
        "Shakil","Imran","Ripon","Arif","Saiful","Ratan","Tuhin","Mehedi",
        "Nasir","Jakir","Abul","Motin","Jalal","Forhad","Tanvir","Sabbir",
        "Raihan","Sagor","Polash","Nayan","Titu","Dipu","Babu","Mintu"
    };
    vector<string> femaleNames = {
        "Mitu","Ritu","Lipi","Shila","Rekha","Puja","Nasrin","Kakoli",
        "Bristy","Shanta","Moni","Lima","Tania","Sonia","Ruma","Poli",
        "Dipa","Sharmin","Farhana","Nipa","Sumi","Liza","Konika","Arifa",
        "Brishti","Mousumi","Fatema","Rahima","Sajeda","Salma"
    };
    vector<string> surnames = {
        "Hossain","Ahmed","Khan","Ali","Islam","Miah","Sheikh","Mondal",
        "Sarker","Biswas","Das","Roy","Chowdhury","Akter","Begum","Khatun",
        "Talukder","Molla","Gazi","Dewan","Pramanik","Babu","Nabi","Alam"
    };

    // ─ Random engine ─────────────────────────
    mt19937 rng{42};

    // ─ Department → (count, salary range) ────
    struct DeptConfig {
        Department dept;
        int        headcount;
        double     salMin, salMax;
    };

    // Total = 300+400+100+100+150+50+300+150+230+230+245+245 = 2500
    vector<DeptConfig> deptConfigs = {
        {Department::Sales,                300,  35000, 70000},
        {Department::Marketing,            400,  40000, 80000},
        {Department::Audit,                100,  45000, 90000},
        {Department::Admin,                100,  35000, 65000},
        {Department::InventoryManagement,  150,  38000, 68000},
        {Department::Legal,                 50,  60000,100000},
        {Department::Production,           300,  40000, 75000},
        {Department::QualityControl,       150,  42000, 78000},
        {Department::SupplyChain,          230,  40000, 72000},
        {Department::HumanResource,        230,  38000, 70000},
        {Department::Export,               245,  45000, 85000},
        {Department::InternationalBusiness,245,  55000,100000},
    };

    // ─ Build full workforce ───────────────────
    void buildWorkforce() {
        employees.reserve(2500);
        int idCounter = 10001;

        for (auto& cfg : deptConfigs) {
            uniform_real_distribution<double> salDist(cfg.salMin, cfg.salMax);
            uniform_int_distribution<int>     nameDist(0,
                (int)(maleNames.size() + femaleNames.size()) - 1);
            uniform_int_distribution<int>     surDist(0,(int)surnames.size()-1);
            uniform_int_distribution<int>     dayDist(1,28);
            uniform_int_distribution<int>     monDist(1,12);
            uniform_int_distribution<int>     yearDist(2015,2023);

            for (int i = 0; i < cfg.headcount; i++) {
                int ni = nameDist(rng);
                string firstName = (ni < (int)maleNames.size())
                                   ? maleNames[ni]
                                   : femaleNames[ni - (int)maleNames.size()];
                string fullName  = firstName + " " + surnames[surDist(rng)];

                double sal = round(salDist(rng) / 500.0) * 500.0; // round to 500
                Date joining = {dayDist(rng), monDist(rng), yearDist(rng)};

                employees.emplace_back(idCounter++, fullName, sal, cfg.dept, joining);
            }
        }
    }
};

// ─────────────────────────────────────────────
//  Main  —  interactive menu
// ─────────────────────────────────────────────

int main() {
    cout<<"Welcome to pharma company: \n";
    cout<<"\n";

    cout<<"Now start Testing"<<endl;

    cout<<"X1"<<endl;
    cout << "\n";
    cout << "MediCare Pharma Ltd.\n";
    cout<<"X2\n"<<endl;
    cout << "Corporate Head Office — HR Management System\n";
    cout << "\n";
    cout << " Initializing 2500 employee records … ";
    cout<<"\nTesting going ON\n"<<endl;
    cout.flush();

    MediCarePharma company;

    cout << "Congratulation information allocation Done!\n";


    cout<<"\nTesting going ON\n"<<endl;
    bool running = true;
    while (running) {

        cout << "Now enter Choice form 1 to 3: "<<endl;
        cout << "1. View Employee Monthly Report\n";
        cout << "2. Department Headcount \n";
        cout << "3. Exit    \n";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                company.queryByIdAndMonth();
                break;
            case 2:
                company.printDepartmentSummary();
                break;
            case 3:
                running = false;
                cout << "  Goodbye!\n";
                break;
            default:
                cout << "  Invalid choice. Try again.\n";
        }
    }

    return 0;
}
