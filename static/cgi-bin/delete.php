<?php

if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    $filename = "test";
    
    // Specify the directory where your files are stored
    $directory = '/Users/yelaissa/42-WebServ/static/uploads/';

    // Construct the full path to the file
    $filepath = $directory . $filename;
    echo $filepath . "\n";
    // Check if the file exists before attempting to delete
    if (file_exists($filepath)) {
        // Attempt to delete the file
        if (unlink($filepath)) {
            echo "File deleted successfully.";
        } else {
            echo "Error deleting the file.";
        }
    } else {
        echo "File not found.";
    }
}
else {
    echo "This script only works with DELETE requests.";
}

?>
