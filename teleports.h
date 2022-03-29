#pragma once
#include "nonorgraph.h"

struct SOLUTION{
    std::vector<double> teleport_position;
    NonOrGraph list_of_edges_between_teleports;
    std::vector<size_t> walking_order;
    enum POSITION{
        BEGIN = 0,
        END = 1
    };
};

class teleports{
    NonOrGraph portals;
    std::vector<size_t> walking;
    std::vector<double> vertices_to_numbers;
    size_t available_teleports = 0u;
    size_t score = 0u;
    bool solved = false;

    enum POSITION{
        BEGIN = 0,
        END = 1,
        SIZE
    };
    
public:
    struct COMPONENTS{
        size_t starting_vertex = POSITION::BEGIN;
        size_t vertices_in_component = 0;
    };
private:

    size_t find_vertex_before_end();
    COMPONENTS find_largest_component_in_graph();
    bool is_valid_edge(size_t, size_t);
    size_t add_vertex(double);
    bool add_edge(double, double);
    size_t calculate_score();
    double get_position_between(size_t,size_t);
public:

    teleports();
    bool add_edgefo(size_t,size_t);
    size_t set_available_teleports(size_t);
    size_t find_solution();
    size_t get_score();
    void clear();
    SOLUTION get_solution();
};

size_t teleports::find_vertex_before_end(){
    for(size_t i = POSITION::END + 1; i < walking.size(); i++){
        if(walking[i] == POSITION::END) return i;
    }
    return POSITION::BEGIN;
}
size_t teleports::calculate_score(){
    score = 0;
    size_t current_pos = POSITION::BEGIN;
    while(current_pos != POSITION::END){
        current_pos = walking[current_pos];
        if(current_pos != POSITION::END && current_pos != POSITION::BEGIN){
            score++;
            current_pos = portals.get_adjacent_vertices_to(current_pos).front();
        }
    }
    return score;
}


double teleports::get_position_between(size_t from, size_t to ){
    double pos_of_from = 0.0;
    bool not_empty = false;
    if(from != POSITION::BEGIN && from != POSITION::END){ 
        not_empty = true;
        pos_of_from = vertices_to_numbers[from]; 
    }
    
    double pos_of_to = pos_of_from;
    if(to != POSITION::BEGIN && to != POSITION::END){
        not_empty = true;
        pos_of_to = vertices_to_numbers[to];
    }

    if(not_empty == true)
    return (pos_of_from + pos_of_to) / 2.0;
    return 1.0;
}

size_t teleports::find_solution(){
    solved = true;

    while(available_teleports != 0u){
        available_teleports--;
        COMPONENTS largest_component = find_largest_component_in_graph();
        
        size_t add_after = largest_component.starting_vertex;
        size_t add_before = walking[add_after];
        if(available_teleports > 0)
            if(add_after == POSITION::BEGIN){
                add_after = find_vertex_before_end();
                add_before = POSITION::END;
            }
        double position = get_position_between(add_after, add_before);
        size_t to = add_vertex(position);
        
        add_after = POSITION::BEGIN;
        add_before = walking[add_after];
        position = get_position_between(add_after, add_before);
        size_t from = add_vertex(position);

        portals.add_edge_between_vertices(from,to);
    }

    score = calculate_score();
    return score;
}

teleports::COMPONENTS teleports::find_largest_component_in_graph(){
    COMPONENTS largest_component;
    for(size_t i = POSITION::END + 1; i < walking.size(); i++){
        size_t current_pos = i;
        bool repeat_walk = false;
        size_t counter = 0u;
        while(current_pos != POSITION::END && repeat_walk != true){
            current_pos = walking[current_pos];
            
            if(portals.get_adjacent_vertices_to(current_pos).empty() == true && (current_pos != POSITION::BEGIN && current_pos != POSITION::END)) throw "Exception";
            if(current_pos != POSITION::END){
                current_pos = portals.get_adjacent_vertices_to(current_pos).front();
                if(current_pos == i) 
                    repeat_walk = true;
                else counter++;
            }
        }
        if(counter > largest_component.vertices_in_component && repeat_walk == true){
            largest_component.starting_vertex = i;
            largest_component.vertices_in_component = counter;
        }
    }
    return largest_component;
}

bool teleports::is_valid_edge(size_t from, size_t to){
    return true;
}

size_t teleports::add_vertex(double pos){
    size_t add_after = POSITION::BEGIN;
    double previous_closest = 0.0;
    for(int i =POSITION::END + 1; i<vertices_to_numbers.size(); i++){
        if(vertices_to_numbers[i] < pos) 
            if(previous_closest < vertices_to_numbers[i]){
                add_after = i;
                previous_closest = vertices_to_numbers[i];
            }
    }

    size_t add_before = walking[add_after];
    size_t size = walking.size();
    walking.resize(size + 1);
    walking[add_after] = size;
    walking[size] = add_before;

    size_t vertex = portals.add_vertex();
    vertices_to_numbers.resize(vertices_to_numbers.size() + 1);
    vertices_to_numbers[vertex] = pos;

    return vertex;
}

bool teleports::add_edge(double from, double to){
    size_t vertex1 = add_vertex(from);
    size_t vertex2 = add_vertex(to);
    portals.add_edge_between_vertices(vertex1, vertex2);
    return true;
}

bool teleports::add_edgefo(size_t from, size_t to){
    if(is_valid_edge(from, to) == true)
        return add_edge((double) from, (double) to);
    else return false;
}

size_t teleports::set_available_teleports(size_t new_value){
    size_t previous_value = available_teleports;
    available_teleports = new_value;
    return previous_value;
}

size_t teleports::get_score(){
    return score;
}

SOLUTION teleports::get_solution(){
    SOLUTION value;
    value.teleport_position = vertices_to_numbers;
    value.list_of_edges_between_teleports = portals;
    value.walking_order = walking;

    return value;
}

void teleports::clear(){
    *this = teleports();
}

teleports::teleports(){
    portals.add_vertex();
    portals.add_vertex();
    walking.resize(POSITION::SIZE);
    walking[BEGIN] = END;
    vertices_to_numbers.resize(POSITION::SIZE);    

};
