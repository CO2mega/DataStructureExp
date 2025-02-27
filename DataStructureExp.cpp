﻿#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

typedef struct {
    int left;
    int right;
    int parent;
    int weight;
    int used;
} HuffmanNode;

typedef struct {
    char magic[4];    // 文件标识"HUF"
    int original_size;// 原始文件大小
    int weight[256];  // 字符频率表
} FileHeader;

// 哈夫曼树构建
void build_huffman_tree(HuffmanNode* tree, int* weights, int n) {
    // 初始化节点
    for (int i = 0; i < 2 * n - 1; ++i) {
        tree[i].left = tree[i].right = tree[i].parent = -1;
        tree[i].weight = 0;
        tree[i].used = 0;
    }

    for (int i = 0; i < n; ++i) {
        tree[i].weight = weights[i];
    }

    for (int i = n; i < 2 * n - 1; ++i) {
        int min1 = -1, min2 = -1;

        for (int j = 0; j < i; ++j) {
            if (!tree[j].used && tree[j].parent == -1) {
                if (min1 == -1 || tree[j].weight < tree[min1].weight) {
                    min2 = min1;
                    min1 = j;
                }
                else if (min2 == -1 || tree[j].weight < tree[min2].weight) {
                    min2 = j;
                }
            }
        }

        if (min1 == -1 || min2 == -1) break;

        tree[i].left = min1;
        tree[i].right = min2;
        tree[i].weight = tree[min1].weight + tree[min2].weight;
        tree[min1].parent = tree[min2].parent = i;
        tree[min1].used = tree[min2].used = 1;
    }
}

// 生成编码表
void generate_codes(HuffmanNode* tree, int** codes, int* code_lens) {
    for (int i = 0; i < 256; ++i) {
        int current = i;
        int len = 0;
        int path[256];

        while (tree[current].parent != -1) {
            int parent = tree[current].parent;
            path[len++] = (tree[parent].left == current) ? 0 : 1;
            current = parent;
        }

        codes[i] = (int*)malloc(len * sizeof(int));
        code_lens[i] = len;
        for (int j = 0; j < len; ++j) {
            codes[i][j] = path[len - j - 1];
        }
    }
}

// 压缩文件
void compress(const char* input, const char* output) {
   
    FILE* fin = fopen(input, "rb");
    if (!fin) {
        fprintf(stderr, "无法打开输入文件\n");
        return;
    }

    FileHeader header;
    memset(&header, 0, sizeof(header));
    strncpy(header.magic, "HUF", 3);

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        header.weight[ch & 0xFF]++;
    }
    fseek(fin, 0, SEEK_END);
    header.original_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    // 构建哈夫曼树
    HuffmanNode tree[511];
    build_huffman_tree(tree, header.weight, 256);

    // 生成编码表
    int* codes[256];
    int code_lens[256];
    generate_codes(tree, codes, code_lens);

    // 写入文件头
    FILE* fout = fopen(output, "wb");
    fwrite(&header, sizeof(FileHeader), 1, fout);

    // 写入编码表
    for (int i = 0; i < 256; ++i) {
        fwrite(&code_lens[i], sizeof(int), 1, fout);
        if (code_lens[i] > 0) {
            fwrite(codes[i], sizeof(int), code_lens[i], fout);
        }
    }

    // 压缩数据
    unsigned char buffer = 0;
    int bit_count = 0;
    while ((ch = fgetc(fin)) != EOF) {
        int* code = codes[ch];
        for (int i = 0; i < code_lens[ch]; ++i) {
            buffer = (buffer << 1) | code[i];
            if (++bit_count == 8) {
                fputc(buffer, fout);
                buffer = 0;
                bit_count = 0;
            }
        }
    }

    // 处理剩余位
    if (bit_count > 0) {
        buffer <<= (8 - bit_count);
        fputc(buffer, fout);
    }

    // 清理资源
    fclose(fin);
    fclose(fout);
    for (int i = 0; i < 256; ++i) {
        if (codes[i]) free(codes[i]);
    }
}

// 解压文件
void decompress(const char* input, const char* output) {
    FILE* fin = fopen(input, "rb");
    if (!fin) {
        fprintf(stderr, "无法打开输入文件\n");
        return;
    }

    
    FileHeader header;
    fread(&header, sizeof(FileHeader), 1, fin);
    if (strncmp(header.magic, "HUF", 3) != 0) {
        fprintf(stderr, "无效文件格式\n");
        fclose(fin);
        return;
    }

   
    int code_lens[256];
    int* codes[256] = { 0 };
    for (int i = 0; i < 256; ++i) {
        fread(&code_lens[i], sizeof(int), 1, fin);
        if (code_lens[i] > 0) {
            codes[i] = (int*)malloc(code_lens[i] * sizeof(int));
            fread(codes[i], sizeof(int), code_lens[i], fin);
        }
    }

    HuffmanNode tree[511];
    build_huffman_tree(tree, header.weight, 256);

    FILE* fout = fopen(output, "wb");
    int root = 510; 
    int node = root;
    int remaining = header.original_size;

    unsigned char byte;
    while (remaining > 0 && fread(&byte, 1, 1, fin) == 1) {
        for (int mask = 0x80; mask > 0 && remaining > 0; mask >>= 1) {
            node = (byte & mask) ? tree[node].right : tree[node].left;

            if (tree[node].left == -1) {
                fputc(node, fout);
                node = root;
                --remaining;
            }
        }
    }
    fclose(fin);
    fclose(fout);
    for (int i = 0; i < 256; ++i) {
        if (codes[i]) free(codes[i]);
    }
}

int main() {
    int choice;
    cout << "1. 压缩文件\n2. 解压文件\n选择操作: ";
    cin >> choice;

    char input[256], output[256];

    if (choice == 1) {
        cout << "输入要压缩的文件: ";
        cin >> input;
        compress(input, strcat(input, ".huf"));
    }
    else if (choice == 2) {
        cout << "输入要解压的文件: ";
        cin >> input;
        *output = *input;
        output[strlen(output) - 4] = '\0';
        decompress(input,output);
    }
    else {
        cerr << "无效选择!\n";
    }
    return 0;
}