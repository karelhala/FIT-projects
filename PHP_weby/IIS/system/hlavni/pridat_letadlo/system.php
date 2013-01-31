<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
	
<link rel="stylesheet" href="../../images/style.css" type="text/css" />
<?php include("../../head.php"); ?>	
<?php include("../../core.php"); ?>
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
					if ($_GET['msg'] == 4) {
						$msg = '<span style="color:red">Nebyl zadán typ.</span>';
					} elseif ($_GET['msg'] == 5) {
						$msg = '<span style="color:red">Špatně zadaný typ.</span>';
					} elseif ($_GET['msg'] == 6) {
						$msg = '<span style="color:red">Nebylo zadané sériové číslo.</span>';
					} elseif ($_GET['msg'] == 7) {
						$msg = '<span style="color:red">Špatně zadané sériové číslo.</span>';
					} 
					echo '<table><tr><td>'.$msg.'</td></tr></table>';
				}

				if(isset($_GET['vytvorit'])) {

				} elseif (isset($_GET['letadlo'])) {
					$result = mysql_query('SELECT * FROM Letadlo t WHERE t.ID ="'.$_GET['letadlo'].'"');
					
					$row = mysql_fetch_array($result);

				}

				include('form.php');
			?>

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
