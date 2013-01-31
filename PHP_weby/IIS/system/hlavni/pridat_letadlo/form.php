<?php
	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");

	//CAST ZPRACOVAVAJICI DATA Z FORMULARE
	if(isset($_POST['action'])) {
		//odtraneni zaznamu
		if($_POST['action'] == 'Odstranit') {


			echo $_POST['ID'];
			$result = mysql_query("DELETE FROM Letadlo WHERE ID = '".$_POST['ID']."' LIMIT 1");
			if(!$result) {echo mysql_error();}

			header("Location: index.php?msg=1");

		} else {

			$typ = $_POST['typ'];
			$spz = $_POST['spz'];

			//vytvoreni zaznamu
			if ($_POST['action'] == 'Vytvořit') {
				$msg=2;

				$typ = iconv("utf-8", "ASCII//IGNORE", $_POST['typ']);
				$spz = iconv("utf-8", "ASCII//IGNORE", $_POST['spz']);

			//uprava stavajiciho zaznamu
			} elseif ($_POST['action'] == 'Uložit') {

				$msg=3;
			}

			//nebylo zadáno jméno
			if ($typ == ""){
				$msg = 4;
			}
			else if ($typ != "") {
				if(strlen($typ) <4) {
					$msg = 5;
				}
			}
			if ($spz == ""){
				$msg = 6;
			}
			else if ($spz != "") {
				if(strlen($spz) <4) {
					$msg = 7;
				}
			}

			if($msg <= 3) {
				if($msg == 3) {
					$result = mysql_query('UPDATE Letadlo SET typ="'.$typ.'", seriovec="'.$spz.'" WHERE ID = '.$_POST['ID']);
				}
				else
				{
					$result = mysql_query('INSERT INTO Letadlo SET typ="'.$typ.'", ID="'.$ID.'", seriovec="'.$spz.'"');
				}
				header("Location: index.php?msg=".$msg);

			} else {				
				header("Location: system.php?msg=".$msg);				
			}
		}
	//CAST VYTVAREJICI FORMULAR
	} else {

		$vytvor = false;

		if(!isset($row)) {
			$vytvor = true;
			$row['typ'] = "";		
			$row['seriovec'] = "";
		} 
?>
		<form action="form.php" method="post">
		<table>
			<tr>
				<td>Jméno:</td>
				<td><input type="text" name="typ" value="<?=$row['typ']?>"/>*</td>
				<td>Sériové číslo:</td>
				<td><input type="text" name="spz" value="<?=$row['seriovec']?>"/></td>
			</tr>
		</table>

		<table>
			<tr>
				<td><input class="button" type="button" onClick="location.href='index.php'" value="Zpět"/></td>
				<?php if($vytvor): ?>
					<td><input class="button" name="action" type="submit" value="Vytvořit"/></td>
				<?php else: ?>
					<td><input class="button" name="action" type="submit" value="Uložit"/></td>
					<td><input class="button" name="action" type="submit" value="Odstranit"/></td>
				<?php endif; ?>
			</tr>
		</table>
		<input type="hidden" name="ID" value="<?=$row['ID'] ?>"/>
		</form>
	<?php } ?>
