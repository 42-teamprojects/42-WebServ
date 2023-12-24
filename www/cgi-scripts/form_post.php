<?php
// SQLite database setup
$databaseFile = 'todos.db';
$pdo = new PDO('sqlite:' . $databaseFile);
$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

// Create the todos table if it doesn't exist
$createTableQuery = "
    CREATE TABLE IF NOT EXISTS todos (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        task TEXT NOT NULL
    )
";
$pdo->exec($createTableQuery);

// Handle POST requests to add a new todo
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST["task"])) {
    $task = $_POST["task"];
    $insertQuery = "INSERT INTO todos (task) VALUES (:task)";
    $stmt = $pdo->prepare($insertQuery);
    $stmt->bindParam(':task', $task, PDO::PARAM_STR);
    $stmt->execute();
}

// Handle GET requests to delete a todo
if (isset($_GET["delete"]) && is_numeric($_GET["delete"])) {
    $deleteId = $_GET["delete"];
    $deleteQuery = "DELETE FROM todos WHERE id = :id";
    $stmt = $pdo->prepare($deleteQuery);
    $stmt->bindParam(':id', $deleteId, PDO::PARAM_INT);
    $stmt->execute();
}

// Retrieve todos from the database
$selectQuery = "SELECT * FROM todos";
$todos = $pdo->query($selectQuery)->fetchAll(PDO::FETCH_ASSOC);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Simple TODO App</title>
</head>
<body>

<h2>TODO App</h2>

<!-- Form to add a new todo -->
<form method="post" action="/cgi/form_post.php">
    <label for="task">New Task:</label>
    <input type="text" name="task" required>
    <button type="submit">Add Task</button>
</form>

<!-- List of existing todos -->
<ul>
    <?php foreach ($todos as $todo): ?>
        <li>
            <?php echo htmlspecialchars($todo["task"]); ?>
            <a href="?delete=<?php echo $todo["id"]; ?>" onclick="return confirm('Are you sure?')">Delete</a>
        </li>
    <?php endforeach; ?>
</ul>

</body>
</html>
