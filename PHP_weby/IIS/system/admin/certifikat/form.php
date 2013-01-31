<?php
	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");

	//CAST ZPRACOVAVAJICI DATA Z FORMULARE
	if(isset($_POST['action'])) {
		//odtraneni zaznamu
		if($_POST['action'] == 'Odstranit') {


			echo $_POST['ID'];
			$result = mysql_query("DELETE FROM Certifikat WHERE ID = '".$_POST['ID']."' LIMIT 1");
			if(!$result) {echo mysql_error();}

			header("Location: index.php?msg=1");

		} else {

			$jmeno = $_POST['jmeno'];
			$popis = $_POST['popis'];

			//vytvoreni zaznamu
			if ($_POST['action'] == 'Vytvořit') {
				$msg=2;

			//uprava stavajiciho zaznamu
			} elseif ($_POST['action'] == 'Uložit') {

				$msg=3;
			}

			//nebylo zadáno jméno
			if ($jmeno == ""){
				$msg = 4;
			}
			else if ($jmeno != "") {
				if(strlen($jmeno) <4) {
					$msg = 5;
				}
			}

			if($msg <= 3) {
				if($msg == 3) {
					$result = mysql_query('UPDATE Certifikat SET jmeno="'.$jmeno.'", popis="'.$popis.'" WHERE ID = '.$_POST['ID']);
				}
				else
				{
					$result = mysql_query('INSERT INTO Certifikat SET jmeno="'.$jmeno.'", popis="'.$popis.'"');
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
			$row['jmeno'] = "";		
			$row['popis'] = "";
		} 
?>
		<form action="form.php" method="post">
		<table>
			<tr>
				<td>Jméno:</td>
				<td><input type="text" name="jmeno" value="<?=$row['jmeno']?>"/>*</td>
				<td>Popis:</td>
				<td><input type="text" name="popis" value="<?=$row['popis']?>"/></td>
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
