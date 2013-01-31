<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>

<link rel="stylesheet" href="../../images/style.css" type="text/css" />
<script src="../../js/jquery-1.8.3.min.js"></script>
<?php include("../../head.php"); ?>	
<?php include("../../core.php"); ?>
</head>

<body>
			
<!-- wrap starts here -->
<div id="wrap">
		
	<!-- navigační lišta-->
	<?php 
		$current = 3;
		include("../menu.php"); 
	?>		

	<!-- content starts -->
	<div id="content">
	
		<div id="main">

			<?php
				if(isset($_GET['msg'])) {
					if($_GET['msg'] == 1) {
						$msg = 'Plán byl vytvořen.';
					} elseif ($_GET['msg'] == 2) {
						$msg = 'Plán byl odstraněn.';
					} elseif ($_GET['msg'] == 3) {
						$msg = '<span style="color:red">Nebyl zadán typ testu.</span>';
					} elseif ($_GET['msg'] == 4) {
						$msg = 'Plán byl změněn';
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
					<td align="center">
						<input class="button" type="button" onClick="location.href='system.php?vytvorit=1'" value="Vytvořit nový plán"/>
					</td>
				</tr>
				<tr>
					<td align="center">
						<input class="button" type="button" onClick="location.href='grid.php'" value="Upravit plán"/>
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
