# Mini Resume Database System (C++)

A console-based application that stores and manages candidate resumes using efficient Data Structures such as AVL Tree, Heap, and HashMap.

## 🔧 Features
- Insert new resume (Name, Age, Experience, Skills)
- Search by Name with auto-suggest (Prefix matching using AVL Tree)
- Search candidates by Skill (O(1) lookup using HashMap)
- Sort candidates by Experience (Max Heap)
- Display all stored candidates
- Persistent storage using File Handling (resumes.txt)

## 🧠 Data Structures & Technologies Used (Tech Stack)

| Component | Technology |
|------------|-----------|
| Programming Language | C++ |
| Name Search | AVL Tree (Prefix Search / Auto-Suggest) |
| Skill Search | HashMap (`unordered_map`) |
| Sorting | Max Heap (Heap Sort) |
| Storage | Vectors |
| File Handling | fstream (Text File Database) |
| OOP | Classes, Encapsulation |
| UI | Menu-driven Console Interface |

---

## 🔄 System Workflow

1. **Data Input**
   - Recruiter enters candidate details: Name, Age, Experience, Skills.

2. **Storage & Indexing**
   - Names stored in AVL Tree for fast prefix search.
   - Skills mapped using HashMap → Skill → List of Candidates.
   - All candidates stored in vector for heap-based sorting.
   - Data saved permanently in `resumes.txt`.

3. **Operations**
   - Insert Resume
   - Auto-Suggest Search by Name
   - Search by Skill
   - Sort by Experience (Heap)
   - Display All Resumes

4. **Output**
   - Formatted resume display in terminal.

---

## 📂 Project Structure
Mini-Resume-Database-CPP/
├── main.cpp
├── resumes.txt
└── README.md

## 📸 Sample Functionalities
- Insert Resume
- Auto-Suggest Search by Name
- Search by Skill
- Sort by Experience
- Display All Candidates

## 🌍 Real-Life Applications
- Recruitment portals
- HR resume screening systems
- Job portals (LinkedIn, Naukri, Indeed type backend logic)
- Applicant Tracking Systems (ATS)
- Efficient searching and filtering of large resume databases using optimized data structures.

## 🚀 Future Enhancements
- Replace AVL prefix search with full Trie implementation
- Add GUI using Qt
- Migrate storage from text file to SQL database
- Implement ranking using multiple parameters (skills + experience + education)
