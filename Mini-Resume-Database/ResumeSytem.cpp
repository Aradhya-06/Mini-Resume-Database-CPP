#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

//lowercase ke liye helper function
string toLowerCase(string s) {
    for(char &c : s) c = tolower(c);
    return s;
}

class Candidate {
private:
    string ogname;
    string namekey;
    vector<string> ogskills;
    vector<string> skillskey;
    int experience;
    int age;

public:
    Candidate() {}

    Candidate(string n, vector<string> s, int exp, int a)
        : ogname(n), ogskills(s), experience(exp), age(a) 
    {
        namekey = toLowerCase(n);
        for (string sk : s) skillskey.push_back(toLowerCase(sk));
    }

    string getname() const { return ogname; }
    string getnamekey() const { return namekey; }
    vector<string> getskills() const { return ogskills; }
    vector<string> getskillkeys() const { return skillskey; }
    int getexp() const { return experience; }
    int getage() const { return age; }

    //display
    void display() const {
        cout << "\n\t\t\t\t\t=====================================================\n";
        cout << "\t\t\t\t\t                   RESUME DETAILS                    \n";
        cout << "\t\t\t\t\t=====================================================\n";
        cout << "\t\t\t\t\t Name: " << ogname << "\n";
        cout << "\t\t\t\t\t Age: " << age << "\n";
        cout << "\t\t\t\t\t Experience: " << experience << " years\n";
        cout << "\t\t\t\t\t Skills: ";
        for (auto &s : ogskills) cout << s << ", ";
        cout << "\n\t\t\t\t\t=====================================================\n";
    }

    //file handling
    string toFileString() const {
        stringstream ss;
        ss << ogname << "|" << age << "|" << experience << "|";
        for (int i = 0; i < ogskills.size(); i++) {
            ss << ogskills[i];
            if (i != ogskills.size() - 1) ss << ",";
        }
        return ss.str();
    }

    static Candidate fromFileString(const string &line) {
        stringstream ss(line);
        string n, ageStr, expStr, skillStr, token;
        int age, exp;
        vector<string> skills;

        getline(ss, n, '|');
        getline(ss, ageStr, '|');
        getline(ss, expStr, '|');
        getline(ss, skillStr);

        age = stoi(ageStr);
        exp = stoi(expStr);

        stringstream s2(skillStr);
        while (getline(s2, token, ',')) {
            if (!token.empty()) skills.push_back(token);
        }

        return Candidate(n, skills, exp, age);
    }
};

//avl tree (name wise)
struct AVLNode {
    Candidate* cand;
    AVLNode *left, *right;
    int height;
    AVLNode(Candidate* c) : cand(c), left(nullptr), right(nullptr), height(1) {}
};

class NameAVL {
private:
    AVLNode* root;
    int height(AVLNode* n) { return n ? n->height : 0; }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    AVLNode* insertRec(AVLNode* node, Candidate* cand) {
        if (!node) return new AVLNode(cand);

        if (cand->getnamekey() < node->cand->getnamekey())
            node->left = insertRec(node->left, cand);
        else
            node->right = insertRec(node->right, cand);

        return node;
    }

    void prefixsearchrec(AVLNode* node, const string& prefix, vector<Candidate*>& out) const {
        if (!node) return;
        if (node->cand->getnamekey().rfind(prefix, 0) == 0)
            out.push_back(node->cand);
        prefixsearchrec(node->left, prefix, out);
        prefixsearchrec(node->right, prefix, out);
    }

public:
    NameAVL() : root(nullptr) {}
    void insert(Candidate* cand) { root = insertRec(root, cand); }

    vector<Candidate*> prefixsearch(string prefix) const {
        prefix = toLowerCase(prefix);
        vector<Candidate*> result;
        prefixsearchrec(root, prefix, result);
        return result;
    }
};

// maxHeap for exp
class ExperienceHeap {
private:
    vector<Candidate*> heap;

    void heapifyDown(int idx) {
        int n = heap.size();
        int largest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < n && heap[left]->getexp() > heap[largest]->getexp())
            largest = left;
        if (right < n && heap[right]->getexp() > heap[largest]->getexp())
            largest = right;
        if (largest != idx) {
            swap(heap[idx], heap[largest]);
            heapifyDown(largest);
        }
    }

    void heapifyUp(int idx) {
        if(idx == 0) return;
        int parent = (idx - 1) / 2;
        if (heap[idx]->getexp() > heap[parent]->getexp()) {
            swap(heap[idx], heap[parent]);
            heapifyUp(parent);
        }
    }

public:
    void insert(Candidate* c) {
        heap.push_back(c);
        heapifyUp(heap.size() - 1);
    }

    Candidate* extractMax() {
        if (heap.empty()) return nullptr;
        Candidate* maxCand = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return maxCand;
    }

    void buildHeap(const vector<Candidate*>& arr) {
        heap = arr;
        int n = heap.size();
        for (int i = n/2-1; i >= 0; --i)
            heapifyDown(i);
    }

    vector<Candidate*> heapSort() {
        ExperienceHeap tempHeap = *this;
        vector<Candidate*> result;
        while (!tempHeap.heap.empty()) {
            result.push_back(tempHeap.extractMax());
        }
        return result;
    }
};


