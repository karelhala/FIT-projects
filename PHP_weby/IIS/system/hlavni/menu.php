<!-- navigation starts-->	
<div  id="nav">
	<ul>
		<?php

			if(!isset($current)) {
				$current = -1;
			}
			if($current == 3) {
				echo '<li id="current"><a href="index.php">Plány</a></li>';
			} else {
				echo '<li><a href="../priradit_plany/index.php">Plány</a></li>';
			}
			if($current == 2) {
				echo '<li id="current"><a href="index.php">Přehled plánů</a></li>';
			} else {
				echo '<li><a href="../plany/index.php">Přehled plánů</a></li>';
			}

			if($current == 4) {
				echo '<li id="current"><a href="index.php">Letadla</a></li>';
			} else {
				echo '<li><a href="../pridat_letadlo/index.php">Letadla</a></li>';
			}
			if($current == 5) {
				echo '<li id="current"><a href="index.php">Certifikáty</a></li>';
			} else {
				echo '<li><a href="../certifikat_technikum/index.php">Certifikáty</a></li>';
			}
			if($current == 6) {
				echo '<li id="current"><a href="index.php">Testy</a></li>';
			} else {
				echo '<li><a href="../test/index.php">Testy</a></li>';
			}
			if($current == 7) {
				echo '<li id="current"><a href="index.php">Mé testy</a></li>';
			} else {
				echo '<li><a href="../testy/index.php">Mé testy</a></li>';
			}
			if($current == 1) {
				echo '<li id="current"><a href="index.php">Změna hesla</a></li>';
			} else {
				echo '<li><a href="../heslo/index.php">Změna hesla</a></li>';
			}
			
		?>
	</ul>
	<div id="logout">
		<p>
			<input class="button" type="button" onclick='location.href="../../logout.php"' value="Odhlásit"/>	
		</p>			
	</div>
<!-- navigation ends-->	
</div>


<?php
	$path = "../../logout.php";		
	$minPrava = 10;
	include("../../session.php");
?>
