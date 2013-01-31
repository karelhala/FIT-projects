<?php include("core.php"); ?>
<?php
	$login = $_POST['login'];
	$pass = $_POST['pass'];

	//nebylo zadáno ani jméno ani heslo
	if (($login == "") || ($pass == "")){
		header("Location: index.php");
	} else {
		$result = mysql_query('SELECT * FROM Technik t WHERE t.login="'.$login.'"');

		//uživatel neexistuje
		if(!$result) {
			header("Location: index.php?bad=1");

		} else {
			$row = mysql_fetch_array($result);
		  	$dbpass = $row['heslo'];
		  	$prava = $row['prava'];
		 // 	die($dbpass." ".$pass."\n".md5($pass));

		  	//pokud sedí hesla, vytvoří se sezení a uživatel se přihlásí
		  	if(md5($pass) == $dbpass) {
		  		session_start();
		  		$_SESSION['login'] = $login;
		  		$_SESSION['prava'] = $prava;
		  		//$_SESSION['time'] = time();
		  		//$_SESSION['timout'] = 600;
		  		if($prava < 10) {
		  			header("Location: admin/uzivatel/index.php");
		  		} elseif ($prava < 20) {
		  			header("Location: hlavni/priradit_plany/index.php");
		  		} elseif ($prava < 30) {
		  			header("Location: technik/testy/index.php");
		  		}


		  	} else {
		  		header("Location: index.php?bad=1");
		  	}
		}
	}

?>
