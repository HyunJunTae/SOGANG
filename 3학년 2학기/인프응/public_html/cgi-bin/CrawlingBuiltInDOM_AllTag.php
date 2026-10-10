<?php

$url = $_POST["url"];
$html = file_get_contents($url);

$dom = new DOMDocument();
$dom->loadHTML($html);

echo "Image Tags <br>" ;
$nodes = $dom->getElementsByTagName("img");

foreach ($nodes as $node) {
	echo $node->getAttribute("src") . "<br/>";
	echo $node->getAttribute("alt") . "<br/>";
	echo $node->nodeValue . "<br/>";
}

echo "Anchor Tags <br>" ;
$nodes = $dom->getElementsByTagName("a");

foreach ($nodes as $node) {
	echo $node->getAttribute("href") . "<br/>";
	echo $node->getAttribute("onclick") . "<br/>";
	echo $node->nodeValue . "<br/>";
}

echo "Script Tags <br>" ;
$nodes = $dom->getElementsByTagName("script");

foreach ($nodes as $node) {
	echo $node->nodeValue . "<br/>";
}

echo "Link Tags <br>" ;
$nodes = $dom->getElementsByTagName("link");

foreach ($nodes as $node) {
	echo $node->getAttribute("href") . "<br/>";
	echo $node->getAttribute("rel") . "<br/>";
	echo $node->nodeValue . "<br/>";
}
?>
