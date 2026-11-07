<?php
header('Content-Type: application/json; charset=UTF-8');

// 검색어 받기
$q = $_GET['q'] ?? ''; /* (1) 여기를 채워러 */  

// 배열 등 잘못된 입력은 거부
if (!is_string($q)) {
    http_response_code(400);

    echo json_encode([
        'error' => '검색어는 문자열이어야 합니다.'
    ], JSON_UNESCAPED_UNICODE);

    exit;
}

$q = trim($q);

// 실습용 도서 데이터. 그냥 php 배열.
$books = [
    [
        'title' => 'jQuery Basics',
        'author' => 'Kim',
        'keywords' => 'jquery javascript web'
    ],
    [
        'title' => 'Ajax Practice',
        'author' => 'Lee',
        'keywords' => 'jquery ajax json'
    ],
    [
        'title' => 'PHP Introduction',
        'author' => 'Park',
        'keywords' => 'php server web'
    ],
    [
        'title' => 'Python Programming',
        'author' => 'Choi',
        'keywords' => 'python programming'
    ]
];

$results = [];

if ($q !== '') {
    foreach ($books as $book) {
        // 제목과 키워드를 합쳐 검색
        $searchText = $book['title'] . ' ' . $book['keywords']; /* (2) 여기를 채워라 */ 

        // 대소문자를 구분하지 않고 포함 여부 확인
        if (stripos($searchText, $q /* (3) 여기를 채원라 */ ) !== false) {
            // 제목과 저자만 결과에 추가
            $results[] = [
                'title' => $book['title'] /* (4) 여기를 채워라 */,
                'author' => $book['author']/* (5) 여기를 채워라 */ 
            ];
        }
    }
}

// 검색 결과를 JSON으로 응답
echo json_encode($results, JSON_UNESCAPED_UNICODE);
?>
