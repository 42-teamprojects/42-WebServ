<?php

if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    $filename = "test";
    
    $directory = '/Users/yelaissa/42-WebServ/www/uploads/';

    $filepath = $directory . $filename;
    if (file_exists($filepath)) {
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
