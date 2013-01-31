<?php
	header("Content-Type: text/html; charset=utf-8");	
	include("../../core.php");

	//CAST ZPRACOVAVAJICI DATA Z FORMULARE
	if(isset($_POST['action'])) {
		//odtraneni zaznamu
		if($_POST['action'] == 'Odstranit') {


			echo $_POST['ID'];
			$result = mysql_query("DELETE FROM Test WHERE ID = '".$_POST['ID']."' LIMIT 1");
			if(!$result) {
				echo mysql_error();
			} else {
				header("Location: index.php?msg=1");
			}

		} else {

			$popis = $_POST['popis'];
			$limity = $_POST['limity'];

			//vytvoreni zaznamu
			if ($_POST['action'] == 'Vytvořit') {
				$msg=2;

				$popis = iconv("utf-8", "ASCII//IGNORE", $_POST['popis']);
				$limity = iconv("utf-8", "ASCII//IGNORE", $_POST['limity']);

			//uprava stavajiciho zaznamu
			} elseif ($_POST['action'] == 'Uložit') {

				$msg=3;
			}

			//nebylo zadáno jméno
			if ($popis == ""){
				$msg = 4;
			}
			else if ($popis != "") {
				if(strlen($popis) <8) {
					$msg = 5;
				}
			}

			if($msg <= 3) {
				if($msg == 3) {
					$result = mysql_query('UPDATE Test SET popis="'.$popis.'", limity="'.$limity.'" WHERE ID = '.$_POST['ID']);

				}
				else
				{
					$result = mysql_query('INSERT INTO Test SET popis="'.$popis.'", limity="'.$limity.'"');
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
			$row['popis'] = "";		
			$row['limity'] = "";
		} 
?>
		<form action="form.php" method="post">
		<table>
			<tr>
				<td>Popis:</td>
				<td><input type="textarea" name="popis" value="<?=$row['popis']?>"/>*</td>
				<td>Limity:</td>
				<td><input type="textarea" name="limity" value="<?=$row['limity']?>"/></td>
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
