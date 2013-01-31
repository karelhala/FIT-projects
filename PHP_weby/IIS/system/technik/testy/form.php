<?php
	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");
	session_start();
	session_regenerate_id();

	$date = date("Y-m-d");

	$result = mysql_query('UPDATE Plany SET vysledek="'.$_POST['result'].'", Hotovo=1, Datum_ukonceni="'.$date.'" WHERE ID = '.$_POST['ID']);
	header("Location: index.php");
?>
