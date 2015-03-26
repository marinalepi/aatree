#ifndef AATREE_H
#define AATREE_H

#include "aadata.h"

struct AATreeNode {
	AADataBase *data;
	unsigned int level;
	AATreeNode *left;
	AATreeNode *right;
	AATreeNode() :
		data(NULL), level(1), left(NULL), right(NULL) {
	}
	AATreeNode(AADataBase *dataInit) :
		level(1), left(NULL), right(NULL) {
		data = dataInit;
	}
};

class AATree {
private:
	AATreeNode *tree;
	unsigned int nodeCount;
	
	int insert(AADataBase *inp, AATreeNode* &node);
	void remove(AADataBase *inp, AATreeNode* &node, AATreeNode* &found, AATreeNode* &last);
	void skew(AATreeNode* &node);
	void split(AATreeNode* &node);
	void erase(AATreeNode* &node);
	int load(ifstream &f, AATreeNode* &node, AADataBase * (*createData)(unsigned char *a, unsigned int s));
	int save(ofstream &f, AATreeNode* node) const;
	
public:
	AATree() {
		tree = NULL;
		nodeCount = 0;
	}
	~AATree() {
		erase();
	}
	
	int insert(AADataBase *inp) {
		return insert(inp, tree);
	}
	void remove(AADataBase *inp) {
		AATreeNode *found = NULL;
		AATreeNode *last = NULL;
		return remove(inp, tree, found, last);
	}
	AATreeNode *get() const {
		return tree;
	}
	AATreeNode *get(AADataBase *inp) const;
	unsigned int getNumNode() const {
		return nodeCount;
	}
	void prettyPrint() const;
	void erase() {
		erase(tree);
	}
	int load(const char *fName, AADataBase *(*createData)(unsigned char *a, unsigned int s) =  createAADataBase);
	int save(const char *fName) const;
};
#endif // AATREE_H
