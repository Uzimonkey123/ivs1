//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Norman Babiak <xbabia01@stud.fit.vutbr.cz>
// $Date:       $2023-03-03
//============================================================================//
/**
 * @file tdd_code.cppL
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"


Graph::Graph(){

}

Graph::~Graph(){}

std::vector<Node*> Graph::nodes() {
    std::vector<Node*> nodes;
    for (int i = 0; i < nodesArr.size(); i++) {
        nodes.push_back(&nodesArr[i]);
    }
    return nodes;
}

std::vector<Edge> Graph::edges() const {
    return edgesArr;
}

Node* Graph::addNode(size_t nodeId) 
{
    for (Node n : nodesArr) {
        if (n.id == nodeId) {
            return nullptr;        
        }
    }

    Node newNode;
    newNode.id = nodeId;
    newNode.color = 0;
    
    nodesArr.push_back(newNode);
    return &nodesArr.back();
}

bool Graph::addEdge(const Edge& edge){
    if(!getNode(edge.a)) {
        addNode(edge.a);
    }
    if(!getNode(edge.b)) {
        addNode(edge.b);
    }

    if (edge.a == edge.b) {
        return false;
    }

    for (auto e : edgesArr) {
        if ((e.a == edge.a && e.b == edge.b) || (e.a == edge.b && e.b == edge.a)) {
            return false;
        }
    }
    
    edgesArr.push_back(edge);
    return true;
}


void Graph::addMultipleEdges(const std::vector<Edge>& edges) {

    for(int i = 0; i < edges.size(); i++) {
        addEdge(edges[i]);

    }
}

Node* Graph::getNode(size_t nodeId){
    for (auto& nodePtr : nodesArr) {
        if (nodePtr.id == nodeId) {
            return &nodePtr;
        }
    }
    return nullptr;
}


bool Graph::containsEdge(const Edge& edge) const{
    for (const Edge& existing_edge : edgesArr) {
        if ((existing_edge.a == edge.a && existing_edge.b == edge.b) ||
            (existing_edge.a == edge.b && existing_edge.b == edge.a)) {
            return true;
        }
    }
    return false;
}

void Graph::removeNode(size_t nodeId){
    if (!getNode(nodeId)) {
        throw std::out_of_range("Node is not in the Graph.");
    }

    auto iterEdge = edgesArr.begin();
    while (iterEdge != edgesArr.end()) {
        if (iterEdge->a == nodeId || iterEdge->b == nodeId) {
            iterEdge = edgesArr.erase(iterEdge);
        } else {
            ++iterEdge;
        }
    }

    auto iterNode = nodesArr.begin();
    while( iterNode != nodesArr.end()) {  
        if((*iterNode).id == nodeId) {
            break;
        } else {
            ++iterNode;
        }
    }

    nodesArr.erase(iterNode);
}

void Graph::removeEdge(const Edge& edge){
    auto iter = edgesArr.begin();
    while (iter != edgesArr.end()) {
        if((*iter).a == edge.a || (*iter).b == edge.a || (*iter).a == edge.b || (*iter).b == edge.b) {
            break;
        }
        iter++;
    }

    if (!containsEdge(edge)) {
        throw std::out_of_range("Node is not in the Graph.");
    }

    edgesArr.erase(iter);
}

size_t Graph::nodeCount() const{
    return nodesArr.size();
}

size_t Graph::edgeCount() const{
    size_t count = 0;
    for (const auto& edge : edgesArr) {
        count++;
    }

    return count;
}

size_t Graph::nodeDegree(size_t nodeId) const{

    bool nodeExists = false;
    for (const auto& node : nodesArr) {
        if (node.id == nodeId) {
            nodeExists = true;
            break;
        }
    }
    if (!nodeExists) {
        throw std::out_of_range("Node is not in the Graph.");
    }

    size_t degree = 0;
    for (const auto& edge : edgesArr) {
        if (edge.a == nodeId || edge.b == nodeId) {
            ++degree;
        }
    }

    return degree;
}

size_t Graph::graphDegree() const{
    size_t degree = 0;

    for(auto node : nodesArr) {
        size_t newDegree = nodeDegree(node.id);
        if(newDegree > degree) {
            degree = newDegree;
        }
    }

    return degree;
}

void Graph::coloring() {
    for (auto& node : nodesArr) {
        node.color = -1;
    }

    for (auto& node : nodesArr) {
        std::vector<int> colors = { -1 };

        for (auto& edge : edgesArr) {
            if (edge.a == node.id || edge.b == node.id) {

                Node* otherNode;
                if (edge.a == node.id) {
                    otherNode = getNode(edge.b);
                } else {
                    otherNode = getNode(edge.a);
                }

                colors.push_back(otherNode->color);
            }
        }
    
        bool foundColor = false;
        int color = 1;

        for (int idx = 0; idx < colors.size(); idx++) {
            if (colors[idx] == color) {
                color++;
                idx = -1;
            }

            if (idx == colors.size() - 1) {
                foundColor = true;
                node.color = color;
            }
        } 
    }      
}


void Graph::clear() {
    for (auto& edge : edgesArr) {
        removeEdge(edge);
    }

    while(nodesArr.size() > 0) {
        for (auto& node : nodesArr) {
            if(getNode(node.id)) {
                removeNode(node.id);
            }
        }
    }
}

/*** Konec souboru tdd_code.cpp ***/