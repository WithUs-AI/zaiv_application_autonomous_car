module.exports = function (RED) {



    function ZaivParserMetadataNode(config) {
        RED.nodes.createNode(this, config);
        var node = this;


        // Store local copies of the node configuration (as defined in the .html)
        node.port_count = 1;

        var flowContext = node.context().flow;
        var nodeContext = this.context();
        var globalContext = this.context().global;


        this.subs = config.subs || [];

        for (var i = 0; i < this.subs.length; i += 1) {
            var sub = this.subs[i];
            //console.trace("sub " + sub);
        }

        node.port_count = node.subs.length + 1;

        function treatReqAnswer(pObj) {
            var i;
            var ports = new Array(node.port_count);

            for (i = 0; i < node.port_count; i += 1) {
                ports[i] = 0;
            }

            if (typeof pObj !== undefined) {

                console.trace("pObj.payload.length " + pObj.payload.length);
                console.trace("node.port_count " + node.port_count);

                for (idx = 0; idx < pObj.payload.length; idx++) {
                    if (idx >= node.port_count)
                        break;

                    msg = {
                        topic: pObj.payload[idx].class,
                        payload: 1
                    };
                    ports[idx] = msg;
                }

                //send msg
                node.send(ports);
                //}
            }
        }

        // We received something from the Node input
        node.on('input', function (msg) {
            var p;

            if (msg !== undefined) {
                if (typeof msg === "string") {    //payload is a string
                    console.trace(msg);
                    try {
                        p = JSON.parse(msg);
                    } catch (err) {
                        console.trace("parse failed");
                        node.status({
                            fill: "red",
                            shape: "dot",
                            text: "Failed to parse from input string"
                        });
                    }
                }
                else if (typeof msg === "object") {      //payload is an object
                    console.trace("It is object!");
                    console.trace(JSON.stringify(msg));   // we make it a string to be transfered on the WS
                    p = msg;
                }
                else {
                    console.trace("Unknown object");
                    node.status({
                        fill: "red",
                        shape: "dot",
                        text: "Unknown object."
                    });
                }
            }
            else {
                console.trace("parse failed");
                node.status({
                    fill: "red",
                    shape: "dot",
                    text: "Undefined input message."
                });
            }

            if (p !== undefined) {
                treatReqAnswer(p);

                node.status({
                    fill: "green",
                    shape: "dot",
                    text: "ok"
                });
            }

            //if (typeof msg !== 'undefined') {
                //console.trace("socket:" + p.cmd);

                //Node send payload


        });
    }
    RED.nodes.registerType("zaiv parser metadata", ZaivParserMetadataNode);
}