class ResumeSystem {
private:
    vector<Candidate*> allCandidates;
    NameAVL nameTree;
    unordered_map<string, vector<Candidate*>> skillMap;

public:
    void insertresume(Candidate* c) {
        allCandidates.push_back(c);
        nameTree.insert(c);
        for (auto &sk : c->getskillkeys())
            skillMap[sk].push_back(c);
    }

    vector<Candidate*> autoSuggest(const string& prefix) const {
        return nameTree.prefixsearch(prefix);
    }

    vector<Candidate*> searchbyskill(const string& skill) const {
        string key = toLowerCase(skill);
        if (skillMap.count(key)) return skillMap.at(key);
        return {};
    }

    vector<Candidate*> sortbyexp() const {
        ExperienceHeap heap;
        heap.buildHeap(allCandidates);
        return heap.heapSort();
    }

    vector<Candidate*> displayAll() const {
        return nameTree.prefixsearch(""); 
    }
};

//file handling
void saveToFile(const vector<Candidate*>& db, const string &filename) {
    ofstream out(filename);
    for (auto c : db) out << c->toFileString() << "\n";
}

void loadFromFile(vector<Candidate*>& db, const string& filename) {
    ifstream in(filename);
    string line;
    while (getline(in, line)) if (!line.empty())
        db.push_back(new Candidate(Candidate::fromFileString(line)));
}

int main() {
    ResumeSystem rs;
    vector<Candidate*> loaded;
    string filename = "resumes.txt";

    loadFromFile(loaded, filename);
    for (auto c : loaded) rs.insertresume(c);

    int choice;
    while(true) {
        cout << "\n\t\t\t\t\t=================== MINI RESUME DATABASE ===================";
        cout << "\n\t\t\t\t\t1. Insert New Resume";
        cout << "\n\t\t\t\t\t2. Search by Name (Auto-Suggest)";
        cout << "\n\t\t\t\t\t3. Search by Skill";
        cout << "\n\t\t\t\t\t4. Sort by Experience";
        cout << "\n\t\t\t\t\t5. Display All Candidates";
        cout << "\n\t\t\t\t\t6. Exit";
        cout << "\n\t\t\t\t\t============================================================";
        cout << "\n\t\t\t\t\tEnter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string name;
            int age, exp, count;
            vector<string> skills;

            cout << "\n\t\t\t\t\tEnter Name: ";
            getline(cin, name);

            cout << "\t\t\t\t\tEnter Age: ";
            cin >> age; cin.ignore();

            cout << "\t\t\t\t\tEnter Experience (years): ";
            cin >> exp; cin.ignore();

            cout << "\t\t\t\t\tNumber of Skills: ";
            cin >> count; cin.ignore();

            for (int i = 0; i < count; i++) {
                string skill;
                cout << "\t\t\t\t\tSkill " << i+1 << ": ";
                getline(cin, skill);
                skills.push_back(skill);
            }

            Candidate* c = new Candidate(name, skills, exp, age);
            rs.insertresume(c);
            loaded.push_back(c);
            saveToFile(loaded, filename);

            cout << "\n\t\t\t\t\t Resume Added Successfully!\n";
        }
        else if (choice == 2) {
            string prefix;
            cout << "\n\t\t\t\t\tEnter name prefix: ";
            getline(cin, prefix);

            auto result = rs.autoSuggest(prefix);

            if (result.empty())
                cout << "\n\t\t\t\t\tNo matching names found.\n";
            else {
                cout << "\n\t\t\t\t\tSuggestions:\n";
                for (auto c : result) c->display();
            }
        }
        else if (choice == 3) {
            string skill;
            cout << "\n\t\t\t\t\tEnter skill: ";
            getline(cin, skill);

            auto result = rs.searchbyskill(skill);

            if (result.empty())
                cout << "\n\t\t\t\t\tNo candidates found.\n";
            else 
                for (auto c : result) c->display();
        }
        else if (choice == 4) {
            cout << "\n\t\t\t\t\tSorted by Experience:\n";
            auto sorted = rs.sortbyexp();
            for (auto c : sorted) c->display();
        }
        else if (choice == 5) {
            auto all = rs.displayAll();
            if (all.empty()) cout << "\n\t\t\t\t\t No records found.\n";
            else {
               cout << "\n\t\t\t\t\tAll Stored Candidates:\n";
                for (auto c : all) c->display();
            }
        }
        else if (choice == 6) {
            cout << "\n\t\t\t\t\tSaved. Exiting...\n";
            break;
        }else{
            cout<<"\t\t\t\t\tInvalid choice!\n\n";
        }
    };

    return 0;
}
