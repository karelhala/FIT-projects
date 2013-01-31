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
		$current = 1;
		include("../menu.php"); 
	?>		

	<!-- content starts -->
	<div id="content">
	
		<div id="main">

			<?php
				if(isset($_GET['msg'])) {
					if($_GET['msg'] == 1) {
						$msg = 'Heslo změněno.';
					} elseif ($_GET['msg'] == 2) {
						$msg = '<span style="color:red">Staré heslo nesouhlasí.</span>';
					} elseif ($_GET['msg'] == 3) {
						$msg = '<span style="color:red">Nová hesla se neshodují.</span>';
					} elseif ($_GET['msg'] == 3) {
						$msg = '<span style="color:red">Nebylo zadáno nové heslo.</span>';
					}

					echo '<table><tr><td>'.$msg.'</td></tr></table>';
				}
			?>


			<form action="form.php" method="post">
				<table>
				<tr>
					<td>Staré heslo:</td>
					<td><input type="password" name="old" value=""/>*</td>
				</tr>
				<tr>
					<td>Nové heslo:</td>
					<td><input type="password" name="new1" value=""/>*</td>
				</tr>
				<tr>
					<td>Potvrzení nového hesla:</td>
					<td><input type="password" name="new2" value=""/>*</td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input class="button" type="submit" value="Změnit heslo"/>
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
