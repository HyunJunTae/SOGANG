<?php
header("Content-Type: application/json");

echo json_encode(["stock" => 12, "Memo" => "선물용"]);
?>




<!-- 

JSON 인코딩 디코딩

PHP

// 인코딩: 배열 → 문자열
$str = json_encode(["stock" => 12, "Memo" => "선물용"], JSON_UNESCAPED_UNICODE);
// '{"stock":12,"Memo":"선물용"}'

// 디코딩: 문자열 → 배열 (true를 줘야 연관 배열로 나와요)
$arr = json_decode($str, true);
echo $arr["stock"];   // 12

true를 빼면 객체로 나와서 $obj->stock처럼 써야 해요.



JavaScript

// 인코딩: 객체 → 문자열
const str = JSON.stringify({ stock: 12, Memo: "선물용" });

// 디코딩: 문자열 → 객체
const data = JSON.parse(str);
data.stock;   // 12 

Ajax에서는 보통 이렇게 이어져요. 
PHP가 json_encode로 문자열을 만들어 echo하고, JS에서 res.json()이 받아서 디코딩해요(JSON.parse와 같은 일). 
반대로 JS에서 서버로 JSON을 보낼 땐 JSON.stringify로 만들어 보내고 PHP가 json_decode로 풀어요.

-->



<!-- 
.json 함수와 .parse 함수 차이

아래 둘은 같은 결과.
const data = await res.json();

const text = await res.text();   // 본문을 문자열로 읽고
const data = JSON.parse(text);   // 직접 parse 
-->


<!-- 

php나 자바스크립트가 서로에게 넘겨줄 떄는 반드시 json_encode나 stringify를 해서 넘겨줘야해?

반드시는 아니에요. 
HTTP로 오가는 건 결국 다 텍스트라서, 양쪽이 같은 형식으로 읽기만 하면 돼요. 
JSON은 그중 가장 편한 약속일 뿐이에요.

PHP → JS

값 하나만 보낼 거면 echo "12"; 하고 JS에서 res.text()로 읽어도 돼요.

값이 여러 개거나 배열/객체 구조면 json_encode + res.json()이 편해요. 
직접 문자열로 만들면 JS에서 다시 쪼개야 하거든요.


JS → PHP

수업 예제처럼 URLSearchParams(GET 쿼리스트링이나 POST body)로 보내면 
stringify 없이 PHP에서 $_GET, $_POST로 바로 받아요. 이게 오히려 기본이에요.

JSON으로 보내려면 JSON.stringify 해서 body에 넣는데, 이때는 $_POST에 안 들어오고 json_decode(file_get_contents("php://input"), true)로 직접 꺼내야 해요. 
-->