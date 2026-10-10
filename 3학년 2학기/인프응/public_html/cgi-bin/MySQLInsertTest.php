<?php
$servername = "localhost";
$username = "cse20221627";
$password = "cse20221627";
$dbname = "db_cse20221627";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$first_name = $_GET["first_name"];
$last_name = $_GET["last_name"];
$email = $_GET["email"];

$sql = "INSERT INTO THJ_MyGuests (firstname, lastname, email)
VALUES ('$first_name', '$last_name', '$email')";

if ($conn->query($sql) === TRUE) {
  echo "New record created successfully";
} else {
  echo "Error: " . $sql . "<br>" . $conn->error;
}


$conn->close();
?>
