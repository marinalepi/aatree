#ifndef AATREE_H
#define AATREE_H

#include "aadata.h"

typedef AADataBase *(*createDataElement)(unsigned char *a, unsigned int s);

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
	void remove(const AADataBase *inp, AATreeNode* &node, AATreeNode* &found, AATreeNode* &last);
	void skew(AATreeNode* &node);
	void split(AATreeNode* &node);
	void erase(AATreeNode* &node);
	int load(ifstream &f, AATreeNode* &node, createDataElement foo);
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
	void remove(const AADataBase *inp) {
		AATreeNode *found = NULL;
		AATreeNode *last = NULL;
		return remove(inp, tree, found, last);
	}
	const AATreeNode *get() const {
		return tree;
	}
	const AATreeNode *get(const AADataBase *inp) const;
	unsigned int getNodeCount() const {
		return nodeCount;
	}
	void prettyPrint() const;
	void erase() {
		erase(tree);
	}
	int load(ifstream &f, createDataElement =  createAADataBase);
	int save(ofstream &f) const;
};
#endif // AATREE_H
