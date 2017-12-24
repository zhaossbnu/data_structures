//
// Created by zhaossbnu on 2017/12/19.
//

#ifndef GRAPH_WEIGHTMATRIXGRAPH_H
#define GRAPH_WEIGHTMATRIXGRAPH_H

#include <vector>
#include <iostream>
#include <iostream>
#include <queue>
#include <cfloat>
#include "UnionFind.h"

struct Edge
{
    int source, dest;
    double weight;
    Edge()
    {

    }
    Edge(int source, int dest, double weight)
    {
        this->source = source;
        this->dest = dest;
        this->weight = weight;
    }
    bool operator<(const Edge &a)
    {
        return weight < a.weight;
    }
    friend bool operator>(const Edge &a, const Edge &b)
    {
        return a.weight > b.weight;
    }
};

template <typename T>
class WeightMatrixGraph
{
public:
    WeightMatrixGraph(const std::vector<T> &vertices, const std::vector<std::vector<double>> &edges, bool directed = false)
    {
        this->vertices = vertices;
        this->edges = edges;
        this->directed = directed;
    }

    int number_of_vertices() const
    {
        return vertices.size();
    }

    bool exist_edge(int source, int dest) const
    {
        if (source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size())
        {
            return false;
        }
        return edges[source][dest] < DBL_MAX;
    }

    void insert_edge(int source, int dest, double weight)
    {
        if (source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if (!is_directed())
        {
            edges[source][dest] = edges[dest][source] = weight;
        }
        else
        {
            edges[source][dest] = weight;
        }
    }

    void earse_edge(int source, int dest)
    {
        if (source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if (!is_directed())
        {
            edges[source][dest] = edges[dest][source] = DBL_MAX;
        }
        else
        {
            edges[source][dest] = DBL_MAX;
        }
    }

    int degree(int vertice) const
    {
        if (vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
        }
        int degree = 0;
        if (!is_directed())
        {
            for (int j = 0; j < vertices.size(); ++j)
            {
                if (edges[vertice][j] != DBL_MAX)
                {
                    ++degree;
                }
            }
            return degree;
        }
        else
        {
            std::cerr << "此图为有向图，没有度！！！" << std::endl;
            return  -1;
        }
    }

    int in_degree(int vertice) const
    {
        if (vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
            return -1;
        }
        if (!is_directed())
        {
            std::cerr << "此图为无向图！！！" << std::endl;
            return -1;
        }
        else
        {
            int degree = 0;
            for (int i = 0; i < vertices.size(); ++i)
            {
                if (edges[i][vertice] != DBL_MAX)
                {
                    ++degree;
                }
            }
            return degree;
        }
    }

    int out_degree(int vertice) const
    {
        if (vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
            return -1;
        }
        if (!is_directed())
        {
            std::cerr << "此图为无向图！！！" << std::endl;
            return -1;
        }
        else
        {
            int degree = 0;
            for (int j = 0; j < vertices.size(); ++j)
            {
                if (edges[vertices][j] != DBL_MAX)
                {
                    ++degree;
                }
            }
            return degree;
        }
    }

    bool is_directed() const
    {
        return directed;
    }

    void bfs(int vertice);
    void dfs(int vertice);

    std::vector<int> find_path(int source, int dest);
    bool connected();
    std::vector<int> top_logical_sort();
    std::pair<std::vector<int>, std::vector<double>> dijkstra(int source);
    std::vector<std::vector<double>> kruskal();
    std::vector<std::vector<double>> prim();

private:
    std::vector<T> vertices;
    std::vector<std::vector<double>> edges;
    bool directed = false;
    void dfs(int vertice, std::vector<bool> &reach);
    bool find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach);
};

template <typename T>
void WeightMatrixGraph<T>::bfs(int vertice)
{
    std::vector<bool> reach(vertices.size(), false);
    std::queue<int> q;
    reach[vertice] = true;
    q.push(vertice);
    while (!q.empty())
    {
        vertice = q.front();
        std::cout << vertices[vertice] << std::endl;
        q.pop();
        for (int j = 0; j < vertices.size(); ++j)
        {
            if (edges[vertice][j] != DBL_MAX && !reach[j])
            {
                q.push(j);
                reach[j] = true;
            }
        }
    }
}

template <typename T>
void WeightMatrixGraph<T>::dfs(int vertice)
{
    std::vector<bool> reach(vertices.size(), false);
    dfs(vertice, reach);
}

template <typename T>
void WeightMatrixGraph<T>::dfs(int vertice, std::vector<bool> &reach)
{
    reach[vertice] = true;
    std::cout << vertices[vertice] << std::endl;
    for (int j = 0; j < vertices.size(); ++j)
    {
        if (edges[vertice][j] != DBL_MAX && !reach[j])
        {
            dfs(j, reach);
        }
    }
}

template <typename T>
std::vector<int> WeightMatrixGraph<T>::find_path(int source, int dest)
{
    std::vector<int> path;
    std::vector<bool> reach(vertices.size(), false);
    path.push_back(source);
    if (source == dest || find_path(source, dest, path, reach))
    {
        return path;
    }
    else
    {
        path.clear();
        return path;
    }
}

template <typename T>
bool WeightMatrixGraph<T>::find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach)
{
    reach[source] = true;
    for (int j = 0; j < vertices.size(); ++j)
    {
        if (edges[source][j] != DBL_MAX && !reach[j])
        {
            path.push_back(j);
            if (j == dest || find_path(j, dest, path, reach))
            {
                return true;
            }
            path.pop_back();
        }
    }
    return false;
}

