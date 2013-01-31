<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
<script src="../../js/jquery-1.8.3.min.js"></script>
<link rel="stylesheet" href="../../images/style.css" type="text/css" />
<?php include("../../head.php"); ?>	
<?php include("../../core.php"); ?>
</head>

<body>
			
<!-- wrap starts here -->
<div id="wrap">
		
	<!-- navigační lišta-->
	<?php 
		$current = 2;
		include("../menu.php"); 
		
		$login = $_SESSION['login'];		
		$result = mysql_query('SELECT l.typ, l.seriovec, p.Datum_zadani, tt.popis, p.ID FROM Ma_provest t LEFT JOIN Plany p ON (t.ID_Plany=p.ID) LEFT JOIN Letadlo l ON (ID_letadlo=l.ID) LEFT JOIN Test tt ON tt.ID = p.ID_test WHERE t.login_technika="'.$login.'" AND Hotovo = 0');

		?>		

	<!-- content starts -->
	<div id="content">
	
		<div id="main">
						
				<table>
					<?php while($row = mysql_fetch_array($result)){ ?>
					<form action="form.php" method="post">
						<tr>
							<td>Letadlo:</td>
							<td>
								<input type="text" disabled readonly value="<?php print_r($row['typ']); ?>"/>
							</td>
							<td>Sériové číslo:</td>
							<td>
								<input type="text" disabled readonly value="<?php print_r($row['seriovec']); ?>"/>
							</td>
						</tr>
						<tr>
							<td>Datum zadání:</td>
							<td><input type="text" disabled readonly value="<?php echo $row['Datum_zadani'] ?>"/></td>
							<td>Test:</td>
							<td><input type="text" disabled readonly value="<?php echo $row['popis'] ?>"/></td>
						</tr>
						<tr>
							<td>
								<input class="button"  type="button" value="Přidat výsledek testu" onclick="$('.textove_pole<?= $row['ID'] ?>').slideToggle('fast')"/>
							</td>
							<td colspan="2" ><textarea name="result" rows="5"  cols="35"  style="display:none;" class="textove_pole<?= $row['ID'] ?>"></textarea></td>
							<td><input type="submit"  value="Potvrdit" style="display:none;" class="button textove_pole<?= $row['ID'] ?>"/></td>
						</tr>
						<input type="hidden" name="ID" value="<?=$row['ID'] ?>"/>
						</form>
					<?php } ?>
				</table>			
		<!-- main ends -->	
		</div>

	<!-- content ends-->	
	</div>
		
	<!-- zápatí lišta-->
	<?php include("../../footer.php"); ?>


<!-- wrap ends here -->
</div>

</body>
</html>
