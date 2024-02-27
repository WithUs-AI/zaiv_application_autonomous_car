# Node-RED nodes for Zaiv

This package **node-red-contrib-zaiv** contains nodes to easily connect to a Zaiv runtime from [Withus](http://with-us.kr).

Using these nodes, you can connect to a Zaiv runtime to read and write application and system variables.

## Installation

Install using the managed palette from inside Node-RED.

### In Node-RED (preferred)

* Via **Manage Palette** -> **Search** for `node-red-contrib-zaiv`

### In a shell

* Go to the Node-RED user data directory, e.g.: `~/.node-red`
* Run `npm install -g node-red-contrib-zaiv --save` to install (If you want to install local source codes, run `npm install -g`)
* Run `npm ll -g` to check installed list.
* Run `npm uninstall -D @zaiv/node-red-contrib-zaiv` to uninstall

## Usage - Quick Overview

### Zaiv Websocket

Connects to a WebSocket and subscribes to one or several zaiv variables.
Additionally, it can send other commands when a message is coming in.

There will be an output for each subscribed variable.
Each time the value of a variable is changed, a message with value will be sent on the corresponding output.

An additional output will send message with the result of the command sent.


### Zaiv Message

Sends a command to its output when receive a message.
This node should be used in conjunction with Zaiv Websocket.

## Documentation

Each node has it's own help/documentation available in the help window.

## Licenses

MIT License

Copyright (c) 2023 Withus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## About

Copyright © 2020-2022 Withus. All rights reserved.

<http://with-us.kr>

