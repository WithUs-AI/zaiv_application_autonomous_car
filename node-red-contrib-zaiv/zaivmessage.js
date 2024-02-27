module.exports = function (RED) {

    function ZaivMessageNode(config) {
        RED.nodes.createNode(this, config);
        var node = this;


        // Store local copies of the node configuration (as defined in the .html)
        node.module = config.module;
        node.method = config.method;
        node.command = config.command;
        node.attribute = config.attribute;


        var flowContext = node.context().flow;
        var nodeContext = this.context();
        var globalContext = this.context().global;


        function zaivSendWS(payload) {
            var strMessage = '{"method":"post", "cmd":"config","attr":"class","payload":{["car": 1, "person": 0]}}';
            strMessage += '{\"module\":\"';
            strMessage += this.module;
            strMessage += '\"';
            strMessage += ',';
            strMessage += '{\"method\":\"';
            strMessage += this.method;
            strMessage += '\"';
            strMessage += ',';
            strMessage += '\"cmd\":\"';
            strMessage += this.command;
            strMessage += '\"';
            strMessage += ',';
            strMessage += '\"attr\":\"';
            strMessage += this.attribute;
            strMessage += '\"';
            strMessage += ',';
            strMessage += '\"payload\":';
            strMessage += payload;
            strMessage += '}';
            strMessage += ']}';

            msg = {
                payload: JSON.parse(strMessage)
            }
            node.send(msg);
        }

        // We received something from the Node input
        node.on('input', function (msg) {
            var strPayload = "";

            var value = RED.util.getMessageProperty(msg, "payload");
            if (value !== undefined) {
                if (typeof value === "string") {    //payload is a string
                    strPayload = value;
                }
                else if (typeof value === "number") {      //payload is an integer
                    strPayload = value.toString();   // we make it a string
                }
            }

            if ((this.module !== undefined) &&
                (this.method !== undefined) &&
                (this.command !== undefined) &&
                (this.attribute !== undefined)) {
                node.trace("Send ws packet");
                zaivSendWS(strPayload);
            }
            else {
                node.status({
                    fill: "red",
                    shape: "ring",
                    text: "Variable not specified"
                });
            }

        });
    }
    RED.nodes.registerType("zaiv message", ZaivMessageNode);
}