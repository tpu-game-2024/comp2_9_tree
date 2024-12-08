#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"

#define MAX_DATA 256


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
	int n = (int)strlen(value);
	memcpy(p->value, value, strlen(value)+1);

	p->left = p->right = NULL;

	return p;
}

// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value)
{
	if (t == NULL) return false;

	node* p = generate(key, value);

	if (p == NULL) return false;// メモリ確保できなかった。

	if (t->root == NULL) {
		t->root = p;
		return true;
	}

	// Todo: t->rootの下にkeyの値の大小でleftかrightを切り替えながらpを追加する処理を実装する

	struct node_* p2 = t->root;

	while (1) {
		if (key < p2->key) {
			if (p2->left == NULL) {
				p2->left = p;
				break;
			}
			p2 = p2->left;
		}
		else if (key > p2->key) {
			if (p2->right == NULL) {
				p2->right = p;
				break;
			}
			p2 = p2->right;
		}
		else if (key == p2->key) {
			for (int i = 0; i < MAX_DATA; i++) {
				p2->value[i] = p->value[i];
			}
			free(p);
			break;
		}
	}

	return true;
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
	struct node_* p;
	p = t->root;

	while (p) {
		if (key < p->key) {
			p = p->left;
		}
		else if (key > p->key) {
			p = p->right;
		}
		else {
			return p->value;
		}
	}
	return NULL;
}

void inorder(struct node_* n, void(*func)(const node* p)) 
{
	if (n != NULL) {
		inorder(n->left, func);
		func(n);
		inorder(n->right, func);
	}
	return;
}

// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree* t, void (*func)(const node* p))
{
	struct node_* p = t->root;
	inorder(p, func);
	return;
}