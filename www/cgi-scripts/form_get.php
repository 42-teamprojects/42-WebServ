<?php
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Print the submitted data
    $name = $_GET['name'];
    $email = $_GET['email'];
    echo "<p>Name: $name</p>";
    echo "<p>Email: $email</p>";
}
?>
