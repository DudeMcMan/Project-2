// DepartmentTree.cpp
// Department tree code. The header says what each function does.
// Author: Ryker
#include "../include/DepartmentTree.h"
#include "../include/Utils.h"

#include <fstream>
#include <iostream>

using namespace std;

DepartmentTree::DepartmentTree() : root(nullptr), count(0) {}

// Frees every node, children first.
DepartmentTree::~DepartmentTree() {
    destroy(root);
}

void DepartmentTree::destroy(DepartmentNode* node) {
    if (node == nullptr) return;
    for (size_t i = 0; i < node->children.size(); ++i) destroy(node->children[i]);
    delete node;
}

bool DepartmentTree::isEmpty() const { return root == nullptr; }

int DepartmentTree::size() const { return count; }

// Walks the tree looking for a name, ignoring case.
DepartmentNode* DepartmentTree::findNode(const string& name) const {
    if (root == nullptr) return nullptr;
    vector<DepartmentNode*> stack;
    stack.push_back(root);
    while (!stack.empty()) {
        DepartmentNode* cur = stack.back();
        stack.pop_back();
        if (Utils::equalsIgnoreCase(cur->name, name)) return cur;
        for (size_t i = cur->children.size(); i > 0; --i) stack.push_back(cur->children[i - 1]);
    }
    return nullptr;
}

const DepartmentNode* DepartmentTree::find(const string& name) const {
    return findNode(Utils::trim(name));
}

// Makes a node and hooks it in as the parent's last child (or as the root).
void DepartmentTree::attach(DepartmentNode* parent, const string& name) {
    DepartmentNode* node = new DepartmentNode(name);
    if (parent == nullptr) {
        root = node;
    } else {
        node->parent = parent;
        parent->children.push_back(node);
    }
    ++count;
}

bool DepartmentTree::addDepartment(const string& rawName, const string& rawParent,
                                   string& reason) {
    string name = Utils::trim(rawName);
    string parentName = Utils::trim(rawParent);

    if (name.empty()) {
        reason = "department name is empty";
        return false;
    }
    if (findNode(name) != nullptr) {
        reason = "duplicate department name '" + name + "'";
        return false;
    }
    if (root == nullptr) {          // empty tree, so this one is the root
        attach(nullptr, name);
        return true;
    }
    if (parentName.empty()) {
        reason = "parent name is empty";
        return false;
    }
    DepartmentNode* parent = findNode(parentName);
    if (parent == nullptr) {
        reason = "unknown parent '" + parentName + "'";
        return false;
    }
    attach(parent, name);
    return true;
}

void DepartmentTree::loadFromFile(const string& path, int& accepted, int& rejected) {
    accepted = 0;
    rejected = 0;

    ifstream in(path.c_str());
    if (!in.is_open()) {
        cout << "ERROR: Could not open file: " << path << endl;
        return;
    }

    string line;
    int lineNo = 0;
    while (getline(in, line)) {
        ++lineNo;
        if (Utils::isBlankLine(line) || Utils::isCommentLine(line)) continue;

        string reason;
        vector<string> f = Utils::splitFields(line);

        if (f.size() != 2) {
            reason = "expected 2 fields but found " + to_string(f.size());
        } else if (f[0].empty()) {
            reason = "department name is empty";
        } else if (f[1].empty()) {
            reason = "parent name is empty";
        } else if (findNode(f[0]) != nullptr) {
            reason = "duplicate department name '" + f[0] + "'";
        } else if (root == nullptr) {
            // The first good line has to be the root, with NONE as its parent.
            if (!Utils::equalsIgnoreCase(f[1], "NONE"))
                reason = "first department must be the root with parent NONE";
            else
                attach(nullptr, f[0]);
        } else if (Utils::equalsIgnoreCase(f[1], "NONE")) {
            reason = "only the root may use parent NONE";
        } else {
            DepartmentNode* parent = findNode(f[1]);
            if (parent == nullptr)
                reason = "unknown parent '" + f[1] + "' (must appear on an earlier line)";
            else
                attach(parent, f[0]);
        }

        if (reason.empty()) {
            ++accepted;
        } else {
            ++rejected;
            cout << "WARNING: " << path << " line " << lineNo << ": " << reason << endl;
        }
    }
}

int DepartmentTree::depthOf(const DepartmentNode* node) const {
    int depth = 0;
    while (node != nullptr && node->parent != nullptr) {
        node = node->parent;
        ++depth;
    }
    return depth;
}

void DepartmentTree::displaySearchResult(const string& name) const {
    const DepartmentNode* node = find(name);
    if (node == nullptr) {
        cout << "Department not found: " << Utils::trim(name) << endl;
        return;
    }
    cout << "Department: " << node->name << endl;
    cout << "Parent: " << (node->parent ? node->parent->name : string("None")) << endl;
    cout << "Direct children: " << node->children.size() << endl;
    cout << "Depth: " << depthOf(node) << endl;
}

void DepartmentTree::printIndented(const DepartmentNode* node, int depth) const {
    cout << string(static_cast<size_t>(depth) * 4, ' ') << node->name << endl;
    for (size_t i = 0; i < node->children.size(); ++i)
        printIndented(node->children[i], depth + 1);
}

void DepartmentTree::displayHierarchy() const {
    if (root == nullptr) {
        cout << "No departments are loaded." << endl;
        return;
    }
    printIndented(root, 0);
}

void DepartmentTree::preorderCollect(const DepartmentNode* node, vector<string>& out) const {
    out.push_back(node->name);
    for (size_t i = 0; i < node->children.size(); ++i)
        preorderCollect(node->children[i], out);
}

vector<string> DepartmentTree::preorderList() const {
    vector<string> out;
    if (root != nullptr) preorderCollect(root, out);
    return out;
}

void DepartmentTree::displayPreorder() const {
    if (root == nullptr) {
        cout << "No departments are loaded." << endl;
        return;
    }
    vector<string> seq = preorderList();
    cout << "Preorder: ";
    for (size_t i = 0; i < seq.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << seq[i];
    }
    cout << endl;
}
