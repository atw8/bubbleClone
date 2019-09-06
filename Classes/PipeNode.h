#ifndef _PIPE_NODE_H_
#define _PIPE_NODE_H_

#include "cocos2d.h"
USING_NS_CC;

class PipeNode : public Node {
private:
	PipeNode(float _bodyHeight, float _bodyWidth, PhysicsMaterial _pipeNodeMaterial);
public:
	static PipeNode *create(float _bodyHeight, float _bodyWidth, PhysicsMaterial _pipeNodeMaterial);

	void onEnter() override;

	bool init() override;
	
	const Size &getContentSize() const override;

	float bodyHeight;
	const float bodyWidth;
	const PhysicsMaterial pipeNodeMaterial;

	void setBodyHeight(float _bodyHeight);
private:
	Sprite *pipeTop;
	Sprite *pipeBody;
	Sprite *pipeBottom;
};


#endif //_PIPE_NODE_H_