<?php
	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");

	if(isset($_POST['old'])) {

		session_start();
		session_regenerate_id();

		$login = $_SESSION['login'];

		$result = mysql_query('SELECT * FROM Technik t WHERE t.login="'.$login.'"');
		$row = mysql_fetch_array($result);

		$old = $row['heslo'];

		//staré heslo sedí
		if(md5($_POST['old']) == $old) {

			if($_POST['new1'] != "") {

				//aktualizace hesla
				if($_POST['new1'] == $_POST['new2']) {

					$new = md5($_POST['new1']);
					$result = mysql_query('UPDATE Technik t SET heslo=\''.$new."' WHERE t.login='".$login."'");
					if(!$result) {
						echo mysql_error();
					}
					$id = 1;
				//zadané nové hesla se neshodují
				} else {
					$id = 3;
				}
			} else {
				$id = 4;
			}

		} else {
			$id = 2;
		}
		header("Location: index.php?msg=".$id);

	}

?>
