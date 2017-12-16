//
// Created by zhaossbnu on 2017/12/13.
//

#ifndef GRAPH_MATRIXGRAPH_H
#define GRAPH_MATRIXGRAPH_H

#include <vector>
#include <iostream>
#include <iostream>
#include <queue>

template <typename T>
class MatrixGraph
{
public:
    MatrixGraph(const std::vector<T> &vertices, const std::vector<std::vector<bool>> &edges, bool directed = false)
    {
        this->vertices = vertices;
        this->edges = edges;
        this->directed;
    }

    int number_of_vertices() const
    {
        return vertices.size();
    }

    bool exist_edge(int source, int dest) const
    {
        if(source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size())
        {
            return false;
        }
        return  edges[source][dest];
    }

    void insert_edge(int source, int dest)
    {
        if(source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if(!is_directed())
        {
            edges[source][dest] = edges[dest][source] = true;
        }
        else
        {
            edges[source][dest] = true;
        }
    }

    void earse_edge(int source, int dest)
    {
        if(source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if(!is_directed())
        {
            edges[source][dest] = edges[dest][source] = false;
        }
        else
        {
            edges[source][dest] = false;
        }
    }

    int degree(int vertice) const
    {
        if(vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
        }
        int degree = 0;
        if(!is_directed())
        {
            for(int j = 0; j < vertices.size(); ++j)
            {
                degree += edges[vertices][j];
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
        if(vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
            return -1;
        }
        if(!is_directed)
        {
            std::cerr << "此图为无向图！！！" << std::endl;
            return -1;
        }
        else
        {
            int degree = 0;
            for(int i = 0; i < vertices.size(); ++i)
            {
                degree += edges[i][vertice];
            }
            return degree;
        }
    }

    int out_degree(int vertice) const
    {
        if(vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
            return -1;
        }
        if(!is_directed)
        {
            std::cerr << "此图为无向图！！！" << std::endl;
            return -1;
        }
        else
        {
            int degree = 0;
            for(int j = 0; j < vertices.size(); ++j)
            {
                degree += edges[vertice][j];
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

private:
    std::vector<T> vertices;
    std::vector<std::vector<bool>> edges;
    bool directed = false;
    void dfs(int vertice, std::vector<bool> &reach);
    bool find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach);
};

template <typename T>
void MatrixGraph<T>::bfs(int vertice)
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
        for(int j = 0; j < vertices.size(); ++j)
        {
            if(edges[vertice][j] && !reach[j])
            {
                q.push(j);
                reach[j] = true;
            }
        }
    }
}

template <typename T>
void MatrixGraph<T>::dfs(int vertice)
{
    std::vector<bool> reach(vertices.size(), false);
    dfs(vertice, reach);
}

template <typename T>
void MatrixGraph<T>::dfs(int vertice, std::vector<bool> &reach)
{
    reach[vertice] = true;
    std::cout << vertices[vertice] << std::endl;
    for(int j = 0; j < vertices.size(); ++j)
    {
        if(edges[vertice][j] && !reach[j])
        {
            dfs(j, reach);
        }
    }
}

template <typename T>
std::vector<int> MatrixGraph<T>::find_path(int source, int dest)
{
    std::vector<int> path;
    std::vector<bool> reach(vertices.size(), false);
    path.push_back(source);
    if(source == dest || find_path(source, dest, path, reach))
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
bool MatrixGraph<T>::find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach)
{
    reach[source] = true;
    for(int j = 0; j < vertices.size(); ++j)
    {
        if(edges[source][j] && !reach[j])
        {
            path.push_back(j);
            if(j == dest || find_path(j, dest, path, reach))
            {
                return true;
            }
            path.pop_back();
        }
    }
    return false;
}

template <typename T>
bool MatrixGraph<T>::connected()
{
    if(directed)
    {
        std::cerr << "此图为有向图，没有连通这个概念！" << std::endl;
        return false;
    }
    std::vector<bool> reach(vertices.size(), false);
    dfs(0, reach);
    return std::find(reach.cbegin(), reach.cend(), false) == reach.cend();
}

#endif //GRAPH_MATRIXGRAPH_H
