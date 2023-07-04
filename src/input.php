<html>
<body>
<?php

echo "Hello";
//echo "Input has come here";	
$a = $_GET['city1'];
$ain = $a."\n";
$b = $_GET['city2'];
$bin = $b."\n";
$myfile = fopen("output.txt","w");
$input = $a . PHP_EOL . $b;
fwrite($myfile,$input);

fclose($myfile);
echo $a;

?>

</body>
</html>
