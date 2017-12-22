//
// Created by zhaossbnu on 2017/12/19.
//

#ifndef GRAPH_WEIGHTLINKGRAPH_H
#define GRAPH_WEIGHTLINKGRAPH_H

#include <vector>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <queue>
#include <cfloat>
#include <utility>

template <typename T>
class WeightLinkGraph
{
public:
    WeightLinkGraph(const std::vector<T> &vertices, const std::vector<std::vector<double>> &edges, bool directed = false)
    {
        this->vertices = vertices;
        this->edges.resize(vertices.size());
        for(int i = 0; i < vertices.size(); ++i)
        {
            for(int j = 0; j < vertices.size(); ++j)
            {
                if(edges[i][j] != DBL_MAX)
                {
                    this->edges[i].push_front({j, edges[i][j]});
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
        bool found= false;
        for(auto it = edges[source].cbegin(); it != edges[source].cend(); ++it)
        {
            if(it->first == dest)
            {
                found = true;
            }
        }
        return found;
    }

    void insert_edge(int source, int dest, double weight)
    {
        if(source < 0 || dest < 0 || source >= vertices.size() || dest > vertices.size() || source == dest)
        {
            return;
        }
        if(!is_directed())
        {
            edges[source].push_front({dest, weight});
            edges[dest].push_front({source, weight});
        }
        else
        {
            edges[source].push_front({dest, weight});
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

            std::remove(edges[source].begin(), edges[source].end(), [&dest](std::forward_list<std::pair<int, double>>::iterator &r)->bool { return r->first == dest;});
            std::remove(edges[dest].begin(), edges[dest].end(), [&source](std::forward_list<std::pair<int, double>>::iterator &r)->bool { return r->first == source;});
        }
        else
        {
            std::remove(edges[source].begin(), edges[source].end(), [&dest](std::forward_list<std::pair<int, double>>::iterator &r)->bool { return r->first == dest;});
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
                    if(it->first == vertice)
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
    std::vector<int> top_logical_sort();

private:
    std::vector<T> vertices;
    std::vector<std::forward_list<std::pair<int, double>>> edges;
    bool directed = false;

    void dfs(int vertice, std::vector<bool> &reach);
    bool find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach);
};



template <typename T>
void WeightLinkGraph<T>::bfs(int vertice)
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
            if (!reach[it->first])
            {
                q.push(it->first);
                reach[it->first] = true;
            }
        }

    }
}

template <typename T>
void WeightLinkGraph<T>::dfs(int vertice, std::vector<bool> &reach)
{
    reach[vertice] = true;
    std::cout << vertices[vertice] << std::endl;
    for(auto it = edges[vertice].cbegin(); it != edges[vertice].cend(); ++it)
    {
        if(!reach[it->first])
        {
            dfs(it->first, reach);
        }
    }
}

template <typename T>
void WeightLinkGraph<T>::dfs(int vertice)
{
    std::vector<bool> reach(vertices.size(), false);
    dfs(vertice, reach);
}

template <typename T>
bool WeightLinkGraph<T>::find_path(int source, int dest, std::vector<int> &path, std::vector<bool> &reach)
{
    reach[source] = true;
    for(auto it = edges[source].cbegin(); it != edges[source].cend(); ++it)
    {
        if(!reach[it->first])
        {
            path.push_back(it->first);
            if(it->first == dest || find_path(it->first, dest, path, reach))
            {
                return true;
            }
            path.pop_back();
        }
    }
    return false;
}

template <typename T>
std::vector<int> WeightLinkGraph<T>::find_path(int source, int dest)
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
bool WeightLinkGraph<T>::connected()
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
std::vector<int> WeightLinkGraph<T>::top_logical_sort()
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
            if(exist_edge(p, j))
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

#endif //GRAPH_WEIGHTLINKGRAPH_H
