# MediCare Pharma Ltd. – Employee Management System

## Overview
The **MediCare Pharma Ltd. Employee Management System** is a C++ console-based project designed to simulate a real-world corporate HR and attendance management system for a pharmaceutical company in Bangladesh.

The system manages:
- 2500 employee records
- 12 different departments
- Monthly attendance generation
- Salary deduction based on absence
- Department-wise employee summary
- Employee monthly attendance reports

This project demonstrates:
- Object-Oriented Programming (OOP)
- Classes & Objects
- Structs
- Vectors & Maps
- Random data generation
- Date & Time handling
- Employee attendance management
- Salary calculation system

---

# Features

## Employee Management
Each employee contains:
- Employee ID
- Employee Name
- Salary
- Department
- Joining Date

---

## Departments Included

1. Sales
2. Marketing
3. Audit
4. Admin
5. Inventory Management
6. Legal
7. Production
8. Quality Control
9. Supply Chain
10. Human Resource
11. Export
12. International Business

---

# Attendance System

## Monthly Attendance Generation
The system automatically generates:
- Entry Time
- Exit Time
- Lunch Break Time
- Absence Records
- Weekend Records

---

## Office Timing

| Activity | Time |
|----------|------|
| Office Entry | 8:45 AM – 9:15 AM |
| Lunch Out | 1:00 PM – 1:10 PM |
| Lunch In | 2:30 PM – 2:45 PM |
| Office Exit | 5:00 PM – 5:30 PM |

---

## Work Policy
- Monthly workdays: **22**
- Friday is treated as weekend
- Random attendance generation
- Approximately 5% absence probability

---

# Salary Calculation

The system automatically calculates:
- Gross Salary
- Per-Day Salary
- Salary Deduction
- Net Salary

## Formula

```cpp
Per Day Salary = Monthly Salary / 22
Deduction = Absent Days × Per Day Salary
Net Salary = Salary - Deduction
```

---

# Technologies Used

- C++
- STL Vector
- STL Map
- Object-Oriented Programming (OOP)
- Random Number Generation
- Console Application

---

# Project Structure

```text
Employee Management System
│
├── Date Structure
├── TimePoint Structure
├── DailyAttendance Structure
├── Department Enum
├── Employee Class
├── MediCarePharma Class
└── Main Function
```

---

# How to Run

## Step 1: Compile

Using g++:

```bash
g++ main.cpp -o pharma
```

---

## Step 2: Run

For Linux/Mac:

```bash
./pharma
```

For Windows:

```bash
pharma.exe
```

---

# Menu System

```text
1. View Employee Monthly Report
2. Department Headcount
3. Exit
```

---

# Sample Employee Report

```text
Employee ID   : 10025
Name          : Rahim Ahmed
Department    : Marketing
Monthly Salary: BDT 55000

Day  Date        Status    Entry    Exit
------------------------------------------------
1    01/05/2024 Present   08:50    17:10
2    02/05/2024 Present   09:05    17:20
3    03/05/2024 ABSENT
```

---

# Learning Concepts Covered

This project is useful for practicing:

- C++ OOP
- Encapsulation
- Class Design
- Struct Usage
- Maps & Vectors
- Random Data Simulation
- Attendance Systems
- Payroll Logic
- Real-world HR System Design

---

# Future Improvements

Possible future upgrades:
- File handling
- Database integration
- Login system
- GUI interface
- Employee search by name
- Export report to PDF
- Real-time attendance system
- Admin dashboard

---

# Author

**Shams-uz-zaman Sharit**

C++ Developer | AI & Software Enthusiast

---

# License

This project is created for educational and learning purposes.
