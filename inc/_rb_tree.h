#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include "_entity.h"

#define RED 0
#define BLACK 1

typedef struct _RBTreeNode {

   char color;
   Entity _entity;
   struct _RBTreeNode* parent;
   struct _RBTreeNode* left;
   struct _RBTreeNode* right;

}RBTreeNode;

typedef struct _RBTree {
   
   RBTreeNode* root;
   size_t    size;
   ecompare compare;
   
}RBTree;

extern int init_rbtree(RBTree* pt, ecompare func);

extern RBTreeNode* create_rbnode( Entity e);

extern RBTreeNode* rbtree_minimum (RBTreeNode* pn);

extern RBTreeNode* rbtree_maximum(RBTreeNode* pn);

extern RBTreeNode* rbtree_successor(RBTreeNode* ptnode);

extern RBTreeNode* rbtree_presuccessor(RBTreeNode* ptnode);

extern int left_rotate(RBTree* prb, RBTreeNode* px);

extern int right_rotate(RBTree* prb, RBTreeNode* px);

extern RBTreeNode* rb_search(RBTree* prb, RBTreeNode* pt, Entity e, Entity** entity);

extern int rb_insert_fixup(RBTree* prb, RBTreeNode* pz);

extern int rb_insert(RBTree* prb, Entity e);

extern int rb_delete_fixup(RBTree* prb, RBTreeNode* px, RBTreeNode* px_parent, int is_left);

extern RBTreeNode* rb_remove(RBTree* prb, RBTreeNode* pz);

extern int rb_delete(RBTree* prb, Entity e, Entity *entity);

#endif