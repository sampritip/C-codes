#include<iostream>
#include<queue>
using namespace std;
class node{
    public:
    int value;
    node* left;
    node* right;
    node(int num)
    {
        value = num;
        left = nullptr;
        right = nullptr;
    }
};
void levelOrderTraversal(node* root)
{
    queue<node*> Q;
    Q.push(root);
    while(!Q.empty())
    {
        node* temp = Q.front(); Q.pop();
        cout<<temp->value;
        if(temp->left)
           Q.push(temp->left);
        if(temp->right)
           Q.push(temp->right);

    }

}
void preOrder(node* root)
{
    if(root == NULL)
       return;
    cout<<root->value;
    preOrder(root->left);
    preOrder(root->right);
}
int main()
{
     node* root = new node(1);
     root->left = new node(2);
     root->right = new node(3);
     root->left->left = new node(4);
     root->left->right = new node(5);
     root->right->left = new node(6);
     root->right->right = new node(7);
     //preOrder(root);
     levelOrderTraversal(root);


}