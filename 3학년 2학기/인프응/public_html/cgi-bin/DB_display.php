<?php
$servername = "localhost";
$username = "cse20221627";
$password = "cse20221627";
$dbname = "db_cse20221627";

// 1. MySQL 연결
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
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
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title><?= $page_title ?></title>
    <style>
        body { font-family: sans-serif; margin: 20px; }
        table { border-collapse: collapse; width: 100%; margin-top: 15px; }
        th, td { border: 1px solid #ccc; padding: 8px 12px; text-align: left; }
        th { background-color: #f2f2f2; text-align: center; }
        tr:hover { background-color: #f9f9f9; }
        .color-box { display: inline-block; width: 14px; height: 14px; border: 1px solid #333; vertical-align: middle; }
        .nav-links { margin-bottom: 15px; }
    </style>
</head>
<body>
    <h2><?= $page_title ?></h2>
    
    <div class="nav-links">
        <a href="../search.html">[검색 화면으로]</a> | 
        <a href="DB_display.php?show_all=1">[전체 등록자 새로고침]</a> | 
        <a href="../RegForm.html">[신규 회원 등록]</a>
    </div>

    <p>
        총 <strong><?= $result ? $result->num_rows : 0 ?></strong>명의 회원이 조회되었습니다.
        <?php if (!$show_all && (!empty($search_keyword) || !empty($search_from_date) || !empty($search_gender))): ?>
            <br><small style="color: gray;">적용 쿼리: <?= htmlspecialchars($sql) ?></small>
        <?php endif; ?>
    </p>

    <!-- 회원 목록 테이블 출력 -->
    <table>
        <thead>
            <tr>
                <th>번호</th>
                <th>아이디</th>
                <th>성</th>
                <th>이름</th>
                <th>성별</th>
                <th>학번</th>
                <th>생년월일</th>
                <th>전화번호</th>
                <th>이메일</th>
                <th>URL</th>
                <th>색상</th>
                <th>사진</th>
                <th>코멘트</th>
                <th>등록일시</th>
            </tr>
        </thead>
        <tbody>
            <?php if ($result && $result->num_rows > 0): ?>
                <?php while ($row = $result->fetch_assoc()): ?>
                    <tr>
                        <td align="center"><?= $row['id'] ?></td>
                        <td><strong><?= htmlspecialchars($row['user_id']) ?></strong></td>
                        <td><?= htmlspecialchars($row['user_lastname']) ?></td>
                        <td><?= htmlspecialchars($row['user_firstname']) ?></td>
                        <td align="center"><?= htmlspecialchars($row['user_gender']) ?></td>
                        <td align="center"><?= htmlspecialchars($row['user_student_id'] ?: '-') ?></td>
                        <td align="center"><?= htmlspecialchars($row['user_birthdate'] ?: '-') ?></td>
                        <td><?= htmlspecialchars($row['user_tel'] ?: '-') ?></td>
                        <td><?= htmlspecialchars($row['user_email'] ?: '-') ?></td>
                        <td>
                            <?php if (!empty($row['user_url'])): ?>
                                <a href="<?= htmlspecialchars($row['user_url']) ?>" target="_blank">링크</a>
                            <?php else: ?>
                                -
                            <?php endif; ?>
                        </td>
                        <td align="center">
                            <?php if (!empty($row['user_color'])): ?>
                                <span class="color-box" style="background-color: <?= htmlspecialchars($row['user_color']) ?>;"></span>
                                <?= htmlspecialchars($row['user_color']) ?>
                            <?php else: ?>
                                -
                            <?php endif; ?>
                        </td>
                        <td align="center">
                            <?php 
                            if (!empty($row['user_file']) && file_exists($row['user_file'])): 
                                // 이미지 정보 및 바이너리 데이터를 Base64로 인코딩
                                $img_info = getimagesize($row['user_file']);
                                $mime_type = $img_info['mime'] ?? 'image/jpeg';
                                $img_data = base64_encode(file_get_contents($row['user_file']));
                            ?>
                                <!-- 클릭하면 원본 크기로도 볼 수 있게 링크 + 이미지 태그 결합 -->
                                <a href="data:<?= $mime_type ?>;base64,<?= $img_data ?>" target="_blank">
                                    <img src="data:<?= $mime_type ?>;base64,<?= $img_data ?>" 
                                        alt="프로필" 
                                        style="width: 50px; height: 50px; object-fit: cover; border-radius: 4px; border: 1px solid #ddd;">
                                </a>
                            <?php else: ?>
                                <span style="color: gray;">없음</span>
                            <?php endif; ?>
                        </td>
                        <td><?= nl2br(htmlspecialchars($row['user_comment'] ?: '-')) ?></td>
                        <td align="center"><small><?= $row['reg_date'] ?></small></td>
                    </tr>
                <?php endwhile; ?>
            <?php else: ?>
                <tr>
                    <td colspan="14" align="center" style="padding: 30px; color: #888;">
                        조건에 일치하는 등록 정보가 없습니다.
                    </td>
                </tr>
            <?php endif; ?>
        </tbody>
    </table>
</body>
</html>

<?php
$conn->close();
?>