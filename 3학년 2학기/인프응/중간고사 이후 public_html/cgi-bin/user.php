<?php
header("Content-Type: application/json; charset=UTF-8");

if ($_SERVER["REQUEST_METHOD"] === "GET") {
    // GET: URL 쿼리에서 읽기
    $name = $_GET["name"] ?? "";
    $age = (int)($_GET["age"] ?? 0);

} 

elseif ($_SERVER["REQUEST_METHOD"] === "POST") {
    // POST: JSON 요청 본문에서 읽기
    $data = json_decode(
        file_get_contents("php://input"),
        true
    );

    $name = $data["name"] ?? "";
    $age = (int)($data["age"] ?? 0);

} 

else {
    http_response_code(405);
    header("Allow: GET, POST");

    echo json_encode(["message" => "지원하지 않는 요청 방식"]);
    exit;
}

// 공통 응답
echo json_encode(
    ["message" => $name . " 수신 완료"],
    ["age_message" => $age . " 수신 완료"],
    JSON_UNESCAPED_UNICODE
);
