#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Node
{
public:
    int value;
    Node *left;
    Node *right;

    Node(int val)
    {
        value = val;
        left = NULL;
        right = NULL;
    }
};

// Generate tree from array input
Node *generateTree(vector<int> &values)
{
    int length = values.size();

    if (length == 0)
        return NULL;

    vector<Node *> nodes(length);

    for (int i = 0; i < length; i++)
    {
        if (values[i] != -1)
            nodes[i] = new Node(values[i]);
        else
            nodes[i] = NULL;
    }

    int parent = 0;
    int child = 1;

    while (child < length)
    {
        if (nodes[parent] != NULL)
        {
            nodes[parent]->left = nodes[child++];

            if (child < length)
                nodes[parent]->right = nodes[child++];
        }

        parent++;
    }

    return nodes[0];
}

// Parallel BFS
void parallelBFS(Node *root)
{
    if (root == NULL)
        return;

    vector<Node *> currentLevel;
    currentLevel.push_back(root);

    while (!currentLevel.empty())
    {
        vector<Node *> nextLevel;

#pragma omp parallel for
        for (int i = 0; i < currentLevel.size(); i++)
        {
            Node *node = currentLevel[i];

#pragma omp critical
            cout << node->value << " -> ";

            if (node->left != NULL)
            {
#pragma omp critical
                nextLevel.push_back(node->left);
            }

            if (node->right != NULL)
            {
#pragma omp critical
                nextLevel.push_back(node->right);
            }
        }

        currentLevel = nextLevel;
    }
}

// Parallel DFS
void parallelDFS(Node *root)
{
    if (root == NULL)
        return;

#pragma omp critical
    cout << root->value << " -> ";

#pragma omp parallel sections
    {
#pragma omp section
        parallelDFS(root->left);

#pragma omp section
        parallelDFS(root->right);
    }
}

int main()
{
    int n;

    cout << "Enter number of nodes: ";
    cin >> n;

    vector<int> values(n);

    cout << "Enter tree values level-wise (-1 for NULL):\n";

    for (int i = 0; i < n; i++)
    {
        cin >> values[i];
    }

    Node *root = generateTree(values);

    // Parallel BFS
    auto start1 = high_resolution_clock::now();

    cout << "\nParallel BFS: ";
    parallelBFS(root);

    auto stop1 = high_resolution_clock::now();

    auto duration1 =
        duration_cast<microseconds>(stop1 - start1);

    cout << "\nExecution Time: "
         << duration1.count()
         << " microseconds\n";

    // Parallel DFS
    auto start2 = high_resolution_clock::now();

    cout << "\nParallel DFS: ";
    parallelDFS(root);

    auto stop2 = high_resolution_clock::now();

    auto duration2 =
        duration_cast<microseconds>(stop2 - start2);

    cout << "\nExecution Time: "
         << duration2.count()
         << " microseconds\n";

    return 0;
}