#include "tree.h"

void left_rotate(splaytree_node* x) {
    splaytree_node* y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        // x был корнем дерева
        // y теперь становится корнем
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Вспомогательная функция для поворота вправо
void right_rotate(splaytree_node* y) {
    splaytree_node* x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        // y был корнем дерева
        // x теперь становится корнем
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

// Основная функция splay
splaytree_node* splay(splaytree_node* x) {
    while (x->parent != NULL) {
        splaytree_node* parent = x->parent;
        splaytree_node* grandparent = parent->parent;

        if (grandparent == NULL) {
            // Zig case
            if (x == parent->left) {
                right_rotate(parent);
            } else {
                left_rotate(parent);
            }
        } else {
            if (x == parent->left && parent == grandparent->left) {
                // Zig-Zig case
                right_rotate(grandparent);
                right_rotate(parent);
            } else if (x == parent->right && parent == grandparent->right) {
                // Zig-Zig case
                left_rotate(grandparent);
                left_rotate(parent);
            } else {
                // Zig-Zag case
                if (x == parent->left) {
                    right_rotate(parent);
                    left_rotate(grandparent);
                } else {
                    left_rotate(parent);
                    right_rotate(grandparent);
                }
            }
        }
    }

    return x;
}



splaytree_node* createnode(int key) {
    splaytree_node* new_node = (splaytree_node*)malloc(sizeof(splaytree_node));
    new_node->key = key;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    return new_node;
}

splaytree_node* insert(splaytree_node* root, int key) {
    if (root == NULL) {
        return createnode(key);
    }

    if (key < root->key) {
        if (root->left == NULL) {
            splaytree_node* new_node = createnode(key);
            new_node->parent = root;
            root->left = new_node;
            return splay(new_node);
        } else {
            return insert(root->left, key);
        }
    } else if (key > root->key) {
        if (root->right == NULL) {
            splaytree_node* new_node = createnode(key);
            new_node->parent = root;
            root->right = new_node;
            return splay(new_node);
        } else {
            return insert(root->right, key);
        }
    }

    return splay(root);
}

splaytree_node* splaytree_lookup(splaytree_node* root, int key) {
    if (root == NULL) {
        return NULL; // Если дерево пусто, возвращаем NULL
    }

    splaytree_node* current = root;
    while (current != NULL && current->key != key) {
        if (key < current->key && current->left != NULL) {
            current = current->left;
        } else if (key > current->key && current->right != NULL) {
            current = current->right;
        } else {
            printf("Узел с заданным ключом не найден\n");
            break; // Если ключ не найден, прерываем цикл
        }
    }

    // Выполняем splay для последнего посещенного узла (найденного или ближайшего)
    return splay(current);
}


splaytree_node* findMax(splaytree_node* root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

// Удалить узел с ключом key из дерева с корнем root
splaytree_node* deleteNode(splaytree_node* root, int key) {
    if (root == NULL) {
        return NULL;
    }

    // Ищем узел, который нужно удалить
    root = splaytree_lookup(root, key);

    // Если ключ не найден, то ничего не удаляем
    if (root->key != key) {
        return root;
    }

    // Найденный узел - корень, значит удаляем его
    splaytree_node* temp = root;
    if (root->left == NULL) {
        root = root->right;
        if (root != NULL) {
            root->parent = NULL; // Устанавливаем родителя для нового корня
        }
    } else {
        root = splay(root->left);
        root->right = temp->right;
        if (temp->right != NULL) {
            temp->right->parent = root; // Устанавливаем родителя для правого поддерева
        }
    }

    free(temp);
    return root;
}


void generateDotFile(FILE* file, splaytree_node* root) {
    if (root != NULL) {
        fprintf(file, "%d [label=\"%d\"]\n", root->key, root->key);

        if (root->left != NULL) {
            fprintf(file, "%d -> %d\n", root->key, root->left->key);
            generateDotFile(file, root->left);
        } else {
            if (root->right != NULL) {
                fprintf(file, "null_left%d [shape=point];\n", root->key);
                fprintf(file, "%d -> null_left%d\n", root->key, root->key);
            }
        }

        if (root->right != NULL) {
            fprintf(file, "%d -> %d\n", root->key, root->right->key);
            generateDotFile(file, root->right);
        } else {
            if (root->left != NULL) {
                fprintf(file, "null_right%d [shape=point];\n", root->key);
                fprintf(file, "%d -> null_right%d\n", root->key, root->key);
            }
        }
    }
}

void VisualizeDotFile(splaytree_node* root, const char* filenamePart)
{
    char filename[255];
    snprintf(filename, sizeof(filename), "bst_%s.dot", filenamePart);

    FILE* dotFile = fopen(filename, "w");
    if (dotFile == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(dotFile, "digraph BST {\n");
    generateDotFile(dotFile, root);
    fprintf(dotFile, "}\n");

    fclose(dotFile);

    char cmd[255];
    int k = snprintf(cmd, sizeof(cmd), "dot -Tpng %s -o %s.png", filename, filename);
    if (k < 0)
        abort();
    system(cmd);
}

int main() {
    splaytree_node* root = NULL;
    root = insert(root, 18);
    root = insert(root, 14);    
    root = insert(root, 12);
    VisualizeDotFile(root, "1");
    root = insert(root, 15);
    VisualizeDotFile(root, "2");
    root = splaytree_lookup(root, 11);
    VisualizeDotFile(root, "3");
    root = deleteNode(root, 14);
    VisualizeDotFile(root, "4");
    return 0;
}