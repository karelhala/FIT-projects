<?php


	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");
	

	if (! isset($id_test)) {
		if(isset($_GET['id'])) {
			$id_test = $_GET['id'];
		} else {
			$id_test = -1;
		}
	}

	if (isset($id_test)) {

		$result = mysql_query('SELECT t.login, t.jmeno, t.prijmeni FROM Technik_ma_certi tc LEFT JOIN Certifikat_k_testu ct ON tc.ID_cert = ct.ID_cert LEFT JOIN Technik t ON tc.ID_technik = t.login WHERE ct.ID_test="'.$id_test.'" GROUP BY tc.ID_Technik HAVING COUNT(tc.ID_Cert) = (SELECT COUNT(ID_cert) FROM Certifikat_k_testu ct LEFT JOIN Test t ON ct.ID_test = t.ID WHERE t.ID="'.$id_test.'")');

		while ($row = mysql_fetch_array($result)) {
			echo '<option value="'.$row['login'];
			echo '">'.$row['prijmeni']." ".$row['jmeno']." (".$row['login'].")";
		}	
	}

?>