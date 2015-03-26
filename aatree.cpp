#include <stdio.h>
#include <math.h>
#include <queue>
#include <errno.h>
#include <aatree.h>
#include <assert.h>
#include <fstream>

using namespace std;

#ifndef MIN
#define MIN(a, b) ((a < b) ? a : b)
#endif
#ifndef MAX
#define MAX(a, b) ((a > b) ? a : b)
#endif

void AATree::skew(AATreeNode* &node) {
	// rotate right
	if (!node || !node->left) {
		return;
	}
    
	if (node->left->level == node->level) {
        // Swap the pointers of horizontal left links.
        AATreeNode *temp = node;
		node = node->left;
		temp->left = node->right;
		node->right = temp;
	}
}

void AATree::split(AATreeNode * &node) {
	// rotate left
	if (!node || !node->right || !node->right->right) {
		return;
	}
	
    if (node->level == node->right->right->level) {
        // We have two horizontal right links.  Take the middle node, elevate it, and return it.
		AATreeNode *temp = node;
		node = node->right;
		temp->right = node->left;
		node->left = temp;
		node->level++;
	}
}

int AATree::insert(AADataBase *inp, AATreeNode* &node) {
	if (!inp) {
		return EINVAL;
	}
	
    // Do the normal binary tree insertion procedure. Set the result of the
    // recursive call to the correct child in case a new node was created or the
    // root of the subtree changes.
		
	if (!node) {
        // Create a new leaf node with X.
		node = new AATreeNode(inp);
		if (!node) {
			return ENOMEM;
		}
		nodeCount++;
		return 0;
	} 
    
	int cmpResult = inp->compare(node->data);
	if (cmpResult == 0) {
		return node->data->onequal(inp);
	}
	
	int insertResult;
	if (cmpResult == -1) { 
        insertResult = insert(inp, node->left);
	}
    else { 
        insertResult = insert(inp, node->right);
	}
	if (insertResult != 0) {
		return insertResult;
	}

    // Perform skew and then split. The conditionals that determine whether or
    // not a rotation will occur or not are inside of the procedures, as given
    // above.
    skew(node);
    split(node);
	return 0;
}

void AATree::remove(AADataBase *inp, AATreeNode* &node, AATreeNode* &found, AATreeNode* &last) {
	if (!node) {
        return;
	}
	if (!inp) {
		return;
	}
	
	last = node;
	int cmpResult = inp->compare(node->data);
    if (cmpResult == -1) {
        remove(inp, node->left, found, last);
	} else {
		if (!found && cmpResult == 0) {
			found = node;
		}
        remove(inp, node->right, found, last);
    } 
	
	// If we're a leaf and the node to be deleted is found on the path to the leaf, 
	// remove that node
	if (node == last && found) {
		found->data = node->data;
		found = NULL;
		node = node->right;
		delete last->data;
		delete last;
		last = NULL;
		nodeCount--;
	} else if (node->left->level  < node->level-1 ||
	           node->right->level < node->level-1) {
		// Rebalance the tree. Decrease the level of all nodes in this level if
		// necessary, and then skew and split all nodes in the new level.
		node->level--;
		if (node->right->level > node->level) {
			node->right->level = node->level;
		}
		skew(node);
		skew(node->right);
		skew(node->right->right);
		split(node);
		split(node->right);
	}
}

AATreeNode *AATree::get(AADataBase *inp) const {
	AATreeNode *node = tree;
	
	while (node) {
		int cmpResult = inp->compare(node->data);
		if (cmpResult == 0) {
			return node;
		}
		if (cmpResult == -1) {
			node = node->left;
		} else {
			node = node->right;
		}
	}
	return NULL;
}

void printNodeText(string text, unsigned int curLevel, vector <string> &levels) {
	unsigned int space = pow(2, curLevel)*2;
	if (text.size() < space) {
		// the text will fit
		// get the start point
		unsigned int start = (space-text.size())/2;
		text.insert(0, start, ' ');
		text.append(space-(start+text.size()), ' ');
	} else {
		// the text does not fit, cut it
		text.resize(space-1);
		text[space-2] = '.';
		text[space-1] = ' ';
	}
	levels[curLevel].append(text);
}

