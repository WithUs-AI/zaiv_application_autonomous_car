module.exports = function (RED) {
    var ws = require("ws");
    var url = require('url');

    var abortConnection = function (socket) {
        var response;
        try {
            response = ["HTTP/1.1 401 Unauthorized", "Content-type: text/html"];
            return socket.write(response.concat("", "").join("\r\n"));
        } finally {
            try {
                socket.destroy();
            } catch (_error) {
                console.log(_error);
            }
        }
    };

    var upgradeConnection = function (socket) {
        return socket.write('HTTP/1.1 101 Web Socket Protocol Handshake\r\n' + 'Upgrade: WebSocket\r\n' + 'Connection: Upgrade\r\n' + '\r\n');
    };


    function ZaivWSNode(config) {
        RED.nodes.createNode(this,config);
        var node = this;

        //Get Var list members
        node.listVariables = [];
        node.statusVarList = -1;
        node.socketVarList = null;

        // Store local copies of the node configuration (as defined in the .html)
        node.remoteUrl = config.address;
        node.closing = false;
        node.disconnectCount = 0;
        node.port_count = 1;

        var flowContext = node.context().flow;
        var nodeContext = this.context();
        var globalContext = this.context().global;
        flowContext.set("zaivWS_disconnect_count", node.disconnectCount);

        this.subs = config.subs || [];

        for (var i = 0; i < this.subs.length; i += 1) {
            var sub = this.subs[i];
            //node.trace("sub " + sub);
        }

        node.port_count = node.subs.length + 1;

        node.status({
            fill: "red",
            shape: "ring",
            text: "disconnected"
        });


        function treatReqAnswer(pObj) {
            var i,j;
            var ports = new Array(node.port_count);

            for (i = 0; i < node.port_count; i += 1) {
                ports[i] = null;
            }

            if (typeof pObj !== 'undefined') {
                // node.trace("pObj.cmd " + pObj.cmd);
                // node.trace("pObj.error " + pObj.error);
                // node.trace("pObj.data " + pObj.data);

                //if ((typeof pObj.cmd !== 'undefined')) {
                    //node.trace("sendMsgToPorts: " + node.port_count);

                    msg = {
                        module: pObj.module,
                        method: pObj.method,
                        cmd: pObj.cmd,
                        attr: pObj.attr,
                        sc: pObj.sc,
                        payload: pObj.payload
                    };

                    //ports[node.port_count-1] = msg; //always set the msg (full object) to the LAST output (general)
                for (idx = 0; idx < node.port_count; idx++) {
                        ports[idx] = msg;
                    }
                    // // For other port only get them if read2
                    // if (pObj.cmd == "read" && pObj.error == 200 && (typeof pObj.data !== 'undefined') && pObj.data.length > 0) {

                    //     //For each variable in the message from websocket
                    //     for (i = 0; i < pObj.data.length; i++) {
                    //         //search for the port index
                    //         for (j = 0; j < node.subs.length; j += 1) {
                    //             var sub = node.subs[j];
                    //             if (pObj.data[i].name == sub) {
                    //                 //node.trace("found " + pObj.data[0].name + " at " + i);

                    //                 msg = {
                    //                     cmd: pObj.cmd,
                    //                     payload: pObj.data[i].value,
                    //                     statusCode: pObj.error,
                    //                     extStatusCode: pObj.extError
                    //             };

                    //                 ports[j] = msg;
                    //             }
                    //         }
                    //     }
                    // }

                    //send msg
                    node.send(ports);
                //}
            }
        }

        function startconn() {// Connect to remote endpoint
            //node.trace(node.remoteUrl);
            var socket = new ws(node.remoteUrl);
            node.server = socket;
            // keep for closing
            handleConnection(socket);
        }

        function sendWS(cmd) {
            //node.trace("sendCommand " + cmd);
			try {
					if (node.server !== null) {
            node.server.send(cmd);
					}
                } catch (err) {                 // Error
                }
        }


        function handleConnection(/*socket*/socket)
        {
            socket.on('open', function ()
            {
                //node.trace("open");

                //zaivGetStatusWS();

                //node.emit('opened', '');

                node.status({
                    fill: "green",
                    shape: "dot",
                    text: "connected "
                });
            });

            socket.on('close', function ()
            {
                node.disconnectCount++;
                flowContext.set("zaivWS_disconnect_count", node.disconnectCount);

                //node.trace("close");

                node.status({
                    fill: "red",
                    shape: "ring",
                    text: "disconnected"
                });

                //node.emit('closed');

                if (!node.closing) {
                    node.tout = setTimeout(function () {
                        startconn();
                    }, 30000);
                    // try to reconnect every 30 secs
                }
            });

            // We received a message on the WebSocket
            socket.on('message', function (data, flags)
            {
                node.trace(data);
                var p;
                try {
                    p = JSON.parse(data);
                } catch (err) {
                    node.trace("parse failed");
                }

                if (typeof p !== 'undefined')
                {
                    node.trace("socket:" + p.cmd);

                    //if the message contains the field error
                    if (typeof p.error !== 'undefined')
                    {
                        if (p.error != 200)
                        {
                            node.status({
                                fill: "yellow",
                                shape: "dot",
                                text: p.error + " " + p.errorTxt
                            });
                        }
                        else
                        {
                            node.status({
                                fill: "green",
                                shape: "dot",
                                text:"connected"
                            });
                        }
                    }

                    //Node send payload
                    treatReqAnswer(p);
                }


            });

            socket.on('error', function (err)
            {
                //node.trace("error");

                //node.emit('erro');
                if (!node.closing) {
                    node.tout = setTimeout(function () {
                        startconn();
                    }, 30000);
                    // try to reconnect every 30 secs
                }
            });
        }


        function handleAuthentication(req, socket, head) {
            var authorization,
			    jwtToken,
			    parts,
			    scheme;
            authorization = req.headers.authorization;
            if (typeof (node.auth0) == "undefined" || !node.auth0) {
                return upgradeConnection(socket);
            }

            if (!authorization) {
                return abortConnection(socket, 400, 'Bad Request');
            }
            parts = authorization.split(" ");
            if (parts.length !== 2) {
                return abortConnection(socket, 400, 'Bad Request');
            }
            scheme = parts[0];
            jwtToken = parts[1];
            if ("Bearer" !== scheme || !jwtToken) {
                return abortConnection(socket, 400, 'Bad Request');
            }

            var request = require('request');
            var options = {
                uri: node.auth0,
                method: 'POST',
                json: {
                    id_token: jwtToken
                }
            };
            request(options, function (error, response, body) {
                if (!error && response.statusCode == 200) {
                    req.tokeninfo = body || {};
                    req.tokeninfo.authorized = true;
                    if (node.role && req.tokeninfo && req.tokeninfo.roles && req.tokeninfo.roles.indexOf(node.role) == -1) {
                        req.tokeninfo.authorized = false;
                    }
                    if (node.group && req.tokeninfo && req.tokeninfo.groups && req.tokeninfo.groups.indexOf(node.group) == -1) {
                        req.tokeninfo.authorized = false;
                    }
                    if (req.tokeninfo.authorized) {
                        return upgradeConnection(socket);
                    } else {
                        return abortConnection(socket, 401, 'Unauthorized - HEKKK');
                    }
                } else {
                    return abortConnection(socket, 503, 'The authentication service is unavailable.');
                }
            });
        }


        // start outbound connection
        node.closing = false;
        startconn();

        node.on("close", function () {
            // Workaround https://github.com/einaros/ws/pull/253
            // Remove listeners from RED.server
            node.closing = true;
            node.server.close();
            if (node.tout) {
                clearTimeout(node.tout);
            }
        });

        // We received something from the Node input
        node.on('input', function (msg) {
            var strCmd = "";

            var value = RED.util.getMessageProperty(msg,"payload");
            if (value !== undefined) {
                if (typeof value === "string") {    //payload is a string
                    strCmd = value;
                }
                else if (typeof value === "object") {      //payload is an object
                    strCmd = JSON.stringify(msg.payload);   // we make it a string to be transfered on the WS
                }
            }

            if( strCmd !== '' )
            {
                //node.trace("input " + strCmd);
                sendWS(strCmd);
            }
        });
    }
    RED.nodes.registerType("zaiv websocket", ZaivWSNode);

    // Create an admin configuration API endpoint
    // Sometimes a custom node needs to be able to look up additional data at configuration time (e.g. when editing the configuration of a node instance in the admin interface).
    // The approved way to do this is for the node to register an extra http endpoint to use as an API.
    // see https://github.com/node-red/cookbook.nodered.org/wiki/Create-an-admin-configuration-API-endpoint
    // Handle request to get the list of variables
    //RED.httpAdmin.get('/zaivvarlist/:id', RED.auth.needsPermission('zaivread.read'), function (req, res) {
    RED.httpAdmin.post('/zaivvarlist/:id/:remoteurl', RED.auth.needsPermission('zaivws.read'), function (req, res) {
        console.log("inside httpAdmin" + req.params.id + req.params.remoteurl);

        msg = {
            status: -1,
            data: []
        };

        var node = RED.nodes.getNode(req.params.id);
        if (node !== null && typeof node !== "undefined") {

            var reqnodeid = req.params.id;

            function handleConnectionGetVarList(socket) {

                // On open socket, we wend a command to get the lsit of variables
                // We empty the array of var names
                // We set status to 0 : in process
                socket.on('open', function () {
                    //socket.send("{\"cmd\":\"list\"}");
                    node.statusVarList = 0;
                    node.listVariables = [];
                });

                socket.on('close', function () {
                });

                // On receiving message socket
                socket.on('message', function (data, flags) {
                    //node.trace(data);
                    var pObj;
                    try {
                        pObj = JSON.parse(data);
                    } catch (err) {                 // Error
                        node.statusVarList = 1;     // Operation done
                        node.listVariables = [];    // No variables
                    }

                    if (typeof pObj !== 'undefined') {
                        if (pObj.cmd == "list") {
                            node.statusVarList = 1;     // Operation done
                            node.listVariables = [];    // Empty array

                            if (pObj.cmd == "list" && pObj.error == 200 && (typeof pObj.data !== 'undefined') && pObj.data.length > 0) {

                                //For each variable in the message from websocket
                                for (i = 0; i < pObj.data.length; i++) {
                                    node.listVariables.push(/*reqnodeid + */pObj.data[i]);  // put var name in array
                                }
                            }
                        }
                    }

                    socket.close();
                    node.socketVarList = null;
                });

                socket.on('error', function (err) {
                });
            }

            // We open a socket and try to get the variable list
            // if there is no operation currently and the socket is closed
            if (node.statusVarList == -1) {
                if (node.socketVarList === null) {
                    node.listVariables = [];
                    node.statusVarList = 0;

                    node.socketVarList = new ws("ws://" + req.params.remoteurl);
                    handleConnectionGetVarList(node.socketVarList);
                }
            }

            msg = {
                status: node.statusVarList,
                data: node.listVariables
            };

            //for (var i = 0; i < node.listVariables.length; i++) {
            //    console.log(node.listVariables[i]);
            //}

        } else {
            // if node not saved I'm here.
            msg.status = -1;
        }

        res.json(msg);
    });


}
