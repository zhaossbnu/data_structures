//
// Created by zhaossbnu on 2017/12/14.
//

#ifndef GRAPH_LINKGRAPH_H
#define GRAPH_LINKGRAPH_H

#include <vector>
#include <forward_list>
#include <algorithm>
#include <iostream>

template <typename T>
class LinkGraph
{
public:
    LinkGraph(const std::vector<T> &vertices, const std::vector<std::vector<bool>> &edges, bool directed = false)
    {
        this->vertices = vertices;
        this->edges.resize(vertices.size());
        for(int i = 0; i < vertices.size(); ++i)
        {
            for(int j = 0; j < vertices.size(); ++j)
            {
                if(edges[i][j])
                {
                    this->edges[i].push_front(j);
                }
            }
        }
        this->directed = directed;
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
        return std::find(edges[source].cbegin(), edges[source].cend(), dest) != edges[source].cend();
    }

    void insert_edge(int source, int dest)
    {
        if(source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if(!is_directed())
        {
            edges[source].push_front(dest);
            edges[dest].push_front(source);
        }
        else
        {
            edges[source].push_front(dest);
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
            std::remove(edges[source].begin(), edges[source].end(), dest);
            std::remove(edges[dest].begin(), edges[dest].end(), source);
        }
        else
        {
            std::remove(edges[source].begin(), edges[source].end(), dest);
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
            std::for_each(edges[vertice].begin(), edges[vertice].end(), [&degree](){++degree;});
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
            for(auto list: edges)
            {
                for(auto it = list.cbegin(); it != list.cend(); ++it)
                {
                    if(*it == vertice)
                    {
                        ++degree;
                    }
                }
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
            std::for_each(edges[vertice].begin(), edges[vertice].end(), [&degree](){++degree;});
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
    std::vector<std::forward_list<int>> edges;
    bool directed = false;

    void dfs(int vertice, std::vector<bool> &reach);
    bool find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach);
};

template <typename T>
void LinkGraph<T>::bfs(int vertice)
{
    std::vector<bool> reach(vertices.size(), false);
    std::queue<int> q;
    q.push(vertice);
    reach[vertice] = true;
    while(!q.empty())
    {
        int vertice = q.front();
        std::cout << vertices[vertice] << std::endl;
        q.pop();
        for(auto it = edges[vertice].cbegin(); it != edges[vertice].cend(); ++it)
        {
            if (!reach[*it])
            {
                q.push(*it);
                reach[*it] = true;
            }
        }

    }
}

template <typename T>
void LinkGraph<T>::dfs(int vertice, std::vector<bool> &reach)
{
    reach[vertice] = true;
    std::cout << vertices[vertice] << std::endl;
    for(auto it = edges[vertice].cbegin(); it != edges[vertice].cend(); ++it)
    {
        if(!reach[*it])
        {
            dfs(*it, reach);
        }
    }
}

template <typename T>
void LinkGraph<T>::dfs(int vertice)
{
    std::vector<bool> reach(vertices.size(), false);
    dfs(vertice, reach);
}

template <typename T>
bool LinkGraph<T>::find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach)
{
    reach[source] = true;
    for(auto it = edges[source].cbegin(); it != edges[source].cend(); ++it)
    {
        if(!reach[*it])
        {
            path.push_back(*it);
            if(*it == dest || find_path(*it, dest, path, reach))
            {
                return true;
            }
            path.pop_back();
        }
    }
    return false;
}

template <typename T>
std::vector<int> LinkGraph<T>::find_path(int source, int dest)
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
bool LinkGraph<T>::connected()
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

#endif //GRAPH_LINKGRAPH_H
