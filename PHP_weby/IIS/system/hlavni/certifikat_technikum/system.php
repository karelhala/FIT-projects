<?php

	include("../../core.php");
	
	if(! isset($target)) {
		$target = $_GET['target'];
		$include = true;
	} else {
		$include = false;
		echo $target;
	}


	if($target == "technik_add") {

		if (! isset($login)) {
				$login = $_GET['login'];
		}

		$result = mysql_query('SELECT c.ID, c.jmeno FROM Certifikat c LEFT JOIN Technik_ma_certi tc ON tc.ID_Technik = "'.$login.'" AND c.ID = tc.ID_Cert WHERE tc.ID_Technik IS NULL');


	} else if($target == "technik_del") {

		if (! isset($login)) {
				$login = $_GET['login'];
		}

		$result = mysql_query('SELECT c.ID, c.jmeno FROM Certifikat c LEFT JOIN Technik_ma_certi tc ON c.ID = tc.ID_Cert WHERE tc.ID_Technik = "'.$login.'"');

	} else if($target == "test_add") {

		if (! isset($test)) {
				$test = $_GET['test'];
		}

		$result = mysql_query('SELECT c.ID, c.jmeno FROM Certifikat c LEFT JOIN Certifikat_k_testu ct ON ct.ID_test = "'.$test.'" AND c.ID = ct.ID_cert WHERE ct.ID_test IS NULL');


	} else if($target == "test_del") {

		if (! isset($test)) {
				$test = $_GET['test'];
		}

		$result = mysql_query('SELECT c.ID, c.jmeno FROM Certifikat c LEFT JOIN Certifikat_k_testu ct ON c.ID = ct.ID_cert WHERE ct.ID_test = "'.$test.'"');
	}

	if($result) {
		while ($row = mysql_fetch_array($result)) {
			if($include) {
				$jm = iconv("utf-8", "latin2", $row['jmeno']);
			} else {
				$jm = $row['jmeno'];
			}
			echo '<option value="'.$row['ID'].'">'.$jm;
		}
	}

?>