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

// 2. POST 데이터 수신 및 특수문자 이스케이프
$user_id             = $conn->real_escape_string($_POST['user_id'] ?? '');
$user_pw             = $conn->real_escape_string($_POST['user_pw'] ?? '');
$user_lastname       = $conn->real_escape_string($_POST['user_lastname'] ?? '');
$user_firstname      = $conn->real_escape_string($_POST['user_firstname'] ?? '');
$user_gender         = $conn->real_escape_string($_POST['user_gender'] ?? '');
$user_student_id     = $conn->real_escape_string($_POST['user_student_id'] ?? '');
$user_birthdate      = $conn->real_escape_string($_POST['user_birthdate'] ?? '');
$user_tel            = $conn->real_escape_string($_POST['user_tel'] ?? '');
$user_email          = $conn->real_escape_string($_POST['user_email'] ?? '');
$user_url            = $conn->real_escape_string($_POST['user_url'] ?? '');
$user_color          = $conn->real_escape_string($_POST['user_color'] ?? '');
$user_comment        = $conn->real_escape_string($_POST['user_comment'] ?? '');

// 3. 아이디 중복 확인 (이미 등록된 아이디인지 검사)
$check_sql = "SELECT user_id FROM HOMEWORK2 WHERE user_id = '$user_id'";
$check_result = $conn->query($check_sql);

if ($check_result && $check_result->num_rows > 0) {
    echo "<script>
        alert('이미 존재하는 아이디입니다. 다른 아이디를 입력해주세요.');
        history.back();
    </script>";
    exit();
}

// 4. 파일 업로드 처리
$user_file = "";
if (isset($_FILES['user_file']) && $_FILES['user_file']['error'] === UPLOAD_ERR_OK) {
    $upload_dir = "uploads/";
    if (!is_dir($upload_dir)) {
        mkdir($upload_dir, 0777, true);
    }
    
    $file_name = time() . "_" . basename($_FILES['user_file']['name']);
    $target_path = $upload_dir . $file_name;

    if (move_uploaded_file($_FILES['user_file']['tmp_name'], $target_path)) {
        $user_file = $target_path;
    }
}

// 5. 빈 날짜 값 NULL 처리
$val_birthdate = !empty($user_birthdate) ? "'$user_birthdate'" : "NULL";

// 6. DB INSERT 실행
$sql = "INSERT INTO HOMEWORK2 (
    user_id, user_pw, user_lastname, user_firstname,
    user_gender, user_student_id, user_birthdate,
    user_tel, user_email, user_url, user_color,
    user_file, user_comment
) VALUES (
    '$user_id', '$user_pw', '$user_lastname', '$user_firstname',
    '$user_gender', '$user_student_id', $val_birthdate,
    '$user_tel', '$user_email', '$user_url', '$user_color',
    '$user_file', '$user_comment'
)";

if ($conn->query($sql) === TRUE) {
    echo "<h2>회원 등록이 완료되었습니다!</h2>";
    echo "<p>등록 아이디: " . htmlspecialchars($user_id) . "</p>";
    echo "<p><a href='../RegForm.html'>입력 폼으로 돌아가기</a></p>";
    echo "<p><a href='../search.html'>유저 검색 창으로 이동</a></p>";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>