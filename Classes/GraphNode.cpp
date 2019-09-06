#include "GraphNode.h"
#include <assert.h>

GraphNode::GraphNode(bool _isPivot):isPivot(_isPivot){
	if (isPivot){
		pivotRank = 0;
	}
	else {
		pivotRank = -1;
	}
}
