simple Diffusion
  gates:
     in: in[];
     out: out[];
endsimple

module Node
  submodules:
    node: Diffusion[7];   // we'll have 7 modules
    display: "i=misc/node_s";
  connections:
    node[6].out++ --> delay 100ms --> node[5].in++;
    node[6].in++ <-- delay 100ms <-- node[5].out++;
    node[6].out++ --> delay 100ms --> node[4].in++;
    node[6].in++ <-- delay 100ms <-- node[4].out++;
    node[6].out++ --> delay 100ms --> node[3].in++;
    node[6].in++ <-- delay 100ms <-- node[3].out++;
    node[3].out++ --> delay 100ms --> node[4].in++;
    node[3].in++ <-- delay 100ms <-- node[4].out++;
    node[3].out++ --> delay 100ms --> node[5].in++;
    node[3].in++ <-- delay 100ms <-- node[5].out++;
    node[3].out++ --> delay 100ms --> node[2].in++;
    node[3].in++ <-- delay 100ms <-- node[2].out++;
    node[3].out++ --> delay 100ms --> node[1].in++;
    node[3].in++ <-- delay 100ms <-- node[1].out++;
    node[3].out++ --> delay 100ms --> node[0].in++;
    node[3].in++ <-- delay 100ms <-- node[0].out++;
    node[4].out++ --> delay 100ms --> node[2].in++;
    node[4].in++ <-- delay 100ms <-- node[2].out++;
    node[5].out++ --> delay 100ms --> node[1].in++;
    node[5].in++ <-- delay 100ms <-- node[1].out++;
    node[2].out++ --> delay 100ms --> node[0].in++;
    node[2].in++ <-- delay 100ms <-- node[0].out++;
    node[1].out++ --> delay 100ms --> node[0].in++;
    node[1].in++ <-- delay 100ms <-- node[0].out++;
endmodule

network node : Node
endnetwork
