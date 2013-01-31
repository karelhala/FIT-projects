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
		$current = 3;
		include("../menu.php"); 
	?>	
	<!-- content starts -->
	<div id="content">
	
		<div id="main">

			<?php

				if(isset($_GET['msg'])) {
					if ($_GET['msg'] == 4) {
						$msg = '<span style="color:red">Nebylo zadáno jméno.</span>';
					} elseif ($_GET['msg'] == 5) {
						$msg = '<span style="color:red">Nebylo zadáno příjmení.</span>';
					} elseif ($_GET['msg'] == 6) {
						$msg = '<span style="color:red">Chybně zadaný telefon.</span>';
					} elseif ($_GET['msg'] == 7) {
						$msg = '<span style="color:red">Chybně zadané rodné číslo.</span>';
					} elseif ($_GET['msg'] == 8) {
						$msg = '<span style="color:red">Nezadáno heslo.</span>';
					}
					echo '<table><tr><td>'.$msg.'</td></tr></table>';
				}

				if(isset($_GET['vytvorit'])) {
					$vytvor = true;
				} elseif (isset($_GET['id'])) {
					$vytvor = false;
					$id = $_GET['id'];
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
