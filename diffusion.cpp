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

Define_Module(Diffusion);		//��OMNeT��ע��ģ����

void Diffusion::initialize()
{
  // node[0], ��sink�ڵ㷢�͵�һ����ѯ��Ϣ��interest message��
  if (index() == 0)
  {
    // ����һ��self-message��Ϊ������Ϣ
    intMessage *msg = genMessage();
    scheduleAt(0.0, msg);
  }
}

void Diffusion::handleMessage(cMessage *msg)
{
	  if (simTime() < 0.3) 
	  {
		ev << simTime() << "  ";
		intMessage *intMsg = check_and_cast<intMessage *>(msg);	//ǿ������ת��
		forwardMessage(intMsg);	//ת����Ϣ
	  }
	
	  else 
	  {		
		//��ȡ��������˽ṹ
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
		
		//�������·���㷨�󵽴�Ŀ��ڵ�node(6)�����·�� ������������
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

//��Ϣ�����ɺ���
intMessage* Diffusion::genMessage()
{
  // ����Դ�ڵ��Ŀ��ڵ�ĵ�ַ.
  int src = index();   // ģ��ID
  int n = size();      // ģ��������С
  int dest = intuniform(0,n-1);
  
  char msgName[20];

  // ������Ϣ��������Դ�ڵ��Ŀ��ڵ�����Ա.
  intMessage *msg = new intMessage(msgName);
  msg->setSource(src);
  msg->setDestination(dest);
  return msg;
}

//ת����Ϣ������
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

//�ַ�������ʾ����
void Diffusion::updateDisplay()
{
char buf[40];
sprintf(buf, "Best Path!");
displayString().setTagArg("t", 0, buf);
}
