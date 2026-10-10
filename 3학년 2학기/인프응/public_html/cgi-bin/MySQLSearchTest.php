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

$sql = "SELECT id, firstname, lastname, email FROM THJ_MyGuests WHERE lastname='" . $_GET["last_name"] . "'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
  // output data of each row
  while($row = $result->fetch_assoc()) {
    echo "id: " . $row["id"]. " - Name: " . $row["firstname"]. " " . $row["lastname"]. $row["email"]. "<br>";
  }
} else {
  echo "0 results";
}

$conn->close();
?>
