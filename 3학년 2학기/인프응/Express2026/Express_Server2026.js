const express = require("express");
const app = express();

/* localhost:8081/ 접속시 나올 메시지 */
app.get("/", (request, response) => { 
   response.sendFile(__dirname+"/index.html");
});

/* localhost:8081/서강대 접속시 나올 메시지 */
app.get("/Sogang", (request, response) => { 
    response.send(`<h1>ROOT 서강대학교 컴퓨터공학과 인터넷 프로그래밍 jhnang</h1>`);
});

/* localhost:8081/calculator.html 접속시 나올 메시지 */
app.get("/calculator", (request, response) => { 
   response.sendFile(__dirname+"/calculator.html");
});

/* localhost:8081/main 접속시 나올 메시지 */
app.get("/main", (request, response) => {  
  response.send(`
    <h1>Hello World</h1>
    <p>This is main page</p> 
    `);
});

app.get("/hello/:name", (request, response) => {
    response.json({message: 'Hello, ' + request.params.name + '!'}) ;
})

/* localhost:8081/ 혹은 localhost:8081/main 외의
get하지 않은 페이지 접속시 나올 메시지. */
app.use((request, response) => {
  response.send(`<h1>Sorry, page not found (msg by Express Server !! ):(</h1>`);
});

/* 8081 포트에서 서버 구동 */
app.listen(8081, () => {
  console.log("localhost:8081 에서 서버가 시작됩니다.");
});


/*

express는 node js를 더 쉽게 쓸 수 있게 해주는 그런거야?

네, 거의 맞아요. 정확히는 Node.js로 웹 서버를 더 쉽게 만들게 해주는 프레임워크예요.

Node.js 자체에도 http 모듈이 있어서 서버를 만들 수 있어요. 그런데 이걸로는 URL 분기, 파라미터 추출, JSON 응답을 전부 직접 해야 해요.

- 순수 Node.js
http.createServer((req, res) => {
  if (req.method === 'GET' && req.url.startsWith('/hello/')) {
    const name = req.url.split('/')[2];
    res.writeHead(200, {'Content-Type': 'application/json'});
    res.end(JSON.stringify({message: 'Hello, ' + name + '!'}));
  }
}).listen(3000);


Express를 쓰면 같은 걸 이렇게 짧게 쓸 수 있어요.
app.get('/hello/:name', (req, res) => res.json({message: 'Hello, ' + req.params.name + '!'}));
app.listen(3000);

Express가 해주는 주요 기능은 이거예요.

라우팅: app.get, app.post로 URL과 메서드별로 처리 함수 연결
편의 기능: req.params, req.query, res.json(), res.status() 등
미들웨어: app.use(express.json())처럼 모든 요청에 공통 처리 끼워 넣기
정적 파일 제공: app.use(express.static('public'))로 HTML/CSS/이미지 서빙

수업 흐름으로 보면 Apache + PHP 조합이 하던 일(요청 받고 → 처리하고 → 응답)을 Node.js + Express가 JavaScript로 하는 거라고 생각하면 돼요.

*/