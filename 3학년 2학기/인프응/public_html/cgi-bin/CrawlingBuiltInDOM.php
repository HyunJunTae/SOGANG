<?php

$url = $_POST["url"];
$html = file_get_contents($url);

$dom = new DOMDocument();
$dom->loadHTML($html);

echo "<H1> Image Tags (Src & Alt) </H1> <br> <br>" ;
$nodes = $dom->getElementsByTagName("img");

foreach ($nodes as $node) {
	echo $node->getAttribute("src") . "<br/>";
	echo $node->getAttribute("alt") . "<br/>";
	echo $node->nodeValue . "<br/>";
}

?>
