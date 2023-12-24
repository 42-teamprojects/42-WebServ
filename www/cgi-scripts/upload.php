<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>File Upload</title>
</head>
<body>

<h2>File Upload</h2>

<!-- Form to upload a file -->
<form method="post" action="/cgi/upload.php" enctype="multipart/form-data">
    <label for="file">Choose File:</label>
    <input type="file" name="file" required>

    <br>

    <button type="submit">Upload File</button>
</form>

<?php
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["file"])) {
    $uploadDir = '../uploads/uploads/';
    $uploadFile = $uploadDir . basename($_FILES["file"]["name"]);

    // Check for errors
    if ($_FILES["file"]["error"] !== UPLOAD_ERR_OK) {
        $errorMessages = [
            UPLOAD_ERR_INI_SIZE => "The uploaded file exceeds the upload_max_filesize directive in php.ini",
            UPLOAD_ERR_FORM_SIZE => "The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form",
            UPLOAD_ERR_PARTIAL => "The uploaded file was only partially uploaded",
            UPLOAD_ERR_NO_FILE => "No file was uploaded",
            UPLOAD_ERR_NO_TMP_DIR => "Missing a temporary folder",
            UPLOAD_ERR_CANT_WRITE => "Failed to write file to disk",
            UPLOAD_ERR_EXTENSION => "A PHP extension stopped the file upload"
        ];

        $errorMessage = $errorMessages[$_FILES["file"]["error"]] ?? "Unknown error";
        echo "<p>There was an error uploading the file: $errorMessage</p>";
    } else if (move_uploaded_file($_FILES["file"]["tmp_name"], $uploadFile)) {
        echo "<p>File uploaded successfully. Path: $uploadFile</p>";
    } else {
        echo "<p>There was an error moving the uploaded file.</p>";
    }
}
?>

</body>
</html>
