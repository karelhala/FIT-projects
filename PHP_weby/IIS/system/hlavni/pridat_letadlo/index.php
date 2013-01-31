<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>

<link rel="stylesheet" href="../../images/style.css" type="text/css" />
<?php include("../../core.php"); ?>
<?php include("../../head.php"); ?>	
</head>

<body>
			
<!-- wrap starts here -->
<div id="wrap">
		
	<!-- navigační lišta-->
	<?php 
		$current = 4;
		include("../menu.php"); 
	?>		

	<!-- content starts -->
	<div id="content">
	
		<div id="main">

			<?php
				if(isset($_GET['msg'])) {
					if($_GET['msg'] == 1) {
						$msg = 'Letadlo bylo odstraněn.';
					} elseif ($_GET['msg'] == 2) {
						$msg = 'Letadlo bylo vytvořen.';
					} elseif ($_GET['msg'] == 3) {
						$msg = 'Údaje o letadlu byly změněny.';
					} elseif ($_GET['msg'] == 4) {
						$msg = '<span style="color:red">Nebylo zadáno jméno.</span>';
					} elseif ($_GET['msg'] == 5) {
						$msg = '<span style="color:red">Špatně zadané jméno.</span>';
					} elseif ($_GET['msg'] == 6) {
						$msg = '<span style="color:red">Nebyla zadána SPZ.</span>';
					} elseif ($_GET['msg'] == 7) {
						$msg = '<span style="color:red">Špatně zadaná SPZ.</span>';
					}

					echo '<table><tr><td>'.$msg.'</td></tr></table>';
				}
			?>

			<form action="system.php" method="get">
				<table>
				<tr>
					<td colspan="2" align="center">
						<input class="button" type="button" onClick="location.href='system.php?vytvorit=1'" value="Přidat nové letadlo"/>
					</td>
				</tr>
				<tr>
					<td>
						<select name="letadlo" size="1">
						<?php

							$result = mysql_query('SELECT * FROM Letadlo');

							while ($row = mysql_fetch_array($result)) {
								echo '<option value="'.$row['ID'].'">'.$row['typ'].", ".$row['seriovec'] ;
							}
						?>
						</select>
					</td>
					<td>
						<input class="button" type="submit" value="Upravit letadlo"/>
					</td>
				</tr>
				</table>
			</form>
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
