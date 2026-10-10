
<?php
$xmlDoc = new DOMDocument();
$xmlDoc->load("school.xml");

$students = $xmlDoc->getElementsByTagName('student');

foreach($students AS $student) {
	$studentName =  $student->getAttribute('name') ; 
	echo("Name : $studentName") ; 
	$studentProp = $student->childNodes;
	foreach($studentProp AS $Prop) {
		if ($Prop->nodeType === XML_ELEMENT_NODE) 
			echo " $Prop->tagName ==> $Prop->nodeValue , " ; }
  	echo "<br>";
}
?>

