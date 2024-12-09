#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>
#include <string.h>                     // 文字列操作に必要
#include "../include/lib_func.h"


// 2分木の初期化
void initialize(tree* t)
{
    if (t == NULL) return;

    t->root = NULL;
}

static void release_recursive(node* n)
{
    if (n == NULL) return;

    if (n->right) {
        release_recursive(n->right);
        n->right = NULL;
    }

    if (n->left) {
        release_recursive(n->left);
        n->left = NULL;
    }

    free(n);
}

// 使用メモリの全解放
void finalize(tree* t)
{
    if (t == NULL) return;

    release_recursive(t->root);
    t->root = NULL;
}


static node* generate(int key, const char* value)
{
    node* p = (node*)malloc(sizeof(node));

    if (p == NULL) return NULL;

    p->key = key;
    memcpy(p->value, value, strlen(value) + 1);

    p->left = p->right = NULL;

    return p;
}

// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value)
{
    if (t == NULL) return false;

    node* p = generate(key, value);
    if (p == NULL) return false; // メモリ確保できなかった

    if (t->root == NULL) {
        t->root = p;
        return true;
    }

    node* current = t->root;
    node* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (key == current->key) {
            // 既存のキーの値を上書きする
            memcpy(current->value, value, strlen(value) + 1);
            free(p); // 生成したが使わないノードを解放
            return true;
        }
        else if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    if (key < parent->key) {
        parent->left = p;
    }
    else {
        parent->right = p;
    }

    return true;
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
    if (t == NULL || t->root == NULL) return NULL;

    node* current = t->root;

    while (current != NULL) {
        if (key == current->key) {
            return current->value;
        }
        else if (key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return NULL; // 見つからなかった
}

// keyの小さな順にコールバック関数funcを呼び出す
static void in_order_recursive(const node* n, void (*func)(const node* p))
{
    if (n == NULL) return;

    in_order_recursive(n->left, func);
    func(n);
    in_order_recursive(n->right, func);
}

void search(const tree* t, void (*func)(const node* p))
{
    if (t == NULL || t->root == NULL) return;

    in_order_recursive(t->root, func);
}
