//-----------------------------------------
//
// Generated by nedtool, version 3.3
// date: Sun Jul 20 10:27:02 2008
//
//-----------------------------------------


#include <math.h>
#include "omnetpp.h"

// NEDC version check
#define NEDC_VERSION 0x0303
#if (NEDC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedc: 'make clean' should help.
#endif

// Disable warnings about unused variables. For MSVC and BC only:
// GCC has no way to turn on its -Wunused option in a source file :(
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#endif
#ifdef __BORLANDC__
#  pragma warn -waus
#  pragma warn -wuse
#endif

static cModuleType *_getModuleType(const char *modname)
{
    cModuleType *modtype = findModuleType(modname);
    if (!modtype)
        throw new cRuntimeError("Module type definition %s not found (Define_Module() missing from C++ code?)", modname);
    return modtype;
}

static void _checkModuleVectorSize(int vectorsize, const char *mod)
{
    if (vectorsize<0)
        throw new cRuntimeError("Negative module vector size %s[%d]", mod, vectorsize);
}

static void _readModuleParameters(cModule *mod)
{
    int n = mod->params();
    for (int k=0; k<n; k++)
        if (mod->par(k).isInput())
            mod->par(k).read();
}

static int _checkModuleIndex(int index, int vectorsize, const char *modname)
{
    if (index<0 || index>=vectorsize)
        throw new cRuntimeError("Submodule index %s[%d] out of range, sizeof(%s) is %d", modname, index, modname, vectorsize);
    return index;
}

static cGate *_checkGate(cModule *mod, const char *gatename)
{
    cGate *g = mod->gate(gatename);
    if (!g)
        throw new cRuntimeError("%s has no gate named %s",mod->fullPath().c_str(), gatename);
    return g;
}

static cGate *_checkGate(cModule *mod, const char *gatename, int gateindex)
{
    cGate *g = mod->gate(gatename, gateindex);
    if (!g)
        throw new cRuntimeError("%s has no gate %s[%d]",mod->fullPath().c_str(), gatename, gateindex);
    return g;
}

static cGate *_getFirstUnusedParentModGate(cModule *mod, const char *gatename)
{
    int baseId = mod->findGate(gatename);
    if (baseId<0)
        throw new cRuntimeError("%s has no %s[] gate",mod->fullPath().c_str(), gatename);
    int n = mod->gate(baseId)->size();
    for (int i=0; i<n; i++)
        if (!mod->gate(baseId+i)->isConnectedInside())
            return mod->gate(baseId+i);
    throw new cRuntimeError("%s[] gates are all connected, no gate left for `++' operator",mod->fullPath().c_str(), gatename);
}

static cGate *_getFirstUnusedSubmodGate(cModule *mod, const char *gatename)
{
    int baseId = mod->findGate(gatename);
    if (baseId<0)
        throw new cRuntimeError("%s has no %s[] gate",mod->fullPath().c_str(), gatename);
    int n = mod->gate(baseId)->size();
    for (int i=0; i<n; i++)
        if (!mod->gate(baseId+i)->isConnectedOutside())
            return mod->gate(baseId+i);
    int newBaseId = mod->setGateSize(gatename,n+1);
    return mod->gate(newBaseId+n);
}

static cFunctionType *_getFunction(const char *funcname, int argcount)
{
    cFunctionType *functype = findFunction(funcname,argcount);
    if (!functype)
        throw new cRuntimeError("Function %s with %d args not found", funcname, argcount);
    return functype;
}

static cChannel *_createChannel(const char *channeltypename)
{
    cChannelType *channeltype = findChannelType(channeltypename);
    if (!channeltype)
        throw new cRuntimeError("Channel type %s not found", channeltypename);
    cChannel *channel = channeltype->create("channel");
    return channel;
}

