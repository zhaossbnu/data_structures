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

template <typename T>
class WeightMatrixGraph
{
public:
    WeightMatrixGraph(const std::vector<T> &vertices, const std::vector<std::vector<double>> &edges, bool directed = false)
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
        return edges[source][dest] < DBL_MAX;
    }

    void insert_edge(int source, int dest, double weight)
    {
        if(source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if(!is_directed())
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
        if(source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if(!is_directed())
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
        if(vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
        }
        int degree = 0;
        if(!is_directed())
        {
            for(int j = 0; j < vertices.size(); ++j)
            {
                if(edges[vertice][j] != DBL_MAX)
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
        if(vertice < 0 || vertice >= vertices.size())
        {
            std::cerr << "此点不在图中！！！" << std::endl;
            return -1;
        }
        if(!is_directed())
        {
            std::cerr << "此图为无向图！！！" << std::endl;
            return -1;
        }
        else
        {
            int degree = 0;
            for(int i = 0; i < vertices.size(); ++i)
            {
                if(edges[i][vertice] != DBL_MAX)
                {
                    ++degree;
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
        if(!is_directed())
        {
            std::cerr << "此图为无向图！！！" << std::endl;
            return -1;
        }
        else
        {
            int degree = 0;
            for(int j = 0; j < vertices.size(); ++j)
            {
                if(edges[vertices][j] != DBL_MAX)
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
        for(int j = 0; j < vertices.size(); ++j)
        {
            if(edges[vertice][j] != DBL_MAX && !reach[j])
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
    for(int j = 0; j < vertices.size(); ++j)
    {
        if(edges[vertice][j] != DBL_MAX && !reach[j])
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
bool WeightMatrixGraph<T>::find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach)
{
    reach[source] = true;
    for(int j = 0; j < vertices.size(); ++j)
    {
        if(edges[source][j] != DBL_MAX && !reach[j])
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
bool WeightMatrixGraph<T>::connected()
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

template <typename T>
std::vector<int> WeightMatrixGraph<T>::top_logical_sort()
{
    std::vector<int> result;
    if(is_directed())
    {
        std::cerr << "此图为无向图！！！" << std::endl;
        exit(-1);
    }
    std::vector<int> in_degree(vertices.size(), 0);
    std::queue<int> st;
    for(int i = 0; i < vertices.size(); ++i)
    {
        for(int j = 0; j < vertices.size(); ++j)
        {
            if(exist_edge(i, j))
            {
                ++in_degree[j];
            }
        }
    }
    for(int i = 0; i < vertices.size();  ++i)
    {
        if(in_degree[i] == 0)
        {
            st.push(i);
        }
    }
    for(int i = 0; i < vertices.size(); ++i)
    {
        if(st.empty())
        {
            std::cerr << "此图是有环图！！！" << std::endl;
            result.clear();
            exit(-1);
        }
        int p = st.front();
        st.pop();
        result.push_back(p);
        for(int j = 0; j < vertices.size(); ++j)
        {
            if(exist_edge(i, j))
            {
                --in_degree[j];
                if(in_degree[j] == 0)
                {
                    st.push(j);
                }
            }
        }
    }
    return result;
}

#endif //GRAPH_WEIGHTMATRIXGRAPH_H
