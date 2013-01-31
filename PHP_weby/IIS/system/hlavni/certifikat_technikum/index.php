<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">

<head>
<script src="../../js/jquery-1.8.3.min.js"></script>
<link rel="stylesheet" href="../../images/style.css" type="text/css" />
<?php include("../../core.php"); ?>
<?php include("../../head.php"); ?>	
</head>

<body>
			
<!-- wrap starts here -->
<div id="wrap">
		
	<!-- navigační lišta-->
	<?php 
		$current = 5;
		include("../menu.php"); 
	?>		

	<!-- content starts -->
	<div id="content">
	
		<div id="main">

			<form action="form.php" method="get">
			<table>
			<tr>
				<td>Certifikáty techniků:</td>
			</tr>
			<tr>
				<td>
					<select name="technik" size="1" Id="technik_select" onChange="refresh();">
					<?php
						$result = mysql_query('SELECT * FROM Technik WHERE prava>=10 AND prava < 100'); 
						$row = mysql_fetch_array($result);
						//login prvního tehcnika v selectu
						$login = $row['login'];
						do {
							echo '<option value="'.$row['login'].'">'.$row['prijmeni']." ".$row['jmeno'].", (".$row['login'].")" ;
						} while ($row = mysql_fetch_array($result));
					?>
					</select>
					<script>
						function refresh() {
							var e = document.getElementById("technik_select");
							var login = e.options[e.selectedIndex].value;
							var ajax = new XMLHttpRequest();
							ajax.onreadystatechange = function() {
								if (ajax.readyState == 4) {						
							        data = ajax.responseText;
									document.getElementById("technik_cert").innerHTML = data;
								}
							};
							ajax.open("GET", "system.php?login="+login + "&target=technik_add", true);
							ajax.send(null);
						}

					</script>
				</td>
				<td>
					<select name="technik_certifikat" size="1" id="technik_cert">
					<?php
						$target = "technik_add";
						include("system.php");
					?>
					</select>						
				</td>
				<td>
					<input class="button" type="submit" name="action" value="Přidat"/>
				</td>
			</tr>

			<tr>
				<td>
					<select name="technik2" size="1" id="technik_select2" onChange="refresh2()">
					<?php
						$result = mysql_query('SELECT * FROM Technik WHERE prava>=10 AND prava < 100'); 
						$row = mysql_fetch_array($result);
						//login prvního tehcnika v selectu
						$login = $row['login'];
						do {
							echo '<option value="'.$row['login'].'">'.$row['prijmeni']." ".$row['jmeno'].", (".$row['login'].")" ;
						} while ($row = mysql_fetch_array($result));
					?>
					</select>
					<script>
						function refresh2() {
							var e = document.getElementById("technik_select2");
							var login = e.options[e.selectedIndex].value;
							var ajax = new XMLHttpRequest();
							ajax.onreadystatechange = function() {
								if (ajax.readyState == 4) {
									document.getElementById("technik_cert2").innerHTML = ajax.responseText;
								}
							};
							ajax.open("GET", "system.php?login="+login + "&target=technik_del", true);
							ajax.send(null);
						}
					</script>
				</td>
				<td>						
					<select name="technik_certifikat2" size="1" id="technik_cert2">
					<?php
						$target = "technik_del";
						include("system.php");
					?>
					</select>							
				</td>
				<td>
					<input class="button" type="submit" name="action" value="Odebrat"/>
				</td>
			</tr>			
			</table>
			</form>	
		
			<form action="form.php" method="get">	
			<table>
			<tr>
				<td>Certifikáty testů:</td>
			</tr>				
			<tr>
				<td>
					<select name="letadlo" size="1" id="letadlo_select" onchange="refresh3();">
					<?php
						$result = mysql_query('SELECT * FROM Test ');
						$row = mysql_fetch_array($result);
						$test = $row['ID'];
						do {
							echo '<option value="'.$row['ID'].'">'.$row['popis'] ;
						} while ($row = mysql_fetch_array($result));
					?>
					</select>
					<script>
						function refresh3() {
							var e = document.getElementById("letadlo_select");
							var test = e.options[e.selectedIndex].value;
							var ajax = new XMLHttpRequest();
							ajax.onreadystatechange = function() {
								if (ajax.readyState == 4) {
									 document.getElementById("letadlo_cert").innerHTML = ajax.responseText;
								}
							};
							ajax.open("GET", "system.php?test="+test + "&target=test_add", true);
							ajax.send(null);
						}
					</script>
				</td>
				<td>
					<select name="letadlo_certifikat" size="1" id="letadlo_cert">
					<?php
						$target = "test_add";
						include("system.php");
					?>
					</select>
				</td>
				<td>
					<input class="button" type="submit" name="action"  value="Přidat"/>
				</td>
			</tr>


			<tr>
				<td>
					<?php $result = mysql_query('SELECT * FROM Test'); ?>
					<select name="letadlo2" size="1" id="letadlo_select2" onchange="refresh4()">
					<?php
						$result = mysql_query('SELECT * FROM Test ');
						$row = mysql_fetch_array($result);
						$test = $row['ID'];
						do {
							echo '<option value="'.$row['ID'].'">'.$row['popis'] ;
						} while ($row = mysql_fetch_array($result));
					?>
					</select>
					<script>
						function refresh4() {
							var e = document.getElementById("letadlo_select2");
							var test = e.options[e.selectedIndex].value;
							var ajax = new XMLHttpRequest();
							ajax.onreadystatechange = function() {
								if (ajax.readyState == 4) {
									 document.getElementById("letadlo_cert2").innerHTML = ajax.responseText;
								}
							};
							ajax.open("GET", "system.php?test="+test + "&target=test_del", true);
							ajax.send(null);
						}
					</script>
				</td>
				<td>
					<select name="letadlo_certifikat2" size="1" id="letadlo_cert2">
					<?php
						$target = "test_del";
						include("system.php");
					?>
					</select>
				</td>
				<td>
					<input class="button" type="submit" name="action" value="Odebrat"/>
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