static cChannel *_createNonTypedBasicChannel(double delay, double error, double datarate)
{
    cBasicChannel *channel = new cBasicChannel("channel");
    if (delay!=0) channel->setDelay(delay);
    if (error!=0) channel->setError(error);
    if (datarate!=0) channel->setDatarate(datarate);
    return channel;
}

static cXMLElement *_getXMLDocument(const char *fname, const char *pathexpr=NULL)
{
    cXMLElement *node = ev.getXMLDocument(fname, pathexpr);
    if (!node)
        throw new cRuntimeError(!pathexpr ? "xmldoc(\"%s\"): element not found" : "xmldoc(\"%s\", \"%s\"): element not found",fname,pathexpr);
    return node;
}

ModuleInterface(Diffusion)
    // gates:
    Gate(in[], GateDir_Input)
    Gate(out[], GateDir_Output)
EndInterface

Register_ModuleInterface(Diffusion)

//// Sample code:
// class Diffusion : public cSimpleModule
// {
//     Module_Class_Members(Diffusion,cSimpleModule,16384)
//     virtual void activity();
//     // Add you own member functions here!
// };
//
// Define_Module(Diffusion);
//
// void Diffusion::activity()
// {
//     // Put code for simple module activity here!
// }
//

ModuleInterface(Node)
EndInterface

Register_ModuleInterface(Node);

class Node : public cCompoundModule
{
  public:
    Node() : cCompoundModule() {}
  protected:
    virtual void doBuildInside();
};

Define_Module(Node);

void Node::doBuildInside()
{
    cModule *mod = this;

    // temporary variables:
    cPar tmpval;
    const char *modtypename;

    // submodules:
    cModuleType *modtype = NULL;
    int submodindex;

    //
    // submodule 'node':
    //
    int node_size = (int)(7);
    _checkModuleVectorSize(node_size,"node");
    cModule **node_p = new cModule *[node_size];
    if (node_size>0)
    {
        modtype = _getModuleType("Diffusion");
    }
    for (submodindex=0; submodindex<node_size; submodindex++)
    {
        node_p[submodindex] = modtype->create("node", mod, node_size, submodindex);
        cContextSwitcher __ctx(node_p[submodindex]); // do the rest in this module's context

        _readModuleParameters(node_p[submodindex]);
        node_p[submodindex]->setDisplayString("i=misc/node_s");
    }

    //
    // connections:
    //
    cGate *srcgate, *destgate;

    cChannel *channel;
    cPar *par;
    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(6),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(5),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(5),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(6),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(6),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(4),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(4),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(6),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(6),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(6),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(4),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(4),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(5),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(5),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(2),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(2),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(1),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(1),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(0),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(0),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(3),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(4),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(2),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(2),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(4),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(5),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(1),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(1),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(5),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(2),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(0),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(0),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(2),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(1),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(0),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // connection
    srcgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(0),node_size,"node")], "out");
    destgate = _getFirstUnusedSubmodGate(node_p[_checkModuleIndex((int)(1),node_size,"node")], "in");
    channel = _createNonTypedBasicChannel(0.1, 0, 0);
    srcgate->connectTo(destgate,channel);

    // check all gates are connected:
    mod->checkInternalConnections();


    //
    // this level is done -- recursively build submodules too
    //
    for (submodindex=0; submodindex<node_size; submodindex++)
        node_p[submodindex]->buildInside();
    delete [] node_p;
}

class node : public cNetworkType
{
  public:
    node(const char *name) : cNetworkType(name) {}
    node(const node& n) : cNetworkType(n.name()) {operator=(n);}
    virtual void setupNetwork();
};

Define_Network(node);

void node::setupNetwork()
{
    // temporary variables:
    cPar tmpval;
    const char *modtypename;

    cModuleType *modtype;

    modtype = _getModuleType("Node");
    cModule *node_p = modtype->create("node", NULL);

    cContextSwitcher __ctx(node_p); // do the rest in this module's context
    _readModuleParameters(node_p);
    // build submodules recursively (if it has any):
    node_p->buildInside();
}

