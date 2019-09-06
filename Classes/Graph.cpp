#include "Graph.h"

#include <assert.h>

Graph::Graph(){
}

Graph::~Graph(){
	clear();
}

void Graph::addVert(Dec2 dec2, bool isPivot){
	GraphNode *graphNode = new GraphNode(isPivot);
	graphNodes[dec2] = graphNode;
}
void Graph::removeVert(Dec2 dec2){
	GraphNode *graphNode = graphNodes[dec2];

	std::set<GraphNode *> edges = graphNode->edges;

	for (GraphNode *edge : edges){
		removeEdge(graphNode, edge);
	}

	graphNodes.erase(graphNodes.find(dec2));
	delete graphNode;
}

void Graph::addEdge(Dec2 decA, Dec2 decB){
	addEdge(graphNodes[decA], graphNodes[decB]);
}

void Graph::addEdge(GraphNode *gA, GraphNode *gB){

	gA->edges.insert(gB);
	gB->edges.insert(gA);

	int gAPivotRank = gA->pivotRank;
	int gBPivotRank = gB->pivotRank;



	GraphNode *updateNode = nullptr;
	GraphNode *notUpdateNode = nullptr;

	if (gAPivotRank == -1 && gBPivotRank != -1){
		updateNode = gA;
		notUpdateNode = gB;
	}
	else if (gBPivotRank == -1 && gAPivotRank != -1){
		updateNode = gB;
		notUpdateNode = gA;
	}
	else if (gAPivotRank < gBPivotRank - 1){
		updateNode = gB;
		notUpdateNode = gA;
	}
	else if (gBPivotRank < gAPivotRank - 1){
		updateNode = gA;
		notUpdateNode = gB;
	}

	if (nullptr == updateNode && nullptr == notUpdateNode){
		return;
	}

	std::set<GraphNode *> frontier;
	frontier.insert(updateNode);

	for (int updatePivotRank = notUpdateNode->pivotRank + 1; !frontier.empty(); updatePivotRank++){
		for (GraphNode *front : frontier){
			front->pivotRank = updatePivotRank;
		}

		std::set<GraphNode *> newFrontier;
		for (GraphNode *front : frontier){
			for (GraphNode *edge : front->edges){
				if (edge->pivotRank == -1 || (edge->pivotRank > updatePivotRank + 1)){
					newFrontier.insert(edge);
				}
			}
		}

		frontier = newFrontier;
	}
}

void Graph::removeEdge(Dec2 decA, Dec2 decB){
	removeEdge(graphNodes[decA], graphNodes[decB]);
}


void Graph::removeEdge(GraphNode *gA, GraphNode *gB){

	gA->edges.erase(gB);
	gB->edges.erase(gA);


	int gAPivotRank = gA->pivotRank;
	int gBPivotRank = gB->pivotRank;

	GraphNode *updateNode = nullptr;


	if (gAPivotRank < gBPivotRank){
		updateNode = gB;
	}
	else if (gBPivotRank < gAPivotRank){
		updateNode = gA;
	}

	if (nullptr == updateNode){
		return;
	}

	std::set<GraphNode *> normalizationFront;

	{
		std::set<GraphNode *> frontier;
		frontier.insert(updateNode);
		for (int frontierPivotRank = updateNode->pivotRank; !frontier.empty(); frontierPivotRank++){

			for (GraphNode *front : frontier){
				front->pivotRank = -1;
			}


			std::set<GraphNode *> newFrontier;
			for (GraphNode *front : frontier){
				for (GraphNode *edge : front->edges){
					if (edge->pivotRank > frontierPivotRank){
						newFrontier.insert(edge);
					}
					else if (edge->pivotRank != -1){
						normalizationFront.insert(edge);
					}
				}
			}

			frontier = newFrontier;
		}
	}


	for (GraphNode *normFront : normalizationFront){
		std::set<GraphNode *> frontier;
		for (GraphNode *edge : normFront->edges){
			if (edge->pivotRank == -1 || edge->pivotRank > normFront->pivotRank + 1){
				frontier.insert(edge);
			}
		}

		for (int updatePivotRank = normFront->pivotRank + 1; !frontier.empty(); updatePivotRank++){
			for (GraphNode *front : frontier){
				front->pivotRank = updatePivotRank;
			}

			std::set<GraphNode *> newFrontier;
			for (GraphNode *front : frontier){
				for (GraphNode *edge : front->edges){
					if (edge->pivotRank == -1 || (edge->pivotRank > updatePivotRank + 1)){
						newFrontier.insert(edge);
					}
				}
			}

			frontier = newFrontier;
		}
	}

}


void Graph::clear(){
	for (std::pair<Dec2, GraphNode *> graphNodesPair : graphNodes){
		delete graphNodesPair.second;
	}

	graphNodes.clear();
}

int Graph::getPivotRank(Dec2 dec2){
	return getPivotRank(graphNodes[dec2]);
}
int Graph::getPivotRank(GraphNode *graphNode){
	return graphNode->pivotRank;
}

