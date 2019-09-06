#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <map>

#include "Dec2.h"
#include "GraphNode.h"

class Graph {
public:
	Graph();

	~Graph();
private:
	std::map<Dec2, GraphNode *> graphNodes;

public:
	void addVert(Dec2 dec2, bool isPivot);
	void removeVert(Dec2 dec2);

	void addEdge(Dec2 decA, Dec2 decB);
	void addEdge(GraphNode *gA, GraphNode *gB);

	void removeEdge(Dec2 decA, Dec2 decB);
	void removeEdge(GraphNode *gA, GraphNode *gB);

public:
	void clear();
	int getPivotRank(Dec2 dec2);
	int getPivotRank(GraphNode *graphNode);

};

#endif //_GRAPH_H_