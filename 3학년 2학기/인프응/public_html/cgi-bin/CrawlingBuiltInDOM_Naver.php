<?php

$url = $_POST["url"];
$html = file_get_contents($url);

$dom = new DOMDocument();
$dom->loadHTML($html);


$xpath = new DOMXPath($dom);
// Find the input element with type="hidden"
$hiddenInputs = $xpath->query('//input[@type="hidden"]');
if ($hiddenInputs->length > 0) {
	foreach($hiddenInputs AS $hiddenInput) {
    		$typeAttribute = $hiddenInput->getAttribute('type');
    		echo "type: " . $typeAttribute . ", ";
    		$idAttribute = $hiddenInput->getAttribute('id');
    		echo "id : " . $idAttribute . ", ";
    		$nameAttribute = $hiddenInput->getAttribute('name');
    		echo "name : " . $nameAttribute;
		echo "<br>";
	}

}
