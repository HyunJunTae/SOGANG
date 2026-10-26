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