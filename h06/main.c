#include <stdio.h>
#include <stdlib.h>

/**
 * \brief structure of a node of a tree
*/
struct TreeNode
{
    char c; /**< character as a key to the tree*/
    int n; /**< amoutnt of times we've seen c*/
    struct TreeNode *left; /**< pointer to left subtree*/
    struct TreeNode *right; /**< pointer to right subtree*/
};

/**
 * \brief add elements to the tree recursively
 * \param in starting node. Always feed the root of the tree
 * \param ch character needed to be added to the tree
 */

void add_element_rec(struct TreeNode** in, char ch){
    if(*in == NULL){
        *in = malloc(sizeof(struct TreeNode));
        (*in) -> c = ch;
        (*in) -> n = 1;
        (*in) -> left = NULL;
        (*in) -> right = NULL;
        return;
    }
    if((*in) -> c == ch){
        (*in) -> n++;
    }
    else if(((*in) -> c < ch) && ((*in) -> right != NULL)){
        add_element_rec(&((*in) -> right), ch);
    }
    else if (((*in) -> c < ch) && ((*in)->right == NULL)){
        struct TreeNode* new = malloc(sizeof(struct TreeNode));
        new -> c = ch;
        new -> n = 1;
        new -> left = NULL;
        new -> right = NULL;
        (*in) -> right = new;
    }
    else if (((*in) -> c > ch) && ((*in) -> left != NULL))
    {
        add_element_rec(&((*in) -> left), ch);
    }
    else if (((*in) -> c > ch) && ((*in) -> left == NULL))
    {
        struct TreeNode *new = malloc(sizeof(struct TreeNode));
        new -> c = ch;
        new -> n = 1;
        new -> left = NULL;
        new -> right = NULL;
        (*in) -> left = new;
    }
    return;
}


/**
 * \brief print the tree in-order
 * \param cur the root of the subtree you want to print
 */
void tree_print(struct TreeNode *cur)
{
    if (cur == NULL)
    {
        return;
    }
    tree_print(cur->left);
    printf("%c(%d) %d time(s)\n", cur->c, cur -> c, cur -> n);
    tree_print(cur->right);
}

/**
 * \brief delete the tree from memory
 * \param in the root of subtree you want to delete
 */

void tree_delete(struct TreeNode *in){
    if(in == NULL){
        return;
    }
    tree_delete(in -> right);
    tree_delete(in -> left);
    free(in);
}

int main(void){
    struct TreeNode* tree = NULL;
    add_element_rec(&tree, 5);
    add_element_rec(&tree, 4);
    add_element_rec(&tree, 6);
    tree_print(tree);
    tree_delete(tree);
    return 0;
}