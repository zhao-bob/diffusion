#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "node_m.h"

class Diffusion : public cSimpleModule {
protected:
  virtual intMessage *genMessage();
  virtual void forwardMessage(intMessage *msg);
  virtual void updateDisplay();

  virtual void initialize();
  virtual void handleMessage(cMessage *msg);
};

Define_Module(Diffusion);		//在OMNeT中注册模块类

void Diffusion::initialize()
{
  // node[0], 即sink节点发送第一个查询消息（interest message）
  if (index() == 0)
  {
    // 调用一个self-message作为启动消息
    intMessage *msg = genMessage();
    scheduleAt(0.0, msg);
  }
}

void Diffusion::handleMessage(cMessage *msg)
{
	  if (simTime() < 0.3) 
	  {
		ev << simTime() << "  ";
		intMessage *intMsg = check_and_cast<intMessage *>(msg);	//强制类型转换
		forwardMessage(intMsg);	//转发消息
	  }
	
	  else 
	  {		
		//提取网络的拓扑结构
		cTopology topo;
		topo.extractByModuleType("Diffusion",NULL);
		
		for (int i=0; i<topo.nodes(); i++)
		{
		  cTopology::Node *node = topo.node(i);
		  ev << "Node i=" << i<<  " is " << node->module()->fullPath()<< endl;
		  ev<<"It has " << node->outLinks()<<" conns to other nodes\n";
		  ev<< "and " << node->inLinks()<<" conns from other nodes\n";
		  ev << "Connections to other modules are:\n";
			
		  for(int j=0;j<node->outLinks();j++)
		  {
		    cTopology::Node *neighbour = node->out(j)->remoteNode();
		    cGate *gate = node->out(j)->localGate();
		    ev << "	" << neighbour->module()->fullPath()
		      << " through gate " << gate->fullName() << endl;
		  }
		}
		
		//调用最短路径算法求到达目标节点node(6)的最短路径 ，以跳数计算
		cTopology::Node *targetnode = topo.node(6);
		topo.unweightedSingleShortestPathsTo(targetnode);
		
		cTopology::Node *node = topo.node(0);
		
		if (node == NULL)
		{
		  ev << "We(" << fullPath() << ") are not included in the topology.\n";
		}
		else if (node->paths() == 0)
		{
		  ev << "No path to destination.\n";
		}
		else
		{
		  while (node != topo.targetNode())
		  {
			ev << "We are in " << node->module()->fullPath() << endl;
			ev << node->distanceToTarget() << " hops to go\n";
			ev << "There are " << node->paths()
			  << " equally good directions, taking the first one\n";
			cTopology::LinkOut *path = node->path(0);
			ev << "Taking gate" << path->localGate()->fullName()
			  <<" we arrive in" << path->remoteNode()->module()->fullPath()
			  << " on its gate" << path->remoteGate()->fullName() << endl;
			node = path->remoteNode();
		  }
		}
		
		if(index() == 6) updateDisplay();				
		if(index() == 3) updateDisplay();
		if(index() == 0) updateDisplay();
	  }
}

//消息包生成函数
intMessage* Diffusion::genMessage()
{
  // 生成源节点和目标节点的地址.
  int src = index();   // 模块ID
  int n = size();      // 模块向量大小
  int dest = intuniform(0,n-1);
  
  char msgName[20];

  // 创建消息对象设置源节点和目标节点的域成员.
  intMessage *msg = new intMessage(msgName);
  msg->setSource(src);
  msg->setDestination(dest);
  return msg;
}

//转发消息包函数
void Diffusion::forwardMessage(intMessage *msg)
{
  int n = gate("out")->size();
		
	  if (index() != 6) 
{
	    for( int i=0; i<n; i++) 
{
      ev << "Forwarding message " << "on port out[" << i << "]\n";
      intMessage *copy=(intMessage*) msg->dup();
      send(copy, "out", i);
    }
}
}

//字符更新显示函数
void Diffusion::updateDisplay()
{
char buf[40];
sprintf(buf, "Best Path!");
displayString().setTagArg("t", 0, buf);
}