template <typename T>
bool WeightMatrixGraph<T>::connected()
{
    if (directed)
    {
        std::cerr << "此图为有向图，没有连通这个概念！" << std::endl;
        return false;
    }
    std::vector<bool> reach(vertices.size(), false);
    dfs(0, reach);
    return std::find(reach.cbegin(), reach.cend(), false) == reach.cend();
}

template <typename T>
std::vector<int> WeightMatrixGraph<T>::top_logical_sort()
{
    std::vector<int> result;
    if (is_directed())
    {
        std::cerr << "此图为无向图！！！" << std::endl;
        exit(-1);
    }
    std::vector<int> in_degree(vertices.size(), 0);
    std::queue<int> st;
    for (int i = 0; i < vertices.size(); ++i)
    {
        for (int j = 0; j < vertices.size(); ++j)
        {
            if (exist_edge(i, j))
            {
                ++in_degree[j];
            }
        }
    }
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (in_degree[i] == 0)
        {
            st.push(i);
        }
    }
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (st.empty())
        {
            std::cerr << "此图是有环图！！！" << std::endl;
            result.clear();
            exit(-1);
        }
        int p = st.front();
        st.pop();
        result.push_back(p);
        for (int j = 0; j < vertices.size(); ++j)
        {
            if (exist_edge(i, j))
            {
                --in_degree[j];
                if (in_degree[j] == 0)
                {
                    st.push(j);
                }
            }
        }
    }
    return result;
}

template <typename T>
std::pair<std::vector<int>, std::vector<double>>  WeightMatrixGraph<T>::dijkstra(int source)
{
    if (!is_directed())
    {
        std::cerr << "此图为无向图！！！" << std::endl;
        exit(-1);
    }
    std::vector<bool> reach(vertices.size(), false);
    std::vector<int> path(vertices.size(), -1);
    std::vector<double> cost(vertices.size(), DBL_MAX);
    cost[source] = 0;
    path[source] = -1;
    reach[source] = true;
    for (int i = 0; i < vertices.size(); ++i)
    {
        if (exist_edge(source, i))
        {
            cost[i] = edges[source][i];
            path[i] = source;
        }
    }
    for (int i = 0; i < vertices.size(); ++i)
    {
        double min_len = DBL_MAX;
        int current = source;
        for (int j = 0; j < vertices.size(); ++j)
        {
            if (reach[j] == false && cost[j] < min_len)
            {
                current = j;
                min_len = cost[j];
            }
        }
        reach[current] = true;
        for (int j = 0; j < vertices.size(); ++j)
        {
            if (reach[j] == false && exist_edge(current, j) && cost[current] + edges[current][j] < cost[j])
            {
                cost[j] = cost[current] + edges[current][j];
                path[j] = current;
            }
        }
    }
    return std::make_pair(path, cost);
}

template <typename T>
std::vector<std::vector<double>> WeightMatrixGraph<T>::kruskal()
{
    if(is_directed())
    {
        std::cerr << "此图为有向图！！！" << std::endl;
        exit(-1);
    }
    std::vector<std::vector<double>> tree;
    tree.resize(vertices.size());
    for (auto &i : tree)
    {
        i.resize(vertices.size());
    }
    for (int i = 0; i < vertices.size(); ++i)
    {
        for (int j = 0; j < vertices.size(); ++j)
        {
            tree[i][j] = DBL_MAX;
        }
    }
    UnionFind nodes(vertices.size());
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    for (int i = 0; i < vertices.size(); ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            if (edges[i][j] != DBL_MAX)
            {
                pq.push({ i, j, edges[i][j] });
            }
        }
    }
    int k = 0;
    while (!pq.empty() && k < vertices.size() - 1)
    {
        Edge e = pq.top();
        pq.pop();
        int a = nodes.find(e.source);
        int b = nodes.find(e.dest);
        if (a != b)
        {
            tree[e.source][e.dest] = edges[e.source][e.dest];
            tree[e.dest][e.source] = edges[e.dest][e.source];
            nodes.Union(a, b);
            ++k;
        }
    }
    if (k == vertices.size() - 1)
    {
        return tree;
    }
    else
    {
        std::cout << "没有最小生成树！" << std::endl;
        tree.clear();
        return tree;
    }
}

template <typename T>
std::vector<std::vector<double>> WeightMatrixGraph<T>::prim()
{
    if(is_directed())
    {
        std::cerr << "此图为有向图！！！" << std::endl;
        exit(-1);
    }
    std::vector<std::vector<double>> tree;
    tree.resize(vertices.size());
    for (auto &i : tree)
    {
        i.resize(vertices.size());
    }
    for (int i = 0; i < vertices.size(); ++i)
    {
        for (int j = 0; j < vertices.size(); ++j)
        {
            tree[i][j] = DBL_MAX;
        }
    }
    std::vector<int> reach;
    reach.push_back(0);
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    for(int j = 0; j < vertices.size(); ++j)
    {
        if(edges[0][j] != DBL_MAX)
        {
            pq.push({0, j, edges[0][j]});
        }
    }
    while(!pq.empty() && reach.size() < vertices.size() -1)
    {
        Edge e = pq.top();
        pq.pop();
        if (std::find(reach.cbegin(), reach.cend(), e.dest) == reach.cend())
        {
            tree[e.source][e.dest] = tree[e.dest][e.source] = edges[e.source][e.dest];
            reach.push_back(e.dest);
            for(int j = 0; j < vertices.size(); ++j)
            {
                if(edges[e.dest][j] != DBL_MAX)
                {
                    pq.push({e.dest, j, edges[e.dest][j]});
                }
            }
        }
    }
    if(reach.size() != vertices.size() - 1)
    {
        std::cout << "没有最小生成树！" << std::endl;
        tree.clear();
        return tree;
    }
    else
    {
        return tree;
    }

}


#endif //GRAPH_WEIGHTMATRIXGRAPH_H
