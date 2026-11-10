// DepartmentTree.h
// The department hierarchy as a general tree (a node can have any number of
// children). Handles loading, adding, searching, and printing it.
// Author: Ryker
#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <string>
#include <vector>

// One department. Children stay in the order they were added.
struct DepartmentNode {
    std::string name;
    DepartmentNode* parent;                  // nullptr for the root
    std::vector<DepartmentNode*> children;   // any number of children

    explicit DepartmentNode(const std::string& deptName)
        : name(deptName), parent(nullptr) {}
};

class DepartmentTree {
public:
    DepartmentTree();
    ~DepartmentTree();

    // Reads departments.txt. Every rejected line gets a warning with the file name,
    // line number, and reason. If the file won't open we print an error and carry on
    // with an empty tree. The caller prints the summary line.
    void loadFromFile(const std::string& path, int& accepted, int& rejected);

    // Used by the menu. If the tree is empty the new department just becomes the
    // root (parentName is ignored). Otherwise the parent has to exist already.
    // On failure it returns false, fills in reason, and changes nothing.
    bool addDepartment(const std::string& name, const std::string& parentName,
                       std::string& reason);

    bool isEmpty() const;
    int size() const;

    // Looks a department up by name, ignoring case. nullptr if it isn't there.
    const DepartmentNode* find(const std::string& name) const;

    // How many steps down from the root (the root is 0).
    int depthOf(const DepartmentNode* node) const;

    // Prints the name, parent (None for the root), number of children, and depth.
    void displaySearchResult(const std::string& name) const;

    // Prints the whole tree, one per line, indented 4 spaces per level.
    void displayHierarchy() const;

    // Preorder order as a plain list, no indentation.
    std::vector<std::string> preorderList() const;
    void displayPreorder() const;

private:
    DepartmentNode* root;
    int count;

    DepartmentNode* findNode(const std::string& name) const;
    void attach(DepartmentNode* parent, const std::string& name);
    void destroy(DepartmentNode* node);
    void preorderCollect(const DepartmentNode* node, std::vector<std::string>& out) const;
    void printIndented(const DepartmentNode* node, int depth) const;

    // No copying, since the tree owns its nodes.
    DepartmentTree(const DepartmentTree&);
    DepartmentTree& operator=(const DepartmentTree&);
};

#endif
