<?php
	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");

	//CAST ZPRACOVAVAJICI DATA Z FORMULARE
	if(isset($_POST['action'])) {
		
		//odtraneni zaznamu
		if($_POST['action'] == 'Odstranit') {


			echo $_POST['login'];
			//$result = mysql_query("DELETE FROM Technik WHERE login = '".$_POST['login']."' LIMIT 1");
			$result = mysql_query('UPDATE Technik SET prava = 100 WHERE login = "'.$_POST['login'].'"');
			if(!$result) {echo mysql_error();}
			$result = mysql_query('DELETE FROM Technik_ma_certi WHERE ID_Technik = "'.$_POST['login'].'"');

			header("Location: index.php?msg=1");


		} else {

			$heslo = $_POST['heslo'];
			$jm = $_POST['jmeno'];
			$pr = $_POST['prijmeni'];
			$tel = $_POST['tel_cislo'];
			$rc = $_POST['rc'];
			$prava = $_POST['prava'];

			//vytvoreni zaznamu
			if ($_POST['action'] == 'Vytvořit') {

				$jm = iconv("utf-8", "ASCII//IGNORE", $_POST['jmeno']);
				$pr = iconv("utf-8", "ASCII//IGNORE", $_POST['prijmeni']);

				$login = substr($pr, 0, 4).substr($jm, 0, 2);	
				$login = strtolower($login);
				$num = -1;

				do {
					$num += 1;
					$result = mysql_query("SELECT * FROM Technik WHERE login='".$login.$num."'");
				} while ($sup = mysql_fetch_array($result));

				$login = $login.$num;

				//nezadáno heslo
				if($heslo == "") {
					$msg = 8;
				} else {
					$heslo = md5($heslo);
					$msg=2;
				}

			//uprava stavajiciho zaznamu
			} elseif ($_POST['action'] == 'Uložit') {				
				$login = $_POST['login'];
				$msg=3;
			}

			//nebylo zadáno jméno
			if ($_POST['jmeno'] == "") {
				$msg = 4;
			}

			//nebylo zadáno příjmení
			if ($_POST['prijmeni'] == "") {
				$msg = 5;
			}

			//test telefonu
			if ($tel != "")  {
				if(!is_numeric($tel) || strlen($tel)!=9) {
					$msg = 6;
				}
			}

			//test rodneho cisla
			if ($rc != "")  {
				if(!is_numeric($rc)||strlen($rc)!=10) {
					$msg = 7;
				}
			}


			if($msg <= 3) {
				if($msg == 3) {
					//upraveni stareho zaznamu
					$result = mysql_query('UPDATE Technik SET prava="'.$prava.'", jmeno="'.$_POST['jmeno'].'", prijmeni="'.$_POST['prijmeni'].'", RC="'.$rc.'", tel_cislo="'.$tel.'"WHERE login = "'.$login.'"');
				}
				else{
					$result = mysql_query('INSERT INTO Technik SET heslo="'.$heslo.'", login="'.$login.'", prava="'.$prava.'", jmeno="'.$_POST['jmeno'].'", prijmeni="'.$_POST['prijmeni'].'", tel_cislo="'.$tel.'", RC="'.$rc.'"');
				}

				header("Location: index.php?msg=".$msg."&login=".$login);

			} else {	

				if($_POST['action'] == 'Uložit') {
					header("Location: system.php?msg=".$msg."&uzivatel=".$login);				
				} else {					
					header("Location: system.php?msg=".$msg."&vytvorit=1");	
				}
			}
		}
	//CAST VYTVAREJICI FORMULAR
	} else {

		$vytvor = false;

		if(!isset($row)) {
			$vytvor = true;
			$row['jmeno'] = "";		
			$row['prijmeni'] = "";
			$row['tel_cislo'] = "";
			$row['RC'] = "";
			$row['login'] = "";
			$row['heslo'] = "";
			$row['prava'] = -1;
		} 
?>
		<form action="form.php" method="post">
		<table>
			<?php if( ! $vytvor) { ?>
			<tr>
				<td>Login:</td>
				<td><input disabled readonly type="text" name="login" value="<?=$row['login']?>"/></td>
			</tr>
			<?php } ?>
			<tr>
				<td>Jméno:</td>
				<td><input type="text" name="jmeno" value="<?=$row['jmeno']?>"/>*</td>
				<td>Příjmeni:</td>
				<td><input type="text" name="prijmeni" value="<?=$row['prijmeni']?>"/>*</td>
			</tr>
			<tr>
				<td>Telefon:</td>
				<td><input type="text" name="tel_cislo" value=
				<?php
					if($row['tel_cislo'] == 0) {
						echo '""';
					} else {
						echo '"'.$row['tel_cislo'].'"';
					}
				?>/></td>
				<td>Rodné číslo:</td>
				<td><input type="text" name="rc" value=
				<?php
					if($row['RC'] == 0) {
						echo '""';
					} else {
						echo '"'.$row['RC'].'"';
					}
				?>/></td>
			</tr>
			<tr>
			<?php if($vytvor) { ?>
				<td>Heslo:</td>
				<td><input type="text" name="heslo" value="<?=$row['heslo']?>"/>*</td>
			<?php } ?>
				<td>Kategorie:</td>
				<td>
				<select name="prava" size="1">
				<option value=0 <?php if($row['prava'] == 0) {echo 'selected="selected"';} ?> >Administrátor
				<option value=10 <?php if($row['prava'] == 10) {echo 'selected="selected"';} ?>  >Hlavní technik
				<option value=20 <?php if($row['prava'] == 20) {echo 'selected="selected"';} ?>  >Technik
				</select>
				</td>
			</tr>
		</table>


		<table>
			<tr>
				<td><input class="button" type="button" onClick="location.href='index.php'" value="Zpět"/></td>
				<?php if($vytvor) { ?>
					<td><input class="button" name="action" type="submit" value="Vytvořit"/></td>
				<?php } else { ?>
					<td><input class="button" name="action" type="submit" value="Uložit"/></td>
					<td><input class="button" name="action" type="submit" value="Odstranit"/></td>
					<td><input type="hidden" name="heslo" value="<?=$row['heslo']?>"/></td>
				<?php } ?>
			</tr>
		</table>
		<input type="hidden" name="login" value="<?=$row['login']?>"/>
		</form>
<?php	} ?>
