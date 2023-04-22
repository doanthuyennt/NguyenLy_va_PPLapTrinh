#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <string>
#include <stdio.h>

using namespace std;

class State{
    public:
        int farmer, fox, goose, beans;
        State(int fa, int fx, int go, int be){
            farmer = fa; fox = fx; goose = go; beans = be;
        }

        
        bool operator ==(const State &rhs) {
            return (
                this->farmer == rhs.farmer && this->fox == rhs.fox 
                    && this->goose == rhs.goose && this->beans == rhs.beans
            );
        }
        int heuristic(State* other){
            int h = abs(farmer - other->farmer) + abs(fox - other->fox) + abs(goose - other->goose) + abs(beans - other->beans);
            return h;
        }
        bool checkValidState() {
            if (this->fox == this->goose && this->fox != farmer) return false;
            if (this->goose == this->beans && this->goose != farmer) return false;
            return true;
        }
        friend ostream& operator<<(ostream &os, const State &node);
        // Đã lỡ nạp chồng rồi thì em làm cho hết luôn =))
        ostream& operator<<(ostream &os) {
            return (cout << *this);
        }
};

ostream& operator<<(ostream &os, const State & state) {
    string out_farmer = state.farmer == 1 ? "left" : "right";
    string out_fox = state.fox == 1 ? "left" : "right";
    string out_goose = state.goose == 1 ? "left" : "right";
    string out_beans = state.beans == 1 ? "left" : "right";
    printf("Farmer: %s; Goose: %s; Fox: %s; Beans: %s",out_farmer.c_str(), out_goose.c_str(), out_fox.c_str(), out_beans.c_str());
    return os; // :)) quên hoài
}

class Node{
    public:
        State* state;
        Node* parent;
        int f, g, h;
        Node(State* s, Node* p, int depth){
            state = s; parent = p;
            g = depth;
            h = state->heuristic(new State(2, 2, 2, 2));
            f = g + h;
        }


        vector<Node*> expand(){
            vector<Node*> children;
            vector<State*> next_states = getNextStates();
            for(int i = 0; i < next_states.size(); i++){
                State* next_state = next_states[i];
                if (!next_state->checkValidState()) continue;
                children.push_back(new Node(next_state, this, this->g));
            }
            return children;
        }
        
        // DFS: depth first search
        vector<State*> getNextStates() {
            vector<State*> next_states;

            // calculate the opposite bank
            int opposite_farmer = 3 - state->farmer;
            int opposite_fox = 3 - state->fox;
            int opposite_goose = 3 - state->goose;
            int opposite_beans = 3 - state->beans;

            // farmer only
            next_states.push_back(new State(opposite_farmer, state->fox, state->goose, state->beans));

            // farmer and fox
            if (state->farmer == state->fox) {
                next_states.push_back(new State(opposite_farmer, opposite_fox, state->goose, state->beans));
            }

            // farmer and goose
            if (state->farmer == state->goose) {
                next_states.push_back(new State(opposite_farmer, state->fox, opposite_goose, state->beans));
            }

            // farmer and beans
            if (state->farmer == state->beans) {
                next_states.push_back(new State(opposite_farmer, state->fox, state->goose, opposite_beans));
            }

            return next_states;
        }
};

class Graph {

};

auto compare = [](Node* node1, Node* node2) {
    return node1->f > node2->f;
};
auto hashState = [](State* state) {
    int h = 0;
    h += state->farmer * 10;
    h += state->fox * 100;
    h += state->goose * 1000;
    h += state->beans * 10000;
    return h;
};


auto hashPath = [](vector<State*> path) {
    int h = 0;
    for (const auto & state : path) {
        h += hashState(state);
    }
};
vector<State*> astar_search(State* start, State* goal){

    Node* start_node = new Node(start, NULL, 0);

    priority_queue<Node*, vector<Node*>, decltype(compare) > frontier(compare);
    frontier.push(start_node);
    unordered_set<State*, decltype(hashState) > explored(10, hashState);
    explored.insert(start); 
    

    
    while(!frontier.empty()) { 
        Node* node = frontier.top(); 
        frontier.pop();
        State* state = node->state;
        if (*state == *goal) { 
            vector<State*> path;
            while(node != NULL) { 
                path.push_back(node->state); 
                node = node->parent; 
            } 
            reverse(path.begin(), path.end()); 
            return path; 
        } 
        vector<Node*> children = node->expand(); 
        for(int i = 0; i < children.size(); i++) { 
            State* state = children[i]->state;
            
            if(explored.count(state) > 0) continue; 
            explored.insert(state); 
            frontier.push(children[i]); 
        }
    }
    return vector<State*>();
}


int main() {
    State* start = new State(1, 1, 1, 1);
    State* goal = new State(2, 2, 2, 2);
    vector<State*> path = astar_search(start, goal);
    for(int i = 0; i < path.size(); i++){
        cout << "=====================\n";
        State* state = path[i];
        cout << *state << ":  hash state: " << hashState(state) << "\n";
    }
    return 0;
}