#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_

#include "_entity.h"

typedef struct _TreeNode {
  Entity _entity;
  struct _TreeNode* parent;
  struct _TreeNode* left;
  struct _TreeNode* right;
} TreeNode;

typedef struct _Tree{
	TreeNode* root;
	size_t   size;
  Ecmp compare;
} Tree;

extern TreeNode* create_tnode(Entity kv);
extern int init_tree(Tree* ptree, Ecmp func);
extern int inorder_tree_walk (TreeNode* ptnode, int (*tnode_func)(TreeNode* ptnode));
extern TreeNode* tree_search(Tree *ptree, TreeNode* ptnode,  Entity e, Ecmp filter, Entity** entity);
extern TreeNode* tree_minimum (TreeNode* ptnode);
extern TreeNode* tree_maximum(TreeNode* ptnode);
extern TreeNode* tree_successor(TreeNode* ptnode);
extern TreeNode* tree_presuccessor(TreeNode* ptnode);
extern int tree_insert(Tree* pt, Entity e);
// 把节点移出，未做删除处理。
extern TreeNode* tree_remove(Tree* pt, TreeNode* pz);
// 把节点移出，并做删除处理。
extern int tree_delete(Tree* pt,  Entity e, Ecmp filter, Entity* _entity);
#endif
