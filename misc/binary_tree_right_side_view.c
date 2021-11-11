/*
 * Source: https://leetcode.com/problems/binary-tree-right-side-view
 
 * Input binary tree example:

               1
              / \
             2   3
              \   \
               5   4
*/

int* bfs(struct TreeNode* root, int* returnSize)
{
	queue_t *q = alloc_queue();
	int *out = NULL;
	struct TreeNode *node = NULL;
	int out_pos = 0, len = 0;

	if (root)
		enqueue(q, root); 

	while (!is_empty(q)) {
		len = queue_size(q);
		while(len--) {
			node = dequeue(q);

			if (node->left)
				enqueue(q, node->left); 

			if (node->right)
				enqueue(q, node->right);
		}

		out = realloc(out, (out_pos + 1) * sizeof(int));
		out[out_pos++] = node->val;
	}

	*returnSize = out_pos;
	return out;
}

int* rightSideView(struct TreeNode* root, int* returnSize){    
	return bfs(root, returnSize);
}
