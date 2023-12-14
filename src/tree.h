#pragma once
// splaytree.h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct splaytree_node {
    int key;
    struct splaytree_node* left;
    struct splaytree_node* right;
    struct splaytree_node* parent;
} splaytree_node;

splaytree_node* splaytree_left_rotate(splaytree_node* x);
splaytree_node* splaytree_right_rotate(splaytree_node* y);
splaytree_node* splay(splaytree_node* x);
splaytree_node* createnode(int key);
splaytree_node* insert(splaytree_node* root, int key);
splaytree_node* splaytree_lookup(splaytree_node* root, int key);
splaytree_node* deleteNode(splaytree_node* root, int key);
void VisualizeDotFile(splaytree_node* root, const char* filenamePart);
void generateDotFile(FILE* file, splaytree_node* root);
