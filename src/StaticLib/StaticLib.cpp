#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

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
	if (p == NULL) return false;

	if (t->root == NULL) {
		t->root = p;
		return true;
	}
	// Todo: t->rootの下にkeyの値の大小でleftかrightを切り替えながらpを追加する処理を実装する
	node* current = t->root;
	while (true) {
		if (key < current->key) {
			if (current->left == NULL) {
				current->left = p;
				return true;
			}
			current = current->left;
		}
		else {
			if (current->right == NULL) {
				current->right = p;
				return true;
			}
			current = current->right;
		}
	}
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree * t, int key)
{
	// ToDo: 実装する
	if (t == NULL) return NULL;

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
	return NULL;
}


// keyの小さな順にコールバック関数funcを呼び出す
void search(const tree * t, void (*func)(const node * p))
{
	// ToDo: 実装する
	if (t == NULL || func == NULL) return;

	node* stack[100]; 
	int top = -1;
	node* current = t->root;

	while (current != NULL || top != -1) {
		while (current != NULL) {
			stack[++top] = current;
			current = current->left;
		}

		current = stack[top--];
		func(current);
		current = current->right;
	}
}
