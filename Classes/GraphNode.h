#ifndef _GRAPH_NODE_H_
#define _GRAPH_NODE_H_

#include <set>
#include <map>

#include "Dec2.h"


class GraphNode {
public:
	GraphNode(bool _isPivot);

	bool isPivot;
	int pivotRank;

	std::set<GraphNode *> edges;

};

#endif //_GRAPH_NODE_H_