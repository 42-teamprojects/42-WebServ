<?php
function parseQueryString($queryString)
{
    $params = array();
    parse_str($queryString, $params);
    return $params;
}
if ($_SERVER['REQUEST_METHOD'] === 'GET') {

    $data = parseQueryString($_SERVER['QUERY_STRING']);
    // Retrieve and sanitize the data
    $name = isset($data['name']) ? htmlspecialchars($data['name']) : 'notprovided';
    $email = isset($data['email']) ? htmlspecialchars($data['email']) : 'notprovided';

    // Print the submitted data
    echo "<p>Name: $name</p>";
    echo "<p>Email: $email</p>";
}
?>
