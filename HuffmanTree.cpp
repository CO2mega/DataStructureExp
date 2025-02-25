#include "HuffmanTree.h"
#include <iostream>
#include <cstdlib>

void HuffmanTree_encode(HuffmanNode* huffman_tree, int* weight, int n) {
    // 初始化哈夫曼树
    for (int i = 0; i < 2 * n - 1; i++)
    {
        huffman_tree[i].left_child = -1;
        huffman_tree[i].right_child = -1;
        huffman_tree[i].parent = -1;
        huffman_tree[i].weight = 0;
        huffman_tree[i].flag = 0;
    }
    // 初始化叶子节点
    for (int i = 0; i < n; i++)
    {
        huffman_tree[i].weight = weight[i];
    }
    // 构建哈夫曼树
    for (int i = n; i < 2 * n - 1; i++)
    {
        int min1 = 0x7fffffff;
        int min2 = 0x7fffffff;
        int min1_index = -1;
        int min2_index = -1;
        for (int j = 0; j < i; j++)
        {
            if (huffman_tree[j].parent == -1 && huffman_tree[j].flag == 0)
            {
                if (huffman_tree[j].weight < min1)
                {
                    min2 = min1;
                    min2_index = min1_index;
                    min1 = huffman_tree[j].weight;
                    min1_index = j;
                }
                else if (huffman_tree[j].weight < min2)
                {
                    min2 = huffman_tree[j].weight;
                    min2_index = j;
                }
            }
        }
        huffman_tree[min1_index].parent = i;
        huffman_tree[min2_index].parent = i;
        huffman_tree[min1_index].flag = 1;
        huffman_tree[min2_index].flag = 1;
        huffman_tree[i].left_child = min1_index;
        huffman_tree[i].right_child = min2_index;
        huffman_tree[i].weight = huffman_tree[min1_index].weight + huffman_tree[min2_index].weight;
    }
}

void Tree2Code(HuffmanNode* huffman_tree, Code* huffman_code, int n) {
    Code* code = (Code*)malloc(sizeof(Code));
    if (code == NULL)
    {
        printf("内存分配失败");
        return;
    }
    int child, parent;
    for (int i = 0; i < n; i++)
    {
        code->start = 10000 - 1;
        code->weight = huffman_tree[i].weight;
        child = i;
        parent = huffman_tree[child].parent;
        while (parent != -1)
        {
            if (huffman_tree[parent].left_child == child)
            {
                code->bit[code->start] = 0;
            }
            else
            {
                code->bit[code->start] = 1;
            }
            code->start--;
            child = parent;
            parent = huffman_tree[child].parent;
        }
        code->start++;
        huffman_code[i] = *code;
        huffman_code[i].start = code->start;
        huffman_code[i].weight = code->weight;
    }
    free(code);
}
