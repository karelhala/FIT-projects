<?php
	header("Content-Type: text/html; charset=utf-8");		
	include("../../core.php");

	session_start();
	session_regenerate_id();

	if(($_GET['action'] == 'Odebrat') && isset($_GET['technik_certifikat2'])){
		$result = mysql_query('DELETE FROM Technik_ma_certi WHERE ID_Technik="'.$_GET['technik2'].'" AND  ID_Cert="'.$_GET['technik_certifikat2'].'" LIMIT 1');


		$sup = mysql_query('SELECT mt.ID_Plany 
			FROM Ma_provest mt LEFT JOIN Plany p ON mt.ID_Plany = p.ID 
			LEFT JOIN Certifikat_k_testu ct ON p.ID_test = ct.ID_test 
			LEFT JOIN Technik_ma_certi tc ON ct.ID_cert = tc.ID_cert AND tc.ID_Technik = "'.$_GET['technik2'].'"
			WHERE mt.login_technika = "'.$_GET['technik2'].'" AND tc.ID_Cert IS NULL AND Hotovo = 0 GROUP BY p.ID');

		$plany = array();
		while ($row = mysql_fetch_array($sup)) {
			$plany[] = $row['ID_Plany'];
		}

		foreach($plany as $id) {
			$sup = mysql_query('DELETE FROM Ma_provest WHERE login_technika="'.$_GET['technik2'].'" AND ID_Plany="'.$id.'"');
			echo mysql_error()." ".$_GET['technik2']." ".$id." ! ";
		}	

	} else if(($_GET['action'] == 'Přidat') && isset($_GET['technik_certifikat'])){ 
		$result = mysql_query('SELECT * FROM Technik_ma_certi WHERE ID_Technik="'.$_GET['technik'].'" AND ID_Cert="'.$_GET['technik_certifikat'].'"');
		if (mysql_num_rows($result)==0){
			$result = mysql_query('INSERT INTO Technik_ma_certi SET ID_Technik="'.$_GET['technik'].'",  ID_Cert="'.$_GET['technik_certifikat'].'"');
		}

	} else if(($_GET['action'] == 'Odebrat') && isset($_GET['letadlo_certifikat2'])){
			$result = mysql_query('DELETE FROM Certifikat_k_testu WHERE ID_test="'.$_GET['letadlo2'].'" AND  ID_cert="'.$_GET['letadlo_certifikat2'].'" LIMIT 1');

	} else if (($_GET['action'] == 'Přidat') && isset($_GET['letadlo_certifikat'])){

		$result = mysql_query('SELECT * FROM Certifikat_k_testu WHERE ID_test="'.$_GET['letadlo'].'" AND ID_Cert="'.$_GET['letadlo_certifikat'].'"');
		if (mysql_num_rows($result)==0){
			$result = mysql_query('INSERT INTO Certifikat_k_testu SET ID_test="'.$_GET['letadlo'].'",  ID_cert="'.$_GET['letadlo_certifikat'].'"');
		}


		$sup = mysql_query('SELECT mt.ID_Plany, mt.login_technika 
			FROM Ma_provest mt LEFT JOIN Plany p ON mt.ID_Plany = p.ID 
			LEFT JOIN Certifikat_k_testu ct ON p.ID_test = ct.ID_test 
			LEFT JOIN Technik_ma_certi tc ON ct.ID_cert = tc.ID_cert AND tc.ID_Technik = mt.login_technika AND tc.ID_Cert = "'.$_GET['letadlo_certifikat'].'"
			WHERE tc.ID_Cert IS NULL AND Hotovo = 0 AND ct.ID_Cert = "'.$_GET['letadlo_certifikat'].'"');

		$data = array();
		while ($row = mysql_fetch_array($sup)) {

			$entry = array(
				'id' => $row['ID_Plany'],
				'login' => $row['login_technika']
			);
			$data[] = $entry;
		}

		foreach($data as $entry) {
			$sup = mysql_query('DELETE FROM Ma_provest WHERE login_technika="'.$entry['login'].'" AND ID_Plany="'.$entry['id'].'"');
			echo mysql_error()." ".$entry['login']." ".$entry['id']." ! ";
		}	

	}

	header("Location: index.php?");


?>
