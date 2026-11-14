<?php
header('Content-Type: application/json; charset=utf-8');
header('Access-Control-Allow-Origin: *');

$servername = "localhost";
$username = "cse20221627";
$password = "cse20221627";
$dbname = "db_cse20221627";

// 1. MySQL 연결
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    echo json_encode([
        'status' => 'error',
        'message' => 'Connection failed: ' . $conn->connect_error
    ]);
    exit();
}

// 2. 파라미터 수신
$show_all         = isset($_GET['show_all']) && $_GET['show_all'] === '1';
$search_type      = $_GET['search_type'] ?? 'user_id';
$search_keyword   = trim($_GET['search_keyword'] ?? '');
$search_from_date = trim($conn->real_escape_string($_GET['search_from_date'] ?? ''));
$search_gender    = trim($conn->real_escape_string($_GET['search_gender'] ?? ''));
$sort_order       = $_GET['sort_order'] ?? 'reg_desc';

// 정렬 기준
$order_by = "reg_date DESC";
if ($sort_order === 'reg_asc') {
    $order_by = "reg_date ASC";
} elseif ($sort_order === 'id_asc') {
    $order_by = "user_student_id ASC";
}

// 3. 쿼리 생성
if ($show_all) {
    $page_title = "전체 등록자 목록";
    $sql = "SELECT * FROM HOMEWORK2 ORDER BY " . $order_by;
} else {
    $page_title = "회원 검색 결과";
    $where_clauses = ["1=1"];

    // 키워드 조건 분기 (6개 항목)
    if (!empty($search_keyword)) {
        $escaped_kw = $conn->real_escape_string($search_keyword);

        switch ($search_type) {
            case 'lastname': // 성
                $where_clauses[] = "user_lastname LIKE '%$escaped_kw%'";
                break;

            case 'firstname': // 이름
                $where_clauses[] = "user_firstname LIKE '%$escaped_kw%'";
                break;

            case 'both_name': // 성/이름 (성 AND 이름)
                // 스페이스바(공백)를 기준으로 성과 이름을 2개로 분리
                $parts = preg_split('/\s+/', $search_keyword, 2);

                if (count($parts) >= 2) {
                    // 첫 번째 단어: 성 / 두 번째 단어: 이름
                    $ln = $conn->real_escape_string($parts[0]);
                    $fn = $conn->real_escape_string($parts[1]);
                    $where_clauses[] = "(user_lastname LIKE '%$ln%' AND user_firstname LIKE '%$fn%')";
                } else {
                    // 스페이스바 없이 1단어만 입력한 경우: 성 또는 이름 중 하나라도 일치하면 검색
                    $where_clauses[] = "(user_lastname LIKE '%$escaped_kw%' OR user_firstname LIKE '%$escaped_kw%')";
                }
                break;

            case 'user_id': // 아이디
                $where_clauses[] = "user_id LIKE '%$escaped_kw%'";
                break;

            case 'student_id': // 학번
                $where_clauses[] = "user_student_id LIKE '%$escaped_kw%'";
                break;

            case 'email': // 이메일
                $where_clauses[] = "user_email LIKE '%$escaped_kw%'";
                break;
        }
    }

    // 특정 일자 이후 등록자 조건
    if (!empty($search_from_date)) {
        $where_clauses[] = "reg_date >= '$search_from_date 00:00:00'";
    }

    // 성별 필터
    if (!empty($search_gender)) {
        $where_clauses[] = "user_gender = '$search_gender'";
    }

    $sql = "SELECT * FROM HOMEWORK2 WHERE " . implode(" AND ", $where_clauses) . " ORDER BY " . $order_by;
}

$result = $conn->query($sql);

$rows = [];
if ($result && $result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        $file_data_uri = null;
        if (!empty($row['user_file']) && file_exists($row['user_file'])) {
            $img_info = @getimagesize($row['user_file']);
            $mime_type = $img_info['mime'] ?? 'image/jpeg';
            $img_data = base64_encode(file_get_contents($row['user_file']));
            $file_data_uri = "data:" . $mime_type . ";base64," . $img_data;
        }

        $rows[] = [
            'id'              => $row['id'],
            'user_id'         => $row['user_id'],
            'user_lastname'   => $row['user_lastname'],
            'user_firstname'  => $row['user_firstname'],
            'user_gender'     => $row['user_gender'],
            'user_student_id' => $row['user_student_id'],
            'user_birthdate'  => $row['user_birthdate'],
            'user_tel'        => $row['user_tel'],
            'user_email'      => $row['user_email'],
            'user_url'        => $row['user_url'],
            'user_color'      => $row['user_color'],
            'user_file_data'  => $file_data_uri,
            'user_comment'    => $row['user_comment'],
            'reg_date'        => $row['reg_date']
        ];
    }
}

echo json_encode([
    'status'     => 'success',
    'page_title' => $page_title,
    'count'      => count($rows),
    'data'       => $rows
]);

$conn->close();
?>
