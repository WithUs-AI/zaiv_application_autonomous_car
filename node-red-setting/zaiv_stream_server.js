const WebSocket = require('ws');
const net = require('net');
const wss = new WebSocket.Server({ port: 8889 });
const http = require('http');
const fs = require('fs');

wss.on('connection', (ws) => {
  ws.on('message', (message) => {
    // 클라이언트로부터의 메시지 처리
    console.log(`Received message from client: ${message}`);
  });

  ws.on('close', () => {
    // 클라이언트 연결이 종료되었을 때 처리
    console.log('Client connection closed');
  });
});

function broadcast(data) {
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(data);
    } else {
      console.log('client readyState close');
    }
  });
}

const net_server = net.createServer((socket) => {
  console.log('Client connected');

  // 클라이언트로부터 데이터를 받았을 때의 이벤트 핸들러
  socket.on('data', (data) => {
    broadcast(data);
  });

  // 클라이언트와의 연결이 끊어졌을 때의 이벤트 핸들러
  socket.on('end', () => {
    console.log('Client disconnected');
  });
});

const html_server = http.createServer((req, res) => {
  if (req.url === '/') {
    // 루트 경로로 접속할 경우 test.html 파일을 읽어서 전송합니다.
    fs.readFile('/home/pi/.node-red/zaiv_viewer_no_internet.html', (err, data) => {
      if (err) {
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        res.end('Internal Server Error');
      } else {
        res.writeHead(200, { 'Content-Type': 'text/html' });
        res.end(data);
      }
    });
  } else {
    res.writeHead(404, { 'Content-Type': 'text/plain' });
    res.end('Not Found');
  }
});

// 서버 시작
net_server.listen(8888, 'localhost', () => {
  console.log('TCP server started on port 8888');
});

html_server.listen(8890, () => {
  console.log('Access the Test Web page : http://{zaiv ip}:8890');
});
