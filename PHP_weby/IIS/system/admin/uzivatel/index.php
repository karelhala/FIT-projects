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
		$current = 0;
		include("../menu.php"); 
	?>		

	<!-- content starts -->
	<div id="content">
	
		<div id="main">

			<?php
				if(isset($_GET['msg'])) {
					if($_GET['msg'] == 1) {
						$msg = 'Zaměstnanec byl odstraněn.';
					} elseif ($_GET['msg'] == 2) {
						$msg = 'Byl vytvořen zaměstnanec s přihlašovacím jménem "'.$_GET['login'].'".';
					} elseif ($_GET['msg'] == 3) {
						$msg = 'Údaje o zaměstnancovi byly změněny.';
					}

					echo '<table><tr><td>'.$msg.'</td></tr></table>';
				}
			?>

			<form action="system.php" method="get">
				<table>
				<tr>
					<td colspan="2" align="center">
						<input class="button" type="button" onClick="location.href='system.php?vytvorit=1'" value="Přidat nového zaměstnance"/>
					</td>
				</tr>
				<tr>
					<td>
						<select name="uzivatel" size="1">
						<?php

							$result = mysql_query('SELECT * FROM Technik t WHERE t.prava < 100');

							while ($row = mysql_fetch_array($result)) {
								echo '<option value="'.$row['login'].'">'.$row['jmeno']." ".$row['prijmeni'] ;
							}
						?>
						</select>
					</td>
					<td>
						<input class="button" type="submit" value="Upravit zaměstnance"/>
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
