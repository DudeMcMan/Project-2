// test_tree.cpp
// Tests for the department tree: loading, adding, searching, preorder.
// Author: Ryker
#include <cassert>
#include <fstream>
#include <iostream>
#include "../include/DepartmentTree.h"
using namespace std;

static void writeFile(const string& path, const string& text) {
    ofstream out(path.c_str(), ios::binary);
    out << text;
}

int main() {
    string reason;

    // Empty tree: the first add becomes the root, no parent needed.
    DepartmentTree t;
    assert(t.isEmpty() && t.size() == 0);
    assert(t.preorderList().empty());
    assert(t.addDepartment("University", "", reason));
    assert(t.size() == 1 && !t.isEmpty());          // root-only tree
    assert(t.find("university")->parent == nullptr);
    assert(t.depthOf(t.find("University")) == 0);

    // A bad parent, a duplicate, or a blank name shouldn't change the tree.
    assert(!t.addDepartment("Physics", "Nowhere", reason));
    assert(t.size() == 1 && reason.find("unknown parent") != string::npos);
    assert(!t.addDepartment("UNIVERSITY", "University", reason));
    assert(t.size() == 1 && reason.find("duplicate") != string::npos);
    assert(!t.addDepartment("   ", "University", reason));   // blank name
    assert(!t.addDepartment("Science", "", reason));         // blank parent
    assert(t.size() == 1);

    // The sample data with Windows line endings, extra spaces, a comment, and a blank line.
    writeFile("tmp_departments.txt",
        "# DepartmentName|ParentName\r\n"
        "\r\n"
        "University|NONE\r\n"
        "  College of Engineering | University \r\n"
        "College of Science|University\r\n"
        "Computer Science|College of Engineering\r\n"
        "Electrical Engineering|College of Engineering\r\n"
        "Mathematics|College of Science\r\n"
        "Physics|College of Science\r\n");
    DepartmentTree s;
    int acc = 0, rej = 0;
    s.loadFromFile("tmp_departments.txt", acc, rej);
    assert(acc == 7 && rej == 0 && s.size() == 7);
    vector<string> pre = s.preorderList();
    string expected[] = {"University", "College of Engineering", "Computer Science",
                         "Electrical Engineering", "College of Science", "Mathematics",
                         "Physics"};
    assert(pre.size() == 7);
    for (int i = 0; i < 7; ++i) assert(pre[i] == expected[i]);
    assert(s.depthOf(s.find("physics")) == 2);
    assert(s.find("College of Engineering")->children.size() == 2);
    assert(s.find("College of Engineering")->name == "College of Engineering");
    assert(s.find("Student  Union") == nullptr);
    assert(s.find("Biology") == nullptr);

    // Bad lines: wrong field count, empty fields, a duplicate with different
    // capitalization, unknown parent, NONE after the root. A '#' inside a name is fine.
    writeFile("tmp_bad.txt",
        "University|NONE\n"
        "Only One Field\n"
        "A|B|C\n"
        "|University\n"
        "Arts|\n"
        "university|NONE\n"
        "Law|Missing Parent\n"
        "Floating|NONE\n"
        "Room #5 Studies|University\n");
    DepartmentTree b;
    b.loadFromFile("tmp_bad.txt", acc, rej);
    assert(acc == 2 && rej == 7 && b.size() == 2);
    assert(b.find("Room #5 Studies") != nullptr);

    // The first good line has to be the root, so the orphan line gets rejected.
    writeFile("tmp_noroot.txt", "Orphan|University\nUniversity|NONE\n");
    DepartmentTree n;
    n.loadFromFile("tmp_noroot.txt", acc, rej);
    assert(acc == 1 && rej == 1 && n.size() == 1);

    // Missing and empty files shouldn't crash anything.
    DepartmentTree m;
    m.loadFromFile("does_not_exist.txt", acc, rej);
    assert(acc == 0 && rej == 0 && m.isEmpty());
    writeFile("tmp_empty.txt", "");
    DepartmentTree e;
    e.loadFromFile("tmp_empty.txt", acc, rej);
    assert(acc == 0 && rej == 0 && e.isEmpty());

    // Adding after a load works, and the display functions are fine on an empty tree.
    assert(s.addDepartment("Biology", "College of Science", reason));
    assert(s.size() == 8 && s.depthOf(s.find("Biology")) == 2);
    m.displayHierarchy();
    m.displayPreorder();
    m.displaySearchResult("x");
    s.displayHierarchy();
    s.displayPreorder();
    s.displaySearchResult("university");
    s.displaySearchResult("PHYSICS");

    remove("tmp_departments.txt"); remove("tmp_bad.txt");
    remove("tmp_noroot.txt"); remove("tmp_empty.txt");
    cout << "Tree tests passed" << endl;
    return 0;
}
