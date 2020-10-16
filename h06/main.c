#include <stdio.h>
#include <stdlib.h>

// specification1: the max size of the input string is determined by MAXLEN enum
// specification2: newline symbol (code 10 in ascii) is considered a part of am
// input string specification3: the mount of nodes in a tree fits in an unsigned
// int specification4: character can not be encountered more than MAX_INT times
// specification5: if malloc()/realloc() fails to allocate/reallocate memory the
// program exits immideately

enum { MAXLEN = 256 };

/**
 * \brief structure of a node of a tree
 */
struct TreeNode {
    unsigned char c;         /**< character as a key to the tree*/
    int n;                   /**< amoutnt of times we've seen c*/
    struct TreeNode *left;   /**< pointer to left subtree*/
    struct TreeNode *right;  /**< pointer to right subtree*/
    struct TreeNode *parent; /**< pointer to the parent of the node*/
};

/**
 * \brief add elements to the tree recursively
 * \param in starting node. Always feed the root of the tree
 * \param ch character needed to be added to the tree
 */

void add_element_rec(struct TreeNode **in, unsigned char ch) {
    if (*in == NULL) {
        *in = malloc(sizeof(struct TreeNode));
        (*in)->c = ch;
        (*in)->n = 1;
        (*in)->left = NULL;
        (*in)->right = NULL;
        (*in)->parent = NULL;
        return;
    }
    if ((*in)->c == ch)
        (*in)->n++;
    else if (((*in)->c < ch) && ((*in)->right != NULL))
        add_element_rec(&((*in)->right), ch);
    else if (((*in)->c < ch) && ((*in)->right == NULL)) {
        struct TreeNode *new = malloc(sizeof(struct TreeNode));
        new->c = ch;
        new->n = 1;
        new->left = NULL;
        new->right = NULL;
        new->parent = *in;
        (*in)->right = new;
    } else if (((*in)->c > ch) && ((*in)->left != NULL))
        add_element_rec(&((*in)->left), ch);
    else if (((*in)->c > ch) && ((*in)->left == NULL)) {
        struct TreeNode *new = malloc(sizeof(struct TreeNode));
        new->c = ch;
        new->n = 1;
        new->left = NULL;
        new->right = NULL;
        new->parent = *in;
        (*in)->left = new;
    }
    return;
}

/**
 * \brief print the tree in-order
 * \param cur the root of the subtree you want to print
 */
void tree_print(struct TreeNode *cur) {
    if (cur == NULL) return;
    tree_print(cur->left);
    printf("%c(%d) %d time(s)\n", cur->c, cur->c, cur->n);
    tree_print(cur->right);
}

/**
 * \brief delete the tree from memory
 * \param in the root of subtree you want to delete
 * \warning this solution is dumb and it is probably a better way to do it(there
 * is, see below) \warning adapted for c from
 * https://www.geeksforgeeks.org/non-recursive-program-to-delete-an-entire-binary-tree/
 */

void tree_delete(struct TreeNode *in) {
    int i;
    // Base Case
    if (in == NULL) return;

    // Create an empty queue for level order traversal
    struct TreeNode **q = malloc(sizeof(struct TreeNode));
    if (q == NULL) {
        printf("malloc error");
        exit(1);
    }
    unsigned int queue_size = 1;
    // Do level order traversal starting from root
    q[0] = in;
    while (queue_size > 0) {
        struct TreeNode *node = q[0];
        // q.pop();
        // printf("popping %c(%d)\n", q[0]->c, queue_size);
        for (i = 0; i < queue_size - 1; i++) q[i] = q[i + 1];
        q = realloc(q, (--queue_size) * sizeof(struct TreeNode));
        if (node->left != NULL) {
            // q.push(node->left);
            q = realloc(q, (++queue_size) * sizeof(struct TreeNode));
            if (q == NULL) {
                printf("realloc error");
                exit(1);
            }
            q[queue_size - 1] = node->left;
            // printf("adding left %c(%d)\n", q[queue_size - 1]->c, queue_size);
        }
        if (node->right != NULL) {
            // q.push(node->right);
            q = realloc(q, (++queue_size) * sizeof(struct TreeNode));
            if (q == NULL) {
                printf("realloc error");
                exit(1);
            }
            q[queue_size - 1] = node->right;
            // printf("adding right %c(%d)\n", q[queue_size - 1]->c,
            // queue_size);
        }
        free(node);
    }
    free(q);
}

/**
 * \brief deletes the tree utilizing the parent pointer
 * \param in the root of subtree you want to delete
 * \warning DO NOT PASS NULL TO THIS FUNCTION(consider this specification)
 */
void tree_delete_par(struct TreeNode *in) {
    struct TreeNode *node = in;
    struct TreeNode *par;
    while(1){
        while (!((node->left == NULL) && (node->right == NULL))) {
            if (node->left != NULL)
                node = node->left;
            else if (node->right != NULL)
                node = node->right;
        }
        par = node -> parent;
        if(par == NULL){
            free(node);
            break;
        }
        // remember lazy evaluation!
        if((par->right != NULL) && (par->right == node)) par->right = NULL;
        else if((par->left != NULL) && (par->left == node)) par->left = NULL;
        free(node);
        node = par;
    }
}

int main(void) {
    struct TreeNode *tree = NULL;
    unsigned char prompt[MAXLEN];
    fgets(prompt, MAXLEN, stdin);
    char *cur = prompt;
    while (*cur != '\0') {
        add_element_rec(&tree, *cur);
        cur++;
    }
    tree_print(tree);
    tree_delete_par(tree);
    return 0;
}