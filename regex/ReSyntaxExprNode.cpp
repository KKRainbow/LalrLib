#include "ReSyntaxExprNode.h"
RE_BEGIN_INTERNAL_NAMESPACE

void ReSyntaxExprNode::Welcome(ReSyntaxExprNode* _Node, ReSyntaxExprNodeVistor* _Vistor)
{
	if (!_Node)return;
	switch (_Node->NodeType())
	{
		case OR:
			_Vistor->VisitReOrExprNode(static_cast<ReOrExprNode*>(_Node));
			break;
		case CAT:
			_Vistor->VisitReConcatExprNode(static_cast<ReConcatExprNode*>(_Node));
			break;
		case CHAR:
			_Vistor->VisitReCharExprNode(static_cast<ReCharExprNode*>(_Node));
			break;
		case CAPTURE:
			_Vistor->VisitReCapturedGroupExprNode(static_cast<ReCapturedGroupExprNode*>(_Node));
			break;
		case SELECTION:
			_Vistor->VisitReSelectionExprNode(static_cast<ReSelectionExprNode*>(_Node));
			break;
		case LOOK:
			_Vistor->VisitReLookAroundExprNode(static_cast<ReLookAroundExprNode*>(_Node));
			break;
		case BACK:
			_Vistor->VisitReBackTracedGroupExprNode(static_cast<ReBackTracedGroupExprNode*>(_Node));
			break;
		case BACKREF:
			_Vistor->VisitReBackReferenceGroupExprNode(static_cast<ReBackReferenceGroupExprNode*>(_Node));
			break;
		default:
			__LLIB_ASSERT(false);
	}
}

void ReOrExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	ReSyntaxExprNode::Accept(_Vistor);
	Welcome(this, _Vistor);
}
void ReConcatExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	ReSyntaxExprNode::Accept(_Vistor);
	Welcome(this, _Vistor);
}
void ReCapturedGroupExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	ReSyntaxExprNode::Accept(_Vistor);
	Welcome(this, _Vistor);
}
void ReBackTracedGroupExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	ReSyntaxExprNode::Accept(_Vistor);
	Welcome(this, _Vistor);
}
void ReSelectionExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	Welcome(this, _Vistor);
}
void ReLookAroundExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	ReSyntaxExprNode::Accept(_Vistor);
	Welcome(this, _Vistor);
}
void ReCharExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	Welcome(this, _Vistor);
}
void ReBackReferenceGroupExprNode::Accept(ReSyntaxExprNodeVistor* _Vistor)
{
	ReSyntaxExprNode::Accept(_Vistor);
	Welcome(this, _Vistor);
}
RE_END_INTERNAL_NAMESPACE