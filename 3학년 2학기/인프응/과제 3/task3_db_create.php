<?php
$servername = "localhost";
$username = "cse20221627";
$password = "cse20221627";
$dbname = "db_cse20221627";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// HOMEWORK2 테이블 생성
$sql = "CREATE TABLE IF NOT EXISTS HOMEWORK2 (
    id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    user_id VARCHAR(50) NOT NULL UNIQUE,       -- 아이디 (중복 방지 UNIQUE)
    user_pw VARCHAR(255) NOT NULL,             -- 비밀번호
    user_lastname VARCHAR(30) NOT NULL,        -- 성
    user_firstname VARCHAR(30) NOT NULL,       -- 이름
    user_gender VARCHAR(10),                   -- 성별
    user_student_id VARCHAR(20),               -- 학번
    user_birthdate DATE,                       -- 생년월일
    user_tel VARCHAR(30),                      -- 전화번호
    user_email VARCHAR(100),                   -- 이메일
    user_url VARCHAR(255),                     -- URL
    user_color VARCHAR(7),                     -- 좋아하는 색깔
    user_file VARCHAR(255),                    -- 업로드 파일 경로
    user_comment TEXT,                         -- 코멘트
    reg_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
)";

if ($conn->query($sql) === TRUE) {
    echo "Table 'HOMEWORK2' created successfully!";
} else {
    echo "Error creating table: " . $conn->error;
}

$conn->close();
?>