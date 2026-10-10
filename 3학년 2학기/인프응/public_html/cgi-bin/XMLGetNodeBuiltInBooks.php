
<?php
$xmlDoc = new DOMDocument();
$xmlDoc->load("books.xml");

$nodes = $xmlDoc->getElementsByTagName('book');

foreach($nodes as $node) {
	$findme = $node->getElementsByTagName('title');
	echo $findme[0]->nodeValue . "<br/>" ;
}

?>

