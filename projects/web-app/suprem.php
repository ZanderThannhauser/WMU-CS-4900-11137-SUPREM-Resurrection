<?php
	$input = $_POST['input'];
	
	$f = fopen('input.txt', 'w+');
	fwrite($f, $input);
	fwrite($f, "structure out=./str.actual\n");
	fclose($f);
	
	$exit_code = 0;
	
	passthru("./suprem ./input.txt &> output.txt", $exit_code);
	
	$output_txt = file_get_contents("output.txt");
	$output_str = file_get_contents("str.actual");
	
	echo json_encode(array(
		"stdout" => $output_txt,
		"str-data" => $output_str,
		"exit-code" => $exit_code));
?>