void AATree::prettyPrint() const {
	if (!tree) {
		printf ("Empty tree\n");
		return;
	}
	
	// do breadth first tree scan
	queue<AATreeNode* > q;
	q.push(tree);
	
	vector <string> levels;
	levels.resize(tree->level+1);
	
	while (!q.empty()) {
		AATreeNode *cur = q.front();
		q.pop();
		if (!cur) {
			continue;
		}

		string text = cur->data->toString();
		if (cur->right && cur->right->level == cur->level) {
			text.append("-->");
		}
		unsigned int space = pow(2, cur->level)*3;
		if (text.size() < space) {
			// the text will fit
			// get the start point
			unsigned int start = (space-text.size())/2;
			text.insert(0, start, ' ');
			text.append(space-(start+text.size()), ' ');
		} else {
			// the text does not fit, cut it
			text.resize(space-1);
			text[space-2] = '.';
			text[space-1] = ' ';
		}
		levels[cur->level].append(text);

		q.push(cur->left);
		q.push(cur->right);
	}
	for (int i=tree->level; i>=1; i--) {
		printf ("%s\n", levels[i].c_str());
	}
	printf ("\n--\n");
}

void AATree::erase(AATreeNode* &node) {
	if (!node) {
        return;
	}
	
	erase(node->left);
	erase(node->right);

	delete node->data;
	delete node;
	node = NULL;
}

int AATree::load(ifstream &f, AATreeNode* &node, AADataBase *(*createData)(unsigned char *a, unsigned int s)) {
	node = NULL;
	// read size
	unsigned int size;
	f.read(reinterpret_cast< char *>(&size), sizeof(size));
	if (f.fail()) {
		return EIO;
	}
	// if 0, set node to NULL, return
	if (size == 0) {
		return 0;
	}
	//allocate memory
	node = new AATreeNode();
	if (!node) {
		return ENOMEM;
	}
	unsigned char *a = new unsigned char[size];
	if (!a)  {
		return ENOMEM;
	}
	// read data bits
	f.read((char*)a, size);
	if (f.fail()) {
		return EIO;
	}
	// set node data
	node->data = (*createData)(a, size);	
	if (!node->data) {
		return ENOMEM;
	}
	// read level
	f.read(reinterpret_cast<char *>(&node->level), sizeof(node->level));
	if (f.fail()) {
		return EIO;
	}
	nodeCount++;
	// read left
	int ret = load(f, node->left, createData);
	// read right
	return ret == 0? load(f, node->right, createData) : ret;
}

int AATree::load(const char *fName, AADataBase *(*createData)(unsigned char *a, unsigned int s)) {
	ifstream f;
	f.open(fName, ios_base::in | ios_base::binary);
	if (!f.is_open()) {
		return errno;
	}
	
	erase(tree);
	int ret = load(f, tree, createData);
	
	f.close();
	return ret;

}

int AATree::save(ofstream &f, AATreeNode* node) const {
	unsigned int size;
	if (!node) {
		size = 0;
		f.write(reinterpret_cast<const char *>(&size), sizeof(size));
		if (f.fail()) {
			return EIO;
		}
		return 0;
	}

	// get data size and byte array representation
	const unsigned char *a = node->data->getBytes(size);
	if (!a) {
		return ENOMEM;
	}
	
	// save size, then array
	f.write(reinterpret_cast<const char *>(&size), sizeof(size));
	if (f.fail()) {
		return EIO;
	}
	
	f.write((char*)a, size);
	if (f.fail()) {
		return EIO;
	}
	// save level
	f.write(reinterpret_cast<const char *>(&node->level), sizeof(node->level));
	if (f.fail()) {
		return EIO;
	}
	delete[] a;
	
	int ret = save(f, node->left);
	return ret == 0? save(f, node->right) : ret;
}	

int AATree::save(const char *fName) const {
	ofstream f;
	f.open(fName, ios_base::out | ios_base::binary);
	if (!f.is_open()) {
		return errno;
	}
		
	int ret = save(f, tree);

	f.close();
	return ret;
}

