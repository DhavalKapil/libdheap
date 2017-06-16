#include "chunk.h"
#include "libc_malloc.h"

/**
 * Returns a node structure wrapping a chunk
 *
 * @param ch The chunk
 *
 * @return The node structure
 */
struct node *create_node (struct chunk *ch) {
  struct node *n = _libc_malloc(sizeof(struct node));
  if (n == NULL) {
    return NULL;
  }
  n->ch = ch;
  n->height = 1;
  n->left = NULL;
  n->right = NULL;
  return n;
}

/**
 * Get max of two numbers
 */
int max (int n1, int n2) {
  return n1 > n2 ? n1 : n2;
}

/**
 * Get height, returns 0 for NULL nodes
 */
int height (struct node *root) {
  return root ? root->height : 0;
}

/**
 * Adjusts height
 */
void adjust_height (struct node *root) {
  root->height = 1 + max(height(root->left), height(root->right));
}

/**
 * Rotates right
 */
struct node *rotate_right (struct node *root) {
  struct node *temp = root->left;

  root->left = temp->right;
  temp->right = root;

  adjust_height(root);
  adjust_height(temp);

  return temp;
}

/**
 * Rotates left
 */
struct node *rotate_left (struct node *root) {
  struct node *temp = root->right;

  root->right = temp->left;
  temp->left = root;

  adjust_height(root);
  adjust_height(temp);

  return temp;
}

/**
 * Balances a subtree
 *
 * @param root The root node
 *
 * @return The new/updated root node
 */
struct node *balance (struct node *root) {
  adjust_height(root);

  if ( (height(root->left) - height(root->right)) == 2) {
    if ( height(root->left->right) > height(root->left->left) ) {
      root->left = rotate_left(root->left);
    }
    return rotate_right(root);
  } else if ( (height(root->right) - height(root->left)) == 2) {
    if ( height(root->right->left) > height(root->right->right) ) {
      root->right = rotate_right(root->right);
    }
    return rotate_left(root);
  }
  return root;
}

/**
 * Finds minimum node in subtree
 *
 * @param root The root of the subtree
 *
 * @return The minimum node
 */
struct node *find_minimum_node (struct node *root) {
  if (root->left != NULL) {
    return find_minimum_node(root->left);
  } else {
    return root;
  }
}

/**
 * Removes the minimum node from a subtree
 * This only removes from the tree, does not free the memory
 *
 * @param root The root of the subtree
 *
 * @return The root of the new subtree
 */
struct node *remove_minimum_node (struct node *root) {
  if (root->left == NULL) {
    return root->right;
  }
  root->left = remove_minimum_node(root->left);
  return balance(root);
}

/**
 * Same as insert_chunk but manages nodes
 */
struct node *insert_node (struct node *root, struct node *n) {
  if (root == NULL) {
    root = n;
    return root;
  }

  if (n->ch->ptr < root->ch->ptr) {
    root->left = insert_node(root->left, n);
  } else if ( n->ch->ptr > root->ch->ptr) {
    root->right = insert_node(root->right, n);
  }

  return balance(root);
}

/**
 * Same as remove_chunk but manages nodes
 */
struct node *remove_node (struct node *root, struct node *n) {
  struct node *left;
  struct node *right;
  struct node *min_node;

  if (n->ch->ptr < root->ch->ptr) {
    root->left = remove_node(root->left, n);
  } else if (n->ch->ptr > root->ch->ptr) {
    root->right = remove_node(root->right, n);
  } else {
    // Removing 'root' from its subtree
    left = root->left;
    right = root->right;
    _libc_free(root);

    if (right == NULL) {
      return left;
    }

    min_node = find_minimum_node(right);
    min_node->right = remove_minimum_node(right);
    min_node->left = left;

    return balance(min_node);
  }

  return balance(root);
}

/**
 * Same as find_node but manages nods
 */
struct node *find_node_in_tree (struct node *root, void *ptr) {
  if (root == NULL) {
    return NULL;
  }

  if (ptr < root->ch->ptr) {
    return find_node_in_tree (root->left, ptr);
  } else if (ptr > root->ch->ptr) {
    return find_node_in_tree (root->right, ptr);
  } else {
    // Found node
    return root;
  }
}

/**
 * Same as check_overlap but manages nodes
 */
int check_overlap_in_nodes (struct node *root, struct chunk *ch) {
  if (root == NULL) {
    return 0;
  }

  if ( CHUNK_END(ch) < CHUNK_BEGIN(root->ch) ) {
    return check_overlap_in_nodes(root->left, ch);
  } else if ( CHUNK_BEGIN(ch) > CHUNK_END(root->ch) ) {
    return check_overlap_in_nodes(root->right, ch);
  } else {
    // Chunk ch overlaps with chunk root->ch!!
    return 1;
  }
}

// Implementing functions defined in header file

void init_chunks (chunks_storage *chunks) {
  chunks->root = NULL;
}

int insert_chunk (chunks_storage *chunks, struct chunk *ch) {
  struct node *n;

  n = create_node (ch);
  if (n == NULL) {
    return 0;
  }

  if (chunks->root == NULL) {
    // This is the first insert operation
    // Tree is empty
    chunks->root = n;
    return 1;
  }

  chunks->root = insert_node(chunks->root, n);
  return 1;
}

void remove_chunk (chunks_storage *chunks, struct chunk *ch) {
  struct node *n;

  if (chunks->root == NULL) {
    return;
  }

  n = find_node(chunks, ch->ptr);
  chunks->root = remove_node(chunks->root, n);
}

struct node *find_node (chunks_storage *chunks, void *ptr) {
  return find_node_in_tree (chunks->root, ptr);
}


int check_overlap (chunks_storage *chunks, struct chunk *ch) {
  if (chunks->root == NULL) {
    return 0;
  }

  return check_overlap_in_nodes (chunks->root, ch);
}