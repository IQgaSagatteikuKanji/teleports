#pragma once
#include <list>
#include <vector>
#include <algorithm>

class NonOrGraph{
    std::vector<std::list<size_t>> vertices;
public:
    bool contains_vertex(size_t);
    bool contains_edge(size_t,size_t);
    size_t add_vertex();
    bool add_edge_between_vertices(size_t,size_t);
    void delete_edge_between_vertices(size_t,size_t);
    std::list<size_t> get_adjacent_vertices_to(size_t);
};

std::list<size_t> NonOrGraph::get_adjacent_vertices_to(size_t vertex){
    std::list<size_t> return_value;
    if(contains_vertex(vertex) == true)
        return_value = vertices[vertex];
    return return_value;
}

void NonOrGraph::delete_edge_between_vertices(size_t first, size_t second){
    if(contains_edge(first, second) == true){
        vertices[first].remove(second);
        vertices[second].remove(first);
    }
}

bool NonOrGraph::contains_vertex(size_t vertex){
    return vertex < vertices.size();
}

bool NonOrGraph::contains_edge(size_t from, size_t to){
    bool contains = contains_vertex(from) & contains_vertex(to);
    if(contains == true){
        contains = (std::find(vertices[from].begin(), vertices[from].end(), to) != vertices[from].end());
    }
    return contains;
}

bool NonOrGraph::add_edge_between_vertices(size_t first, size_t second){
    bool added = false;
    if(contains_vertex(first) && contains_vertex(second)){
        if(contains_edge(first, second) == false){
            vertices[first].push_front(second);
            if(first != second)
                vertices[second].push_front(first);
        }
        added = true;
    }
    return added;
}

size_t NonOrGraph::add_vertex(){
    vertices.resize(vertices.size() + 1);
    return vertices.size()-1;
}

