#include <stdio.h>
#include <stdlib.h>

/**
 * \file
 * \warning all specifications will be in doxygen warnings
 * \warning the max size of the input string is 256 characters
 */

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
 * \warning this solution is dumb and it is probably a better way to do it
 * \warning adapted for c from https://www.geeksforgeeks.org/non-recursive-program-to-delete-an-entire-binary-tree/
 */

void tree_delete(struct TreeNode *in){
    int i;
    // Base Case
    if (in == NULL)
        return;

    // Create an empty queue for level order traversal
    struct TreeNode **q = malloc(3*sizeof(struct TreeNode));
    
    int queue_size = 1;
    // Do level order traversal starting from root
    q[0] = in;
    // printf("first element %c\n", q[0]->c);
    while (queue_size > 0)
    {
        struct TreeNode *node = q[0];
        // q.pop();
        // printf("popping %c\n", q[0] -> c);
        for(i = 0; i < queue_size-1; i++){
            q[i] = q[i+1];
        }
        // q = realloc(q, --queue_size);
        --queue_size;
        // for(i = 0; i < queue_size; i++)
        //     printf("%c", q[i] -> c);
        // printf("\n");
        if (node->left != NULL){
            // q.push(node->left);
            // q = realloc(q, ++queue_size);
            ++queue_size;
            q[queue_size - 1] = node->left;
            // printf("adding left %c\n", q[queue_size - 1]->c);
        }
        if (node->right != NULL){
            // q.push(node->right);
            // q = realloc(q, ++queue_size);
            ++queue_size;
            q[queue_size - 1] = node->right;
            // printf("adding right %c\n", q[queue_size - 1]->c);
        }
        free(node);
    }
    free(q);
}

int main(void)
{
    struct TreeNode *tree = NULL;
    char *prompt = malloc(256 * sizeof(char));
    scanf("%s", prompt);
    // printf("%s", prompt);
    char *cur = prompt;
    while (*cur != '\0')
    {
        add_element_rec(&tree, *cur);
        cur++;
    }
    free(prompt);
    tree_print(tree);
    tree_delete(tree);
    return 0;
